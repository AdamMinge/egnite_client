/* ----------------------------------- Local -------------------------------- */
#include "egnite/egnite/quick_egnite.h"
#include "egnite/egnite/web/web_client.h"
/* -------------------------------------------------------------------------- */

void initResources() { Q_INIT_RESOURCE(egnite); }

namespace egnite
{

  QScopedPointer<QuickEgnite> QuickEgnite::m_instance =
    QScopedPointer<QuickEgnite>(nullptr);

  QuickEgnite &QuickEgnite::getInstance()
  {
    if (m_instance.isNull()) m_instance.reset(new QuickEgnite);

    return *m_instance;
  }

  void QuickEgnite::deleteInstance() { m_instance.reset(nullptr); }

  QuickEgnite::QuickEgnite() = default;

  QuickEgnite::~QuickEgnite() = default;

  void QuickEgnite::registerTypes()
  {
    initResources();// Q_INIT_RESOURCE must be outside the namespace

    qmlRegisterType<web::WebClient>("egnite", 1, 0, "WebClient");
  }

  void QuickEgnite::initializeEngine(QQmlEngine *engine) {}

}// namespace egnite
