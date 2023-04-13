#ifndef EGNITE_CORE_PLUGINS_PLUGIN_MANAGER_P_H
#define EGNITE_CORE_PLUGINS_PLUGIN_MANAGER_P_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QtCore/private/qobject_p.h>
/* ----------------------------------- Local -------------------------------- */
#include "egnite/core/plugins/plugin_manager.h"
/* -------------------------------------------------------------------------- */

namespace egnite::core::plugins::detail {

class PluginManagerPrivate : public QObjectPrivate {
 public:
  Q_DECLARE_PUBLIC(PluginManager)

 public:
  explicit PluginManagerPrivate();
  ~PluginManagerPrivate() override;

  void addObject(QObject *object);
  void removeObject(QObject *object);

  [[nodiscard]] QObjectList getObjects() const;

 private:
  QObjectList m_objects;
};

}  // namespace egnite::core::plugins::detail

#endif  // EGNITE_CORE_PLUGINS_PLUGIN_MANAGER_P_H
