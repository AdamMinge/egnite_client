/* ----------------------------------- Local -------------------------------- */
#include "egnite/quick_egnite.h"

#include "egnite/rest/authenticator.h"
#include "egnite/rest/client.h"
#include "egnite/rest/enums.h"
#include "egnite/rest/headers.h"
#include "egnite/rest/serializer.h"
/* -------------------------------------------------------------------------- */

void initResources() { Q_INIT_RESOURCE(egnite); }

namespace egnite {

QScopedPointer<QuickEgnite> QuickEgnite::m_instance =
    QScopedPointer<QuickEgnite>(nullptr);

QuickEgnite &QuickEgnite::getInstance() {
  if (m_instance.isNull()) m_instance.reset(new QuickEgnite);

  return *m_instance;
}

void QuickEgnite::deleteInstance() { m_instance.reset(nullptr); }

QuickEgnite::QuickEgnite() = default;

QuickEgnite::~QuickEgnite() = default;

void QuickEgnite::registerTypes() {
  initResources();  // Q_INIT_RESOURCE must be outside the namespace

  registerRestModule();
}

void QuickEgnite::registerRestModule() {
  qmlRegisterType<rest::Client>("Egnite.Rest", 1, 0, "Client");
  qmlRegisterType<rest::Headers>("Egnite.Rest", 1, 0, "Headers");

  qmlRegisterType<rest::Serializer>("Egnite.Rest", 1, 0, "Serializer");
  qmlRegisterType<rest::JsonSerializer>("Egnite.Rest", 1, 0, "JsonSerializer");

  qmlRegisterType<rest::Authenticator>("Egnite.Rest", 1, 0, "Authenticator");
  qmlRegisterType<rest::SimpleJWTAuthenticator>("Egnite.Rest", 1, 0,
                                                "SimpleJWTAuthenticator");
  qmlRegisterType<rest::SimpleJWTAuthenticatorRouting>(
      "Egnite.Rest", 1, 0, "SimpleJWTAuthenticatorRouting");

  qmlRegisterUncreatableMetaObject(
      egnite::rest::network_error::staticMetaObject, "Egnite.Rest", 1, 0,
      "NetworkError", "NetworkError is not instantiable!");
  qmlRegisterUncreatableMetaObject(egnite::rest::ssl_error::staticMetaObject,
                                   "Egnite.Rest", 1, 0, "SslError",
                                   "SslError is not instantiable!");
}

void QuickEgnite::initializeEngine(QQmlEngine *engine) {}

}  // namespace egnite
