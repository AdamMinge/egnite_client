#ifndef EGNITE_QUICK_PLUGIN_H
#define EGNITE_QUICK_PLUGIN_H

/* ------------------------------------ Qml --------------------------------- */
#include <QQmlExtensionPlugin>
/* ----------------------------------- Local -------------------------------- */
#include "egnite/egnite/export.h"
/* -------------------------------------------------------------------------- */

namespace egnite {

class EGNITE_API QuickEgnitePlugin : public QQmlExtensionPlugin {
  Q_OBJECT
  Q_PLUGIN_METADATA(IID QQmlExtensionInterface_iid)

public:
  void registerTypes(const char *uri) override;
  void initializeEngine(QQmlEngine *engine, const char *uri) override;
};

} // namespace egnite

#endif // EGNITE_QUICK_PLUGIN_H
