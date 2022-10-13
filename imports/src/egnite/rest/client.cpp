/* ----------------------------------- Local -------------------------------- */
#include "client.h"
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
  auto headers_map = QVariantMap{};

  for (const auto &header : headers)
    headers_map.insert(QString(header), headers[header]);

  auto engine = QQmlEngine::contextForObject(this)->engine();
  if (engine) {
    qmlWarning(this) << "engine from context is nullptr";
    return QJSValue{};
  }

  return engine->toScriptValue<QVariantMap>(headers_map);
}

QJSValue QmlClient::getGlobalQmlParameters() const {
  const auto parameters = getGlobalParameters().queryItems();
  auto parameters_map = QVariantMap{};

  for (const auto &parameter : parameters)
    parameters_map.insert(parameter.first, parameter.second);

  auto engine = QQmlEngine::contextForObject(this)->engine();
  if (engine) {
    qmlWarning(this) << "engine from context is nullptr";
    return QJSValue{};
  }

  return engine->toScriptValue<QVariantMap>(parameters_map);
}

void QmlClient::setGlobalQmlHeaders(QJSValue object) {
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

    setGlobalHeaders(headers);

  } else {
    qmlWarning(this) << "unsupported parameter, it must be headers object";
  }
}

void QmlClient::setGlobalQmlParameters(QJSValue object) {
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

    setGlobalParameters(parameters);

  } else {
    qmlWarning(this) << "unsupported parameter, it must be parameters object";
  }
}
