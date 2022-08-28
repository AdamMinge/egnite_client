#ifndef EGNITE_REST_DATA_SERIALIZER_H
#define EGNITE_REST_DATA_SERIALIZER_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QMetaType>
#include <QObject>
/* ---------------------------------- Standard ------------------------------ */
#include <memory>
/* ----------------------------------- Local -------------------------------- */
#include "egnite/export.h"
#include "egnite/rest/rest_global.h"
#include "egnite/serializer/cbor_archive.h"
#include "egnite/serializer/json_archive.h"
#include "egnite/serializer/qt_serializer.h"
#include "egnite/utils/binder.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

class EGNITE_API RestDataSerializer : public QObject {
  Q_OBJECT

 public:
  enum class Format { Json, Cbor };

 public:
  RestDataSerializer(QObject* parent = nullptr);
  ~RestDataSerializer() override;

  template <typename Object>
  RestData serialize(const Object& object, Format format) const;

  template <typename Object>
  Object deserialize(const RestData& data) const;
};

template <typename Object>
RestData RestDataSerializer::serialize(const Object& object,
                                       Format format) const {
  auto data = (format == Format::Json) ? RestData(QJsonValue{})
                                       : RestData(QCborValue{});

  std::visit(utils::overloaded{[&](std::nullopt_t) {},
                               [&](QJsonValue& data) {
                                 auto archive = serializer::JsonOArchive(data);
                                 archive& object;
                               },
                               [&](QCborValue& data) {
                                 auto archive = serializer::CborOArchive(data);
                                 archive& object;
                               }},
             data);
  return data;
}

template <typename Object>
Object RestDataSerializer::deserialize(const RestData& data) const {
  return std::visit(
      utils::overloaded{[&](std::nullopt_t) -> Object { return Object{}; },
                        [&](QJsonValue& data) -> Object {
                          auto object = Object{};
                          auto archive = serializer::JsonIArchive(data);
                          archive& object;
                          return object;
                        },
                        [&](QCborValue& data) -> Object {
                          auto object = Object{};
                          auto archive = serializer::CborIArchive(data);
                          archive& object;
                          return object;
                        }},
      const_cast<RestData&>(data));
}

}  // namespace egnite::rest

#endif  // EGNITE_REST_DATA_SERIALIZER_H