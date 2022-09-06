/* ----------------------------------- Local -------------------------------- */
#include "client.h"
/* -------------------------------------------------------------------------- */

QmlClient::QmlClient(QObject* parent) : egnite::rest::Client(parent) {}

QmlClient::~QmlClient() = default;

QQmlListProperty<QmlApi> QmlClient::getApiList() { return {this, &m_api_list}; }
