/* ----------------------------------- Local -------------------------------- */
#include "api.h"
/* ----------------------------------- Egnite ------------------------------- */
#include <egnite/rest/api.h>
/* -------------------------------------------------------------------------- */

QmlApi::QmlApi(QObject* parent)
    : QObject(parent), m_init(false), m_client(nullptr), m_api(nullptr) {}

QmlApi::~QmlApi() = default;

void QmlApi::classBegin() {}

void QmlApi::componentComplete() {
  m_init = true;
  revaluateApi();
}

void QmlApi::setPath(const QString& path) {
  if (m_path == path) return;

  m_path = path;
  revaluateApi();

  Q_EMIT pathChanged(m_path);
}

QString QmlApi::getPath() const { return m_path; }

void QmlApi::setClient(egnite::rest::Client* client) {
  if (m_client == client) return;

  m_client = client;
  revaluateApi();

  Q_EMIT clientChanged(m_client);
}

egnite::rest::Client* QmlApi::getClient() const { return m_client; }

void QmlApi::revaluateApi() {
  if (!m_init || !m_client) return;
  if (m_api) m_api->deleteLater();

  m_api = m_client->createApi(m_path, this);
}
