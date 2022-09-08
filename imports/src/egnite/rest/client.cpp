/* ----------------------------------- Local -------------------------------- */
#include "client.h"
/* -------------------------------------------------------------------------- */

QmlClient::QmlClient(QObject* parent) : egnite::rest::Client(parent) {
  connect(this, &egnite::rest::Client::versionChanged, this,
          [this](auto& version) { Q_EMIT versionChanged(version.toString()); });
}

QmlClient::~QmlClient() = default;

QQmlListProperty<QmlApi> QmlClient::getApiList() { return {this, &m_api_list}; }

void QmlClient::setVersion(const QString& version) {
  auto version_number = QVersionNumber::fromString(version);
  Q_ASSERT(!version_number.isNull());
  egnite::rest::Client::setVersion(QVersionNumber::fromString(version));
}

QString QmlClient::getVersion() const {
  return egnite::rest::Client::getVersion().toString();
}
