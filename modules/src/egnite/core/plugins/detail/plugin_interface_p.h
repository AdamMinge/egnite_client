#ifndef EGNITE_CORE_PLUGINS_PLUGIN_INTERFACE_P_H
#define EGNITE_CORE_PLUGINS_PLUGIN_INTERFACE_P_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QtCore/private/qobject_p.h>
/* ----------------------------------- Local -------------------------------- */
#include "egnite/core/plugins/plugin_interface.h"
/* -------------------------------------------------------------------------- */

namespace egnite::core::plugins::detail {

class PluginInterfacePrivate : public QObjectPrivate {
 public:
  Q_DECLARE_PUBLIC(PluginInterface)

 public:
  explicit PluginInterfacePrivate();
  ~PluginInterfacePrivate() override;

  void addObject(QObject* object);
  void removeObject(QObject* object);

 private:
  QObjectList m_added_objects;
};

}  // namespace egnite::core::plugins::detail

#endif  // EGNITE_CORE_PLUGINS_PLUGIN_INTERFACE_P_H
