/* ----------------------------------- Local -------------------------------- */
#include "egnite/quick_plugin.h"

#include "egnite/quick_egnite.h"
/* ------------------------------------ Qml --------------------------------- */
#include <QFontDatabase>
#include <QQmlEngine>
/* -------------------------------------------------------------------------- */

namespace egnite {

void QuickEgnitePlugin::registerTypes(const char *uri) {
  Q_UNUSED(uri);
  QuickEgnite::getInstance().registerTypes();
}

void QuickEgnitePlugin::initializeEngine(QQmlEngine *engine, const char *uri) {
  Q_UNUSED(uri);
  QuickEgnite::getInstance().initializeEngine(engine);
}

}  // namespace egnite
