/* ----------------------------------- Local -------------------------------- */
#include "egnite/rest/global.h"
/* ----------------------------------- Egnite ------------------------------- */
#include <egnite/core/macros/binder.h>
/* ------------------------------------- Qt --------------------------------- */
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

QByteArray convertDataToByteArray(const QJsonValue& body) {
  switch (body.type()) {
    case QJsonValue::Array:
      return QJsonDocument(body.toArray()).toJson(QJsonDocument::Compact);
    case QJsonValue::Object:
      return QJsonDocument(body.toObject()).toJson(QJsonDocument::Compact);
    default:
      auto converted =
          QJsonDocument(QJsonArray{body}).toJson(QJsonDocument::Compact);
      converted = converted.mid(1, converted.size() - 2);
      return converted;
  }
}

QByteArray convertDataToByteArray(const QCborValue& body) {
  return body.toCbor();
}

QByteArray convertDataToByteArray(const Data& body) {
  return std::visit(
      core::utils::overloaded{
          [](std::nullopt_t) -> QByteArray { return QByteArray{}; },
          [](const QJsonValue& body) -> QByteArray {
            return convertDataToByteArray(body);
          },
          [](const QCborValue& body) -> QByteArray {
            return convertDataToByteArray(body);
          }},
      body);
}

}  // namespace egnite::rest