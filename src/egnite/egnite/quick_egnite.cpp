/* ----------------------------------- Local -------------------------------- */
#include "egnite/egnite/quick_egnite.h"
#include "egnite/egnite/web/authenticator.h"
#include "egnite/egnite/web/client.h"
#include "egnite/egnite/web/enums.h"
#include "egnite/egnite/web/headers.h"
#include "egnite/egnite/web/serializer.h"
/* -------------------------------------------------------------------------- */

void initResources() { Q_INIT_RESOURCE(egnite); }

namespace egnite {

QScopedPointer<QuickEgnite> QuickEgnite::m_instance = QScopedPointer<QuickEgnite>(nullptr);

QuickEgnite &QuickEgnite::getInstance() {
  if (m_instance.isNull())
    m_instance.reset(new QuickEgnite);

  return *m_instance;
}

void QuickEgnite::deleteInstance() { m_instance.reset(nullptr); }

QuickEgnite::QuickEgnite() = default;

QuickEgnite::~QuickEgnite() = default;

void QuickEgnite::registerTypes() {
  initResources(); // Q_INIT_RESOURCE must be outside the namespace

  // register web submodule
  qmlRegisterType<web::Client>("Egnite.Web", 1, 0, "Client");
  qmlRegisterType<web::Headers>("Egnite.Web", 1, 0, "Headers");

  qmlRegisterType<web::Serializer>("Egnite.Web", 1, 0, "Serializer");
  qmlRegisterType<web::JsonSerializer>("Egnite.Web", 1, 0, "JsonSerializer");

  qmlRegisterType<web::Authenticator>("Egnite.Web", 1, 0, "Authenticator");
  qmlRegisterType<web::SimpleJWTAuthenticator>("Egnite.Web", 1, 0, "SimpleJWTAuthenticator");
  qmlRegisterType<web::SimpleJWTAuthenticatorRouting>("Egnite.Web", 1, 0,
                                                      "SimpleJWTAuthenticatorRouting");

  qmlRegisterUncreatableMetaObject(egnite::web::network_error::staticMetaObject, "Egnite.Web", 1, 0,
                                   "NetworkError", "NetworkError is not instantiable!");
  qmlRegisterUncreatableMetaObject(egnite::web::ssl_error::staticMetaObject, "Egnite.Web", 1, 0,
                                   "SslError", "SslError is not instantiable!");
}

void QuickEgnite::initializeEngine(QQmlEngine *engine) {}

} // namespace egnite
