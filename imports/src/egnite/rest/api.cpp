/* ----------------------------------- Local -------------------------------- */
#include "api.h"

#include "reply.h"
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
  auto opt_headers = getHeaders(object);
  if (opt_headers) m_api->setGlobalHeaders(opt_headers.value());
}

void QmlApi::setGlobalQmlParameters(QJSValue object) {
  auto opt_parameters = getParameters(object);
  if (opt_parameters) m_api->setGlobalParameters(opt_parameters.value());
}

void QmlApi::classBegin() {}

void QmlApi::componentComplete() {
  m_revaluate_data.init = true;
  revaluateApi();
}

QmlReply* QmlApi::get(QJSValue path, QJSValue parameters,
                      QJSValue headers) const {
  return callImpl(egnite::rest::IApi::GetVerb, path, parameters, headers);
}

QmlReply* QmlApi::head(QJSValue path, QJSValue parameters,
                       QJSValue headers) const {
  return callImpl(egnite::rest::IApi::HeadVerb, path, parameters, headers);
}

QmlReply* QmlApi::deleteResource(QJSValue path, QJSValue parameters,
                                 QJSValue headers) const {
  return callImpl(egnite::rest::IApi::DeleteVerb, path, parameters, headers);
}

QmlReply* QmlApi::post(QJSValue path, QJSValue data, QJSValue parameters,
                       QJSValue headers) const {
  return callImpl(egnite::rest::IApi::PostVerb, path, parameters, headers,
                  data);
}

QmlReply* QmlApi::put(QJSValue path, QJSValue data, QJSValue parameters,
                      QJSValue headers) const {
  return callImpl(egnite::rest::IApi::PutVerb, path, parameters, headers, data);
}

QmlReply* QmlApi::patch(QJSValue path, QJSValue data, QJSValue parameters,
                        QJSValue headers) const {
  return callImpl(egnite::rest::IApi::PatchVerb, path, parameters, headers,
                  data);
}

void QmlApi::revaluateApi() {
  if (!m_revaluate_data.init) return;
  if (m_api) m_api->deleteLater();

  if (!m_revaluate_data.client)
    qmlWarning(this) << "client property must be set";
  else
    m_api = m_revaluate_data.client->createApi(m_revaluate_data.path, this);
}

std::optional<QString> QmlApi::getPath(const QJSValue& object) const {
  if (object.isString())
    return object.toString();
  else if (object.isUndefined())
    return std::nullopt;

  qmlWarning(this)
      << "Unsupported parameter configuration: Argument is not a path.";
  return std::nullopt;
}

QmlReply* QmlApi::createQmlReply(egnite::rest::IReply* reply) const {
  auto qml_reply = new QmlReply(QQmlEngine::contextForObject(this)->engine(),
                                reply, nullptr);
  QQmlEngine::setObjectOwnership(qml_reply, QQmlEngine::JavaScriptOwnership);
  return qml_reply;
}

QmlReply* QmlApi::callImpl(const QByteArray& verb, const QJSValue& path,
                           const QJSValue& parameters, const QJSValue& headers,
                           const QJSValue& data) const {
  const auto opt_path = getPath(path);
  const auto opt_parameters = getParameters(parameters);
  const auto opt_headers = getHeaders(headers);
  const auto opt_body = getBody(data);

  if (opt_body) {
    return createQmlReply(
        std::visit(egnite::core::utils::overloaded{
                       [&](std::nullopt_t) -> egnite::rest::IReply* {
                         return m_api->callRaw(
                             verb, opt_path.value_or(""),
                             opt_parameters.value_or(QUrlQuery{}),
                             opt_headers.value_or(egnite::rest::Headers{}));
                       },
                       [&](const auto& body) -> egnite::rest::IReply* {
                         return m_api->callRaw(
                             verb, opt_path.value_or(""), body,
                             opt_parameters.value_or(QUrlQuery{}),
                             opt_headers.value_or(egnite::rest::Headers{}));
                       }},
                   *opt_body));
  } else {
    return createQmlReply(m_api->callRaw(
        verb, opt_path.value_or(""), opt_parameters.value_or(QUrlQuery{}),
        opt_headers.value_or(egnite::rest::Headers{})));
  }
}

std::optional<egnite::rest::Data> QmlApi::getBody(
    const QJSValue& object) const {
  if (object.isObject() || object.isArray()) {
    switch (m_api->getRequestDataFormat()) {
      using enum egnite::rest::DataSerializer::Format;

      case Json:
        return QJsonValue::fromVariant(object.toVariant());
      case Cbor:
        return QCborValue::fromVariant(object.toVariant());
    }
  } else if (object.isUndefined())
    return std::nullopt;

  qmlWarning(this) << "Unsupported parameter configuration: Argument is not a "
                      "object or array.";
  return std::nullopt;
}

std::optional<QUrlQuery> QmlApi::getParameters(const QJSValue& object) const {
  if (object.isVariant()) {
    auto variant = object.toVariant();
    if (variant.typeId() == QMetaType::QVariantMap) {
      auto parameters_map = variant.toMap();
      auto parameters = QUrlQuery{};

      for (auto it = parameters_map.begin(); it != parameters_map.end(); ++it) {
        if (!it.value().canConvert<QString>()) {
          qmlWarning(this) << "unsupported value in headers object, it must be "
                              "convertable to string";
          continue;
        }

        parameters.addQueryItem(it.key(), it.value().toString());
      }

      return parameters;
    }
  } else if (object.isUndefined())
    return std::nullopt;

  qmlWarning(this)
      << "Unsupported parameter configuration: Argument is not a parameters.";
  return std::nullopt;
}

std::optional<egnite::rest::Headers> QmlApi::getHeaders(
    const QJSValue& object) const {
  if (object.isVariant()) {
    auto variant = object.toVariant();
    if (variant.typeId() == QMetaType::QVariantMap) {
      auto headers_map = variant.toMap();
      auto headers = egnite::rest::Headers{};

      for (auto it = headers_map.begin(); it != headers_map.end(); ++it) {
        if (!it.value().canConvert<QByteArray>()) {
          qmlWarning(this) << "unsupported value in headers object, it must be "
                              "convertable to bytes";
          continue;
        }

        headers.emplace(it.key().toUtf8(), it.value().toByteArray());
      }

      return headers;
    }
  } else if (object.isUndefined())
    return std::nullopt;

  qmlWarning(this)
      << "Unsupported parameter configuration: Argument is not a headers.";
  return std::nullopt;
}
