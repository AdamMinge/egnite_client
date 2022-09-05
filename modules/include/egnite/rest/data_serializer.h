#ifndef EGNITE_REST_DATA_SERIALIZER_H
#define EGNITE_REST_DATA_SERIALIZER_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QMetaType>
#include <QObject>
/* ---------------------------------- Standard ------------------------------ */
#include <memory>
/* ----------------------------------- Egnite ------------------------------- */
#include <egnite/core/utils/binder.h>
#include <egnite/serializer/cbor_archive.h>
#include <egnite/serializer/json_archive.h>
#include <egnite/serializer/qt_serializer.h>
/* ----------------------------------- Local -------------------------------- */
#include "egnite/rest/export.h"
#include "egnite/rest/global.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

class EGNITE_REST_API DataSerializer : public QObject {
  Q_OBJECT

 public:
  enum class Format { Json, Cbor };

 public:
  explicit DataSerializer(QObject* parent = nullptr);
  ~DataSerializer() override;

  template <typename Object>
  Data serialize(const Object& object, Format format) const;

  template <typename Object>
  Object deserialize(const Data& data) const;
};

template <typename Object>
Data DataSerializer::serialize(const Object& object, Format format) const {
  auto data =
      (format == Format::Json) ? Data(QJsonValue{}) : Data(QCborValue{});

  std::visit(
      core::utils::overloaded{[&](std::nullopt_t) {},
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
Object DataSerializer::deserialize(const Data& data) const {
  return std::visit(core::utils::overloaded{
                        [&](std::nullopt_t) -> Object { return Object{}; },
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
                    const_cast<Data&>(data));
}

}  // namespace egnite::rest

#endif  // EGNITE_REST_DATA_SERIALIZER_H