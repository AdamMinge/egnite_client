/* ----------------------------------- Local -------------------------------- */
#include "api.h"
/* ----------------------------------- Egnite ------------------------------- */
#include <egnite/rest/api.h>
/* -------------------------------------------------------------------------- */

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

void QmlApi::classBegin() {}

void QmlApi::componentComplete() {
  m_revaluate_data.init = true;
  revaluateApi();
}

void QmlApi::revaluateApi() {
  if (!m_revaluate_data.init) return;
  if (m_api) m_api->deleteLater();

  Q_ASSERT(m_revaluate_data.client);
  m_api = m_revaluate_data.client->createApi(m_revaluate_data.path, this);
}
