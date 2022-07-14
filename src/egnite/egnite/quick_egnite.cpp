/* ----------------------------------- Local -------------------------------- */
#include "egnite/egnite/quick_egnite.h"
#include "egnite/egnite/web/web_authenticator.h"
#include "egnite/egnite/web/web_client.h"
#include "egnite/egnite/web/web_headers.h"
#include "egnite/egnite/web/web_serializer.h"
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

  qmlRegisterType<web::WebClient>("egnite", 1, 0, "WebClient");
  qmlRegisterType<web::WebHeaders>("egnite", 1, 0, "WebHeaders");

  qmlRegisterType<web::WebSerializer>("egnite", 1, 0, "WebSerializer");
  qmlRegisterType<web::JsonSerializer>("egnite", 1, 0, "JsonSerializer");

  qmlRegisterType<web::WebAuthenticator>("egnite", 1, 0, "WebAuthenticator");
  qmlRegisterType<web::SimpleJWTAuthenticator>("egnite", 1, 0, "SimpleJWTAuthenticator");
  qmlRegisterType<web::SimpleJWTAuthenticatorRouting>("egnite", 1, 0,
                                                      "SimpleJWTAuthenticatorRouting");
}

void QuickEgnite::initializeEngine(QQmlEngine *engine) {}

} // namespace egnite
