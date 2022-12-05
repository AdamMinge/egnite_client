/* ----------------------------------- Local -------------------------------- */
#include "client.h"

#include "utils.h"
/* -------------------------------------------------------------------------- */

/* --------------------------------- QmlClient ------------------------------ */

QmlClient::QmlClient(QObject *parent)
    : egnite::rest::Client(parent),
      m_reply_decorator(new QmlReplyDecorator(
          egnite::rest::Client::getReplyDecorator(), this)) {
  connect(this, &egnite::rest::Client::versionChanged, this,
          [this](const auto &version) {
            Q_EMIT versionChanged(version.toString());
          });
}

QmlClient::~QmlClient() = default;

QmlReplyDecorator *QmlClient::getReplyDecorator() const {
  return m_reply_decorator;
}

void QmlClient::setStrVersion(const QString &version) {
  auto version_number = QVersionNumber::fromString(version);
  if (version_number.isNull())
    qmlWarning(this) << QLatin1String(
                            "version can't be create from string '%1'")
                            .arg(version);
  else
    setVersion(version_number);
}

QString QmlClient::getStrVersion() const {
  return egnite::rest::Client::getVersion().toString();
}

QJSValue QmlClient::getGlobalQmlHeaders() const {
  const auto headers = getGlobalHeaders();
  auto engine = QQmlEngine::contextForObject(this)->engine();
  return utils::headersToJSValue(headers, engine);
}

QJSValue QmlClient::getGlobalQmlParameters() const {
  const auto parameters = getGlobalParameters();
  auto engine = QQmlEngine::contextForObject(this)->engine();
  return utils::parametersToJSValue(parameters, engine);
}

void QmlClient::setGlobalQmlHeaders(QJSValue object) {
  auto opt_headers = utils::JSValueToHeaders(object);
  if (opt_headers)
    setGlobalHeaders(opt_headers.value());
  else
    qmlWarning(this) << "unsupported parameter, it must be headers object";
}

void QmlClient::setGlobalQmlParameters(QJSValue object) {
  auto opt_parameters = utils::JSValueToParameters(object);
  if (opt_parameters)
    setGlobalParameters(opt_parameters.value());
  else
    qmlWarning(this) << "unsupported parameter, it must be parameters object";
}
