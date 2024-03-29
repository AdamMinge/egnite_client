/* ----------------------------------- Local -------------------------------- */
#include "api.h"

#include "reply.h"
#include "utils.h"
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
  auto engine = QQmlEngine::contextForObject(this)->engine();
  return utils::headersToJSValue(m_revaluate_data.headers, engine);
}

QJSValue QmlApi::getGlobalQmlParameters() const {
  auto engine = QQmlEngine::contextForObject(this)->engine();
  return utils::parametersToJSValue(m_revaluate_data.parameters, engine);
}

void QmlApi::setGlobalQmlHeaders(QJSValue object) {
  setGlobalHeaders(getHeaders(object).value_or(egnite::rest::Headers{}));
}

void QmlApi::setGlobalQmlParameters(QJSValue object) {
  setGlobalParameters(getParameters(object).value_or(QUrlQuery{}));
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
  else {
    m_api = m_revaluate_data.client->createApi(m_revaluate_data.path, this);
    m_api->setGlobalParameters(m_revaluate_data.parameters);
    m_api->setGlobalHeaders(m_revaluate_data.headers);
  }
}

bool QmlApi::isEvaluated() const {
  if (m_api) return true;

  qmlWarning(this) << "components wasn't evaluate correctly or not at all";
  return false;
}

QmlReply* QmlApi::createQmlReply(egnite::rest::IReply* reply) const {
  auto qml_reply =
      new QmlReply(QQmlEngine::contextForObject(this)->engine(), reply);
  QQmlEngine::setObjectOwnership(qml_reply, QQmlEngine::JavaScriptOwnership);
  return qml_reply;
}

QmlReply* QmlApi::callImpl(const QByteArray& verb, const QJSValue& path,
                           const QJSValue& parameters, const QJSValue& headers,
                           const QJSValue& data) const {
  if (!isEvaluated()) return nullptr;

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

void QmlApi::setGlobalHeaders(const egnite::rest::Headers& headers) {
  m_revaluate_data.headers = headers;
  if (m_api) m_api->setGlobalHeaders(m_revaluate_data.headers);
}

void QmlApi::setGlobalParameters(const QUrlQuery& parameters) {
  m_revaluate_data.parameters = parameters;
  if (m_api) m_api->setGlobalParameters(m_revaluate_data.parameters);
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
  auto opt_parameters = utils::JSValueToParameters(object);
  if (opt_parameters) return opt_parameters;
  qmlWarning(this) << "unsupported parameter, it must be parameters object";
  return std::nullopt;
}

std::optional<egnite::rest::Headers> QmlApi::getHeaders(
    const QJSValue& object) const {
  auto opt_headers = utils::JSValueToHeaders(object);
  if (opt_headers) return opt_headers;
  qmlWarning(this) << "unsupported parameter, it must be headers object";
  return std::nullopt;
}

template <typename Type>
QString QmlApi::QJSValueToPath(const QJSValue& path) {
  return QString("%1").arg(path.toVariant().value<Type>());
}

template <>
QString QmlApi::QJSValueToPath<QUuid>(const QJSValue& path) {
  return QString("%1").arg(
      path.toVariant().value<QUuid>().toString(QUuid::WithoutBraces));
}
