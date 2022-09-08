/* ----------------------------------- Local -------------------------------- */
#include "api.h"

#include "client.h"
/* ----------------------------------- Egnite ------------------------------- */
#include <egnite/rest/api.h>
/* -------------------------------------------------------------------------- */

QmlApi::QmlApi(QObject* parent) : QObject(parent), m_init(false) {}

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

void QmlApi::revaluateApi() {
  if (!m_init) return;
  if (m_api) m_api->deleteLater();

  auto client = qobject_cast<QmlClient*>(parent());
  Q_ASSERT(client);

  m_api = client->createApi(m_path, this);
}
