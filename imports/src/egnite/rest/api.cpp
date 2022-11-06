/* ----------------------------------- Local -------------------------------- */
#include "api.h"
/* ----------------------------------- Egnite ------------------------------- */
#include <egnite/rest/api.h>
/* -------------------------------------------------------------------------- */

/* ----------------------------------- QmlApi ------------------------------- */

QmlApi::QmlApi(QObject* parent) : QObject(parent), m_api(nullptr) {}

QmlApi::~QmlApi() = default;

void QmlApi::setPath(const QString& path) {
  if (m_revaluate_data.path == path) return;

  m_revaluate_data.path = path;
  revaluateApi();

  Q_EMIT pathChanged(m_revaluate_data.path);
}

QString QmlApi::getPath() const { return m_revaluate_data.path; }

void QmlApi::setClient(QmlClient* client) {
  if (m_revaluate_data.client == client) return;

  m_revaluate_data.client = client;
  revaluateApi();

  Q_EMIT clientChanged(client);
}

QmlClient* QmlApi::getClient() const { return m_revaluate_data.client; }

QJSValue QmlApi::getGlobalQmlHeaders() const {
  const auto headers = m_api->getGlobalHeaders();
  auto headers_map = QVariantMap{};

  for (const auto& header : headers)
    headers_map.insert(QString(header), headers[header]);

  auto engine = QQmlEngine::contextForObject(this)->engine();
  if (engine) {
    qmlWarning(this) << "engine from context is nullptr";
    return QJSValue{};
  }

  return engine->toScriptValue<QVariantMap>(headers_map);
}

QJSValue QmlApi::getGlobalQmlParameters() const {
  const auto parameters = m_api->getGlobalParameters().queryItems();
  auto parameters_map = QVariantMap{};

  for (const auto& parameter : parameters)
    parameters_map.insert(parameter.first, parameter.second);

  auto engine = QQmlEngine::contextForObject(this)->engine();
  if (engine) {
    qmlWarning(this) << "engine from context is nullptr";
    return QJSValue{};
  }

  return engine->toScriptValue<QVariantMap>(parameters_map);
}

void QmlApi::setGlobalQmlHeaders(QJSValue object) {
  if (object.isObject() || object.isUndefined()) {
    auto headers_map = object.toVariant().toMap();
    auto headers = egnite::rest::Headers{};

    for (auto it = headers_map.begin(); it != headers_map.end(); ++it) {
      if (!it.value().canConvert<QByteArray>()) {
        qmlWarning(this) << "unsupported value in headers object, it must be "
                            "convertable to bytes";
        continue;
      }

      headers.emplace(it.key().toUtf8(), it.value().toByteArray());
    }

    m_api->setGlobalHeaders(headers);

  } else {
    qmlWarning(this) << "unsupported parameter, it must be headers object";
  }
}

void QmlApi::setGlobalQmlParameters(QJSValue object) {
  if (object.isObject() || object.isUndefined()) {
    auto parameters_map = object.toVariant().toMap();
    auto parameters = QUrlQuery{};

    for (auto it = parameters_map.begin(); it != parameters_map.end(); ++it) {
      if (!it.value().canConvert<QString>()) {
        qmlWarning(this) << "unsupported value in headers object, it must be "
                            "convertable to string";
        continue;
      }

      parameters.addQueryItem(it.key(), it.value().toString());
    }

    m_api->setGlobalParameters(parameters);

  } else {
    qmlWarning(this) << "unsupported parameter, it must be parameters object";
  }
}

void QmlApi::classBegin() {}

void QmlApi::componentComplete() {
  m_revaluate_data.init = true;
  revaluateApi();
}

void QmlApi::revaluateApi() {
  if (!m_revaluate_data.init) return;
  if (m_api) m_api->deleteLater();

  if (!m_revaluate_data.client)
    qmlWarning(this) << "client property must be set";
  else
    m_api = m_revaluate_data.client->createApi(m_revaluate_data.path, this);
}
