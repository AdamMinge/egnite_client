#ifndef EGNITE_SERIALIZER_QT_SERIALIZER_H
#define EGNITE_SERIALIZER_QT_SERIALIZER_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QBitArray>
#include <QLine>
#include <QLineF>
#include <QMetaObject>
#include <QMetaProperty>
#include <QMetaType>
#include <QObject>
#include <QRect>
#include <QRectF>
#include <QSize>
/* ----------------------------------- Boost -------------------------------- */
#include <boost/serialization/nvp.hpp>
/* ----------------------------------- Local -------------------------------- */
#include "egnite/export.h"
/* -------------------------------------------------------------------------- */

namespace boost::serialization {

namespace detail {

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
concept IsRegisteredMetaType = !IsMetaTypeSpecialization<Type> && requires {
  qMetaTypeId<Type>();
};

template <typename Archive>
void serializeGadget(Archive& ar, QVariant& variant) {
  auto meta_object = variant.metaType().metaObject();
  for (auto index = 0; index < meta_object->propertyCount(); ++index) {
    auto meta_property = meta_object->property(index);
    auto property = meta_property.readOnGadget(variant.data());

    ar& boost::serialization::make_nvp(meta_property.name(), property);

    meta_property.writeOnGadget(variant.data(), property);
  }
}

}  // namespace detail

template <typename Archive, detail::IsRect Rect>
void serialize(Archive& ar, Rect& rect, const unsigned int version) {
  auto point = rect.topLeft();
  auto size = rect.size();

  ar& boost::serialization::make_nvp("p", point);
  ar& boost::serialization::make_nvp("s", size);
  rect = Rect(point, size);
}

template <typename Archive, detail::IsLine Line>
void serialize(Archive& ar, Line& line, const unsigned int version) {
  auto p1 = line.p1(), p2 = line.p2();

  ar& boost::serialization::make_nvp("p1", p1);
  ar& boost::serialization::make_nvp("p2", p2);
  line = Line(p1, p2);
}

template <typename Archive, detail::IsPoint Point>
void serialize(Archive& ar, Point& point, const unsigned int version) {
  auto x = point.x(), y = point.y();

  ar& boost::serialization::make_nvp("x", x);
  ar& boost::serialization::make_nvp("y", y);
  point = Point(x, y);
}

template <typename Archive, detail::IsSize Size>
void serialize(Archive& ar, Size& size, const unsigned int version) {
  auto width = size.width(), height = size.height();

  ar& boost::serialization::make_nvp("w", width);
  ar& boost::serialization::make_nvp("h", height);
  size = Size(width, height);
}

template <typename Archive, detail::IsTime Time>
void serialize(Archive& ar, Time& time, const unsigned int version) {
  auto text = time.toString(Qt::DateFormat::ISODate);

  ar& text;
  time = Time::fromString(text, Qt::DateFormat::ISODate);
}

template <typename Archive>
void serialize(Archive& ar, QStringList& chr, const unsigned int version) {}

template <typename Archive>
void serialize(Archive& ar, QBitArray& chr, const unsigned int version) {}

template <typename Archive>
void serialize(Archive& ar, QVariant& variant, const unsigned int version) {
#define EGNITE_VARIANT_SERIALIZE_CASE(meta_type, type) \
  case meta_type: {                                    \
    type value = variant.value<type>();                \
    ar& value;                                         \
    variant.setValue(std::move(value));                \
    break;                                             \
  }

  switch (variant.metaType().id()) {
    EGNITE_VARIANT_SERIALIZE_CASE(QMetaType::QUrl, QUrl);
    EGNITE_VARIANT_SERIALIZE_CASE(QMetaType::QUuid, QUuid);
    EGNITE_VARIANT_SERIALIZE_CASE(QMetaType::QRectF, QRectF);
    EGNITE_VARIANT_SERIALIZE_CASE(QMetaType::QRect, QRect);
    EGNITE_VARIANT_SERIALIZE_CASE(QMetaType::QLineF, QLineF);
    EGNITE_VARIANT_SERIALIZE_CASE(QMetaType::QLine, QLine);
    EGNITE_VARIANT_SERIALIZE_CASE(QMetaType::QPointF, QPointF);
    EGNITE_VARIANT_SERIALIZE_CASE(QMetaType::QPoint, QPoint);
    EGNITE_VARIANT_SERIALIZE_CASE(QMetaType::QSizeF, QSizeF);
    EGNITE_VARIANT_SERIALIZE_CASE(QMetaType::QSize, QSize);
    EGNITE_VARIANT_SERIALIZE_CASE(QMetaType::QTime, QTime);
    EGNITE_VARIANT_SERIALIZE_CASE(QMetaType::QDate, QDate);
    EGNITE_VARIANT_SERIALIZE_CASE(QMetaType::QDateTime, QDateTime);
    EGNITE_VARIANT_SERIALIZE_CASE(QMetaType::QStringList, QStringList);
    EGNITE_VARIANT_SERIALIZE_CASE(QMetaType::QString, QString);
    EGNITE_VARIANT_SERIALIZE_CASE(QMetaType::QBitArray, QBitArray);
    EGNITE_VARIANT_SERIALIZE_CASE(QMetaType::Float, float);
    EGNITE_VARIANT_SERIALIZE_CASE(QMetaType::Double, double);
    EGNITE_VARIANT_SERIALIZE_CASE(QMetaType::Bool, bool);
    EGNITE_VARIANT_SERIALIZE_CASE(QMetaType::ULongLong, qulonglong);
    EGNITE_VARIANT_SERIALIZE_CASE(QMetaType::LongLong, qlonglong);
    EGNITE_VARIANT_SERIALIZE_CASE(QMetaType::UInt, uint);
    EGNITE_VARIANT_SERIALIZE_CASE(QMetaType::Int, int);
    default:
      detail::serializeGadget(ar, variant);
      break;
  }

#undef EGNITE_VARIANT_SERIALIZE_CASE
}

template <typename Archive>
void serialize(Archive& ar, QObject& object, const unsigned int version) {
  for (auto& property_name : object.dynamicPropertyNames()) {
    auto property = object.property(property_name.constData());
    ar& boost::serialization::make_nvp(property_name, property);

    object.setProperty(property_name.constData(), property);
  }
}

template <typename Archive, detail::IsRegisteredMetaType Object>
void serialize(Archive& ar, Object& object, const unsigned int version) {
  QVariant variant = QVariant::fromValue(object);
  ar& variant;
  object = variant.value<Object>();
}

}  // namespace boost::serialization

BOOST_CLASS_IMPLEMENTATION(QString, primitive_type)
BOOST_CLASS_IMPLEMENTATION(QUrl, primitive_type)
BOOST_CLASS_IMPLEMENTATION(QUuid, primitive_type)

#endif  // EGNITE_SERIALIZER_QT_SERIALIZER_H