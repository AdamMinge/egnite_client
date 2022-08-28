#ifndef EGNITE_SERIALIZER_QT_SERIALIZER_H
#define EGNITE_SERIALIZER_QT_SERIALIZER_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QBitArray>
#include <QHash>
#include <QLine>
#include <QLineF>
#include <QList>
#include <QMap>
#include <QMetaObject>
#include <QMetaProperty>
#include <QMetaType>
#include <QMultiHash>
#include <QMultiMap>
#include <QObject>
#include <QQueue>
#include <QRect>
#include <QRectF>
#include <QSequentialIterable>
#include <QSize>
#include <QStack>
#include <QVector>
/* ----------------------------------- Boost -------------------------------- */
#include <boost/serialization/collection_size_type.hpp>
#include <boost/serialization/nvp.hpp>
/* ----------------------------------- Local -------------------------------- */
#include "egnite/export.h"
/* -------------------------------------------------------------------------- */

namespace boost::serialization {

namespace detail {

/* ---------------------------------- IsGadget ------------------------------ */

template <typename Type, typename = void>
struct IsGadget : std::false_type {};

template <typename Type>
struct IsGadget<
    Type,
    std::enable_if_t<QtPrivate::IsGadgetHelper<Type>::IsGadgetOrDerivedFrom>>
    : std::true_type {};

/* ----------------------------------- Traits ------------------------------- */

template <typename Type>
concept IsRect = std::is_same_v<Type, QRect> || std::is_same_v<Type, QRectF>;

template <typename Type>
concept IsLine = std::is_same_v<Type, QLine> || std::is_same_v<Type, QLineF>;

template <typename Type>
concept IsPoint = std::is_same_v<Type, QPoint> || std::is_same_v<Type, QPointF>;

template <typename Type>
concept IsSize = std::is_same_v<Type, QSize> || std::is_same_v<Type, QSizeF>;

template <typename Type>
concept IsTime = std::is_same_v<Type, QTime> || std::is_same_v<Type, QDate> ||
    std::is_same_v<Type, QDateTime>;

template <typename Type>
concept IsMetaTypeSpecialization = IsRect<Type> || IsLine<Type> ||
    IsPoint<Type> || IsSize<Type> || IsTime<Type>;

template <typename Type>
concept IsRegisteredMetaType =
    !IsMetaTypeSpecialization<Type> && IsGadget<Type>::value;

/* ------------------------------------ List -------------------------------- */

template <typename Archive>
bool trySerializeVariantList(Archive& ar, const QVariant& variant) {
  if (!variant.canConvert(QMetaType(QMetaType::QVariantList))) return false;

  auto list = variant.value<QVariantList>();
  auto count = collection_size_type(list.size());
  ar << BOOST_SERIALIZATION_NVP(count);

  if (count > 0) {
    ar << make_array<const QVariant, collection_size_type>(
        static_cast<const QVariant*>(&list.data()[0]), count);
  }

  return true;
}

template <typename Archive>
bool tryDeserializeVariantList(Archive& ar, QVariant& variant) {
  if (!variant.canConvert(QMetaType(QMetaType::QVariantList))) return false;

  auto list = variant.value<QVariantList>();
  auto count = collection_size_type(0);
  ar >> BOOST_SERIALIZATION_NVP(count);

  auto meta_container = variant.value<QSequentialIterable>().metaContainer();

  list.resize(count);
  for (auto& value : list) value = QVariant(meta_container.valueMetaType());

  if (count > 0) {
    ar >> make_array<QVariant, collection_size_type>(
              static_cast<QVariant*>(&list.data()[0]), count);
  }

  for (const auto& value : list)
    meta_container.addValue(variant.data(), value.data());

  return true;
}

/* ----------------------------------- Gadget ------------------------------- */

template <typename Archive>
bool trySerializeVariantGadget(Archive& ar, const QVariant& variant) {
  auto meta_type = variant.metaType();
  auto meta_object = meta_type.metaObject();
  if (!meta_type.flags().testFlag(QMetaType::IsGadget) || !meta_object)
    return false;

  for (auto index = 0; index < meta_object->propertyCount(); ++index) {
    auto meta_property = meta_object->property(index);
    auto property = meta_property.readOnGadget(variant.data());

    ar << boost::serialization::make_nvp(meta_property.name(), property);
  }

  return true;
}

template <typename Archive>
bool tryDeserializeVariantGadget(Archive& ar, QVariant& variant) {
  auto meta_type = variant.metaType();
  auto meta_object = meta_type.metaObject();
  if (!meta_type.flags().testFlag(QMetaType::IsGadget) || !meta_object)
    return false;

  for (auto index = 0; index < meta_object->propertyCount(); ++index) {
    auto meta_property = meta_object->property(index);
    auto property = QVariant(meta_property.metaType());

    ar >> boost::serialization::make_nvp(meta_property.name(), property);
    auto s = meta_property.writeOnGadget(variant.data(), property);
  }

  return true;
}

/* ---------------------------------- Base Type ----------------------------- */

template <typename Type, typename Archive>
bool trySerializeVariantBaseType(Archive& ar, const QVariant& variant,
                                 QMetaType::Type meta_type) {
  if (variant.metaType().id() == meta_type) {
    Type value = variant.value<Type>();
    ar << value;
    return true;
  }

  return false;
}

template <typename Type, typename Archive>
bool tryDeserializeVariantBaseType(Archive& ar, QVariant& variant,
                                   QMetaType::Type meta_type) {
  if (variant.metaType().id() == meta_type) {
    Type value;
    ar >> value;
    variant.setValue(std::move(value));
    return true;
  }

  return false;
}

/* ----------------------------------- Variant ------------------------------ */

template <typename Archive>
void serializeVariant(Archive& ar, const QVariant& variant) {
  // clang-format off
  using Serializer = std::function<bool(Archive&, const QVariant&)>;

  static auto type_to_serializer = std::map<int, Serializer*>{};
  static auto serializers = std::to_array<Serializer>({
    [](auto& a, auto& v){ return trySerializeVariantBaseType<QUrl>(a, v, QMetaType::QUrl); },
    [](auto& a, auto& v){ return trySerializeVariantBaseType<QUuid>(a, v, QMetaType::QUuid); },
    [](auto& a, auto& v){ return trySerializeVariantBaseType<QRectF>(a, v, QMetaType::QRectF); },
    [](auto& a, auto& v){ return trySerializeVariantBaseType<QRect>(a, v, QMetaType::QRect); },
    [](auto& a, auto& v){ return trySerializeVariantBaseType<QLineF>(a, v, QMetaType::QLineF); },
    [](auto& a, auto& v){ return trySerializeVariantBaseType<QLine>(a, v, QMetaType::QLine); },
    [](auto& a, auto& v){ return trySerializeVariantBaseType<QPointF>(a, v, QMetaType::QPointF); },
    [](auto& a, auto& v){ return trySerializeVariantBaseType<QPoint>(a, v, QMetaType::QPoint); },
    [](auto& a, auto& v){ return trySerializeVariantBaseType<QSizeF>(a, v, QMetaType::QSizeF); },
    [](auto& a, auto& v){ return trySerializeVariantBaseType<QSize>(a, v, QMetaType::QSize); },
    [](auto& a, auto& v){ return trySerializeVariantBaseType<QTime>(a, v, QMetaType::QTime); },
    [](auto& a, auto& v){ return trySerializeVariantBaseType<QDate>(a, v, QMetaType::QDate); },
    [](auto& a, auto& v){ return trySerializeVariantBaseType<QDateTime>(a, v, QMetaType::QDateTime); },
    [](auto& a, auto& v){ return trySerializeVariantBaseType<QString>(a, v, QMetaType::QString); },
    [](auto& a, auto& v){ return trySerializeVariantBaseType<float>(a, v, QMetaType::Float); },
    [](auto& a, auto& v){ return trySerializeVariantBaseType<double>(a, v, QMetaType::Double); },
    [](auto& a, auto& v){ return trySerializeVariantBaseType<bool>(a, v, QMetaType::Bool); },
    [](auto& a, auto& v){ return trySerializeVariantBaseType<qulonglong>(a, v, QMetaType::ULongLong); },
    [](auto& a, auto& v){ return trySerializeVariantBaseType<qlonglong>(a, v, QMetaType::LongLong); },
    [](auto& a, auto& v){ return trySerializeVariantBaseType<uint>(a, v, QMetaType::UInt); },
    [](auto& a, auto& v){ return trySerializeVariantBaseType<int>(a, v, QMetaType::Int); },
    
    [](auto& a, auto& v){ return trySerializeVariantGadget(a, v); },
    [](auto& a, auto& v){ return trySerializeVariantList(a, v); }});
  // clang-format on

  if (auto id = variant.metaType().id(); type_to_serializer.contains(id))
    (*type_to_serializer[id])(ar, variant);
  else {
    for (auto& serializer : serializers) {
      if (serializer(ar, variant)) {
        type_to_serializer.insert(
            std::make_pair(variant.metaType().id(), &serializer));
        break;
      }
    }
  }
}

template <typename Archive>
void deserializeVariant(Archive& ar, QVariant& variant) {
  // clang-format off
  using Deserializer = std::function<bool(Archive&, QVariant&)>;

  static auto type_to_deserializer = std::map<int, Deserializer*>{};
  static auto deserializers = std::to_array<Deserializer>({
    [](auto& a, auto& v){ return tryDeserializeVariantBaseType<QUrl>(a, v, QMetaType::QUrl); },
    [](auto& a, auto& v){ return tryDeserializeVariantBaseType<QUuid>(a, v, QMetaType::QUuid); },
    [](auto& a, auto& v){ return tryDeserializeVariantBaseType<QRectF>(a, v, QMetaType::QRectF); },
    [](auto& a, auto& v){ return tryDeserializeVariantBaseType<QRect>(a, v, QMetaType::QRect); },
    [](auto& a, auto& v){ return tryDeserializeVariantBaseType<QLineF>(a, v, QMetaType::QLineF); },
    [](auto& a, auto& v){ return tryDeserializeVariantBaseType<QLine>(a, v, QMetaType::QLine); },
    [](auto& a, auto& v){ return tryDeserializeVariantBaseType<QPointF>(a, v, QMetaType::QPointF); },
    [](auto& a, auto& v){ return tryDeserializeVariantBaseType<QPoint>(a, v, QMetaType::QPoint); },
    [](auto& a, auto& v){ return tryDeserializeVariantBaseType<QSizeF>(a, v, QMetaType::QSizeF); },
    [](auto& a, auto& v){ return tryDeserializeVariantBaseType<QSize>(a, v, QMetaType::QSize); },
    [](auto& a, auto& v){ return tryDeserializeVariantBaseType<QTime>(a, v, QMetaType::QTime); },
    [](auto& a, auto& v){ return tryDeserializeVariantBaseType<QDate>(a, v, QMetaType::QDate); },
    [](auto& a, auto& v){ return tryDeserializeVariantBaseType<QDateTime>(a, v, QMetaType::QDateTime); },
    [](auto& a, auto& v){ return tryDeserializeVariantBaseType<QString>(a, v, QMetaType::QString); },
    [](auto& a, auto& v){ return tryDeserializeVariantBaseType<float>(a, v, QMetaType::Float); },
    [](auto& a, auto& v){ return tryDeserializeVariantBaseType<double>(a, v, QMetaType::Double); },
    [](auto& a, auto& v){ return tryDeserializeVariantBaseType<bool>(a, v, QMetaType::Bool); },
    [](auto& a, auto& v){ return tryDeserializeVariantBaseType<qulonglong>(a, v, QMetaType::ULongLong); },
    [](auto& a, auto& v){ return tryDeserializeVariantBaseType<qlonglong>(a, v, QMetaType::LongLong); },
    [](auto& a, auto& v){ return tryDeserializeVariantBaseType<uint>(a, v, QMetaType::UInt); },
    [](auto& a, auto& v){ return tryDeserializeVariantBaseType<int>(a, v, QMetaType::Int); },
    
    [](auto& a, auto& v){ return tryDeserializeVariantGadget(a, v); },
    [](auto& a, auto& v){ return tryDeserializeVariantList(a, v); }});
  // clang-format on

  if (auto id = variant.metaType().id(); type_to_deserializer.contains(id))
    (*type_to_deserializer[id])(ar, variant);
  else {
    for (auto& deserializer : deserializers) {
      if (deserializer(ar, variant)) {
        type_to_deserializer.insert(
            std::make_pair(variant.metaType().id(), &deserializer));
        break;
      }
    }
  }
}

}  // namespace detail

/* ----------------------------------- QRect -------------------------------- */

template <typename Archive, detail::IsRect Rect>
void save(Archive& ar, const Rect& rect, const unsigned int version) {
  const auto point = rect.topLeft();
  const auto size = rect.size();

  ar << boost::serialization::make_nvp("p", point);
  ar << boost::serialization::make_nvp("s", size);
}

template <typename Archive, detail::IsRect Rect>
void load(Archive& ar, Rect& rect, const unsigned int version) {
  decltype(rect.topLeft()) point;
  decltype(rect.size()) size;

  ar >> boost::serialization::make_nvp("p", point);
  ar >> boost::serialization::make_nvp("s", size);
  rect = Rect(point, size);
}

template <typename Archive, detail::IsRect Rect>
void serialize(Archive& ar, Rect& rect, const unsigned int version) {
  split_free(ar, rect, version);
}

/* ----------------------------------- QLine -------------------------------- */

template <typename Archive, detail::IsLine Line>
void save(Archive& ar, const Line& line, const unsigned int version) {
  auto p1 = line.p1(), p2 = line.p2();

  ar << boost::serialization::make_nvp("p1", p1);
  ar << boost::serialization::make_nvp("p2", p2);
}

template <typename Archive, detail::IsLine Line>
void load(Archive& ar, Line& line, const unsigned int version) {
  decltype(line.p1()) p1, p2;

  ar >> boost::serialization::make_nvp("p1", p1);
  ar >> boost::serialization::make_nvp("p2", p2);
  line = Line(p1, p2);
}

template <typename Archive, detail::IsLine Line>
void serialize(Archive& ar, Line& line, const unsigned int version) {
  split_free(ar, line, version);
}

/* ---------------------------------- QPoint -------------------------------- */

template <typename Archive, detail::IsPoint Point>
void save(Archive& ar, const Point& point, const unsigned int version) {
  auto x = point.x(), y = point.y();

  ar << boost::serialization::make_nvp("x", x);
  ar << boost::serialization::make_nvp("y", y);
}

template <typename Archive, detail::IsPoint Point>
void load(Archive& ar, Point& point, const unsigned int version) {
  decltype(point.x()) x, y;

  ar >> boost::serialization::make_nvp("x", x);
  ar >> boost::serialization::make_nvp("y", y);
  point = Point(x, y);
}

template <typename Archive, detail::IsPoint Point>
void serialize(Archive& ar, Point& point, const unsigned int version) {
  split_free(ar, point, version);
}

/* ----------------------------------- QSize -------------------------------- */

template <typename Archive, detail::IsSize Size>
void save(Archive& ar, const Size& size, const unsigned int version) {
  auto width = size.width(), height = size.height();

  ar << boost::serialization::make_nvp("w", width);
  ar << boost::serialization::make_nvp("h", height);
}

template <typename Archive, detail::IsSize Size>
void load(Archive& ar, Size& size, const unsigned int version) {
  decltype(size.width()) width, height;

  ar >> boost::serialization::make_nvp("w", width);
  ar >> boost::serialization::make_nvp("h", height);
  size = Size(width, height);
}

template <typename Archive, detail::IsSize Size>
void serialize(Archive& ar, Size& size, const unsigned int version) {
  split_free(ar, size, version);
}

/* ----------------------------------- QTime -------------------------------- */

template <typename Archive, detail::IsTime Time>
void save(Archive& ar, const Time& time, const unsigned int version) {
  auto text = time.toString(Qt::DateFormat::ISODate);

  ar << text;
}

template <typename Archive, detail::IsTime Time>
void load(Archive& ar, Time& time, const unsigned int version) {
  QString text;

  ar >> text;
  time = Time::fromString(text, Qt::DateFormat::ISODate);
}

template <typename Archive, detail::IsTime Time>
void serialize(Archive& ar, Time& time, const unsigned int version) {
  split_free(ar, time, version);
}

/* ---------------------------------- QVariant ------------------------------ */

template <typename Archive>
void save(Archive& ar, const QVariant& variant, const unsigned int version) {
  detail::serializeVariant(ar, variant);
}

template <typename Archive>
void load(Archive& ar, QVariant& variant, const unsigned int version) {
  detail::deserializeVariant(ar, variant);
}

template <typename Archive>
void serialize(Archive& ar, QVariant& variant, const unsigned int version) {
  split_free(ar, variant, version);
}

/* ---------------------------------- QObject ------------------------------- */

template <typename Archive>
void save(Archive& ar, const QObject& object, const unsigned int version) {
  for (auto& property_name : object.dynamicPropertyNames()) {
    auto property = object.property(property_name.constData());
    ar << boost::serialization::make_nvp(property_name, property);
  }
}

template <typename Archive>
void load(Archive& ar, QObject& object, const unsigned int version) {
  for (auto& property_name : object.dynamicPropertyNames()) {
    auto meta_type = object.property(property_name.constData()).metaType();
    auto property = QVariant(meta_type);
    ar >> boost::serialization::make_nvp(property_name, property);

    object.setProperty(property_name.constData(), property);
  }
}

template <typename Archive>
void serialize(Archive& ar, QObject& object, const unsigned int version) {
  split_free(ar, object, version);
}

/* ---------------------------- RegisteredMetaType -------------------------- */

template <typename Archive, detail::IsRegisteredMetaType Object>
void save(Archive& ar, const Object& object, const unsigned int version) {
  QVariant variant = QVariant::fromValue(object);
  ar << variant;
}

template <typename Archive, detail::IsRegisteredMetaType Object>
void load(Archive& ar, Object& object, const unsigned int version) {
  QVariant variant(QMetaType(qMetaTypeId<Object>()));
  ar >> variant;
  object = variant.value<Object>();
}

template <typename Archive, detail::IsRegisteredMetaType Object>
void serialize(Archive& ar, Object& object, const unsigned int version) {
  split_free(ar, object, version);
}

}  // namespace boost::serialization

BOOST_CLASS_IMPLEMENTATION(QString, primitive_type)
BOOST_CLASS_IMPLEMENTATION(QUrl, primitive_type)
BOOST_CLASS_IMPLEMENTATION(QUuid, primitive_type)

#endif  // EGNITE_SERIALIZER_QT_SERIALIZER_H