/* ----------------------------------- Local -------------------------------- */
#include "client.h"
/* -------------------------------------------------------------------------- */

QmlClient::QmlClient(QObject *parent)
    : egnite::rest::Client(parent),
      m_reply_decorator(new QmlReplyDecorator(
          egnite::rest::Client::getReplyDecorator(), this)) {
  connect(this, &egnite::rest::Client::versionChanged, this,
          [this](auto &version) { Q_EMIT versionChanged(version.toString()); });
}

QmlClient::~QmlClient() = default;

void QmlClient::setVersion(const QString &version) {
  auto version_number = QVersionNumber::fromString(version);
  Q_ASSERT(!version_number.isNull());
  egnite::rest::Client::setVersion(QVersionNumber::fromString(version));
}

QString QmlClient::getVersion() const {
  return egnite::rest::Client::getVersion().toString();
}

QmlReplyDecorator *QmlClient::getReplyDecorator() const {
  return m_reply_decorator;
}
