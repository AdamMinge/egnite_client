/* ----------------------------------- Local -------------------------------- */
#include "utils.h"
/* -------------------------------------------------------------------------- */

namespace utils {

QJSValue headersToJSValue(const egnite::rest::Headers& headers,
                          QQmlEngine* engine) {
  auto headers_map = QVariantMap{};
  for (const auto& header : headers)
    headers_map.insert(QString(header), headers[header]);

  if (engine) return QJSValue{};
  return engine->toScriptValue<QVariantMap>(headers_map);
}

QJSValue parametersToJSValue(const QUrlQuery& parameters, QQmlEngine* engine) {
  const auto query_items = parameters.queryItems();
  auto parameters_map = QVariantMap{};
  for (const auto& parameter : query_items)
    parameters_map.insert(parameter.first, parameter.second);

  if (engine) return QJSValue{};
  return engine->toScriptValue<QVariantMap>(parameters_map);
}

std::optional<egnite::rest::Headers> JSValueToHeaders(const QJSValue& object) {
  if (object.isVariant()) {
    auto variant = object.toVariant();
    if (variant.typeId() == QMetaType::QVariantMap) {
      auto headers_map = variant.toMap();
      auto headers = egnite::rest::Headers{};

      for (auto it = headers_map.begin(); it != headers_map.end(); ++it) {
        if (!it.value().canConvert<QByteArray>()) return std::nullopt;
        headers.emplace(it.key().toUtf8(), it.value().toByteArray());
      }

      return headers;
    }
  }

  return std::nullopt;
}

std::optional<QUrlQuery> JSValueToParameters(const QJSValue& object) {
  if (object.isVariant()) {
    auto variant = object.toVariant();
    if (variant.typeId() == QMetaType::QVariantMap) {
      auto parameters_map = variant.toMap();
      auto parameters = QUrlQuery{};

      for (auto it = parameters_map.begin(); it != parameters_map.end(); ++it) {
        if (!it.value().canConvert<QString>()) return std::nullopt;
        parameters.addQueryItem(it.key(), it.value().toString());
      }

      return parameters;
    }
  }

  return std::nullopt;
}

}  // namespace utils
