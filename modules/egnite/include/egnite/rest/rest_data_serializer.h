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
#include "egnite/utils/binder.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

class EGNITE_API RestDataSerializer : public QObject {
  Q_OBJECT

 public:
  RestDataSerializer(QObject* parent = nullptr);
  ~RestDataSerializer() override;

  template <typename Object>
  Object deserialize(const RestData& data) const;
};

template <typename Object>
Object RestDataSerializer::deserialize(const RestData& data) const {
  return std::visit(
      utils::overloaded{
          [&](std::nullopt_t) -> Object { return Object{}; },
          [&](const QJsonValue& data) -> Object { return Object{}; },
          [&](const QCborValue& data) -> Object { return Object{}; }},
      data);
}

}  // namespace egnite::rest

#endif  // EGNITE_REST_DATA_SERIALIZER_H