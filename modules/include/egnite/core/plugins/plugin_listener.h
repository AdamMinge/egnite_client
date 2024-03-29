#ifndef EGNITE_CORE_PLUGINS_PLUGIN_LISTENER_H
#define EGNITE_CORE_PLUGINS_PLUGIN_LISTENER_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QObject>
/* ----------------------------------- Local -------------------------------- */
#include "egnite/core/export.h"
#include "egnite/core/plugins/plugin_manager.h"
/* -------------------------------------------------------------------------- */

namespace egnite::core::plugins {

template <typename OBJECT>
class EGNITE_CORE_API PluginListener {
 public:
  explicit PluginListener();
  virtual ~PluginListener();

 protected:
  virtual void onObjectAdded(OBJECT *object);
  virtual void onObjectRemoved(OBJECT *object);

  [[nodiscard]] QList<OBJECT *> getObjects() const;

 private:
  QMetaObject::Connection added_object_conn;
  QMetaObject::Connection removed_object_conn;
};

template <typename OBJECT>
PluginListener<OBJECT>::PluginListener() {
  added_object_conn = QObject::connect(
      &PluginManager::getInstance(), &PluginManager::objectAdded,
      [this](auto current_object) {
        if (auto object = qobject_cast<OBJECT *>(current_object); object)
          this->onObjectAdded(object);
      });

  removed_object_conn = QObject::connect(
      &PluginManager::getInstance(), &PluginManager::objectRemoved,
      [this](auto current_object) {
        if (auto object = qobject_cast<OBJECT *>(current_object); object)
          this->onObjectRemoved(object);
      });
}

template <typename OBJECT>
PluginListener<OBJECT>::~PluginListener() {
  QObject::disconnect(added_object_conn);
  QObject::disconnect(removed_object_conn);
}

template <typename OBJECT>
QList<OBJECT *> PluginListener<OBJECT>::getObjects() const {
  return PluginManager::getInstance().getObjects<OBJECT>();
}

template <typename OBJECT>
void PluginListener<OBJECT>::onObjectAdded(OBJECT *object) {}

template <typename OBJECT>
void PluginListener<OBJECT>::onObjectRemoved(OBJECT *object) {}

}  // namespace egnite::core::plugins

#endif  // EGNITE_CORE_PLUGINS_PLUGIN_LISTENER_H