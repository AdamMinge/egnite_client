#ifndef EGNITE_CORE_PLUGINS_PLUGIN_MANAGER_H
#define EGNITE_CORE_PLUGINS_PLUGIN_MANAGER_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QObject>
/* ----------------------------------- Local -------------------------------- */
#include "egnite/core/export.h"
/* -------------------------------------------------------------------------- */

namespace egnite::core::plugins {

namespace detail {
class PluginManagerPrivate;
};

class EGNITE_CORE_API PluginManager : public QObject {
 public:
  [[nodiscard]] static PluginManager &getInstance();

 public:
  ~PluginManager() override;

  void addObject(QObject *object);
  void removeObject(QObject *object);

  template <typename TYPE>
  void forEach(const std::function<void(TYPE *)> &function);
  template <typename TYPE>
  TYPE *forOne(const std::function<bool(TYPE *)> &function);
  template <typename TYPE>
  [[nodiscard]] QList<TYPE *> getObjects();

 Q_SIGNALS:
  void objectAdded(QObject *object);
  void objectRemoved(QObject *object);

 protected:
  explicit PluginManager();
  explicit PluginManager(detail::PluginManagerPrivate &impl);

  [[nodiscard]] QObjectList getObjects() const;

 private:
  static QScopedPointer<PluginManager> m_instance;

 private:
  Q_DECLARE_PRIVATE(detail::PluginManager)
};

template <typename TYPE>
void PluginManager::forEach(const std::function<void(TYPE *)> &function) {
  for (auto object : getObjects())
    if (auto result = qobject_cast<TYPE *>(object); result) function(result);
}

template <typename TYPE>
TYPE *PluginManager::forOne(const std::function<bool(TYPE *)> &function) {
  for (auto object : getObjects()) {
    if (auto result = qobject_cast<TYPE *>(object); result) {
      if (function(result)) return result;
    }
  }

  return nullptr;
}

template <typename TYPE>
QList<TYPE *> PluginManager::getObjects() {
  QList<TYPE *> objects{};
  for (auto object : getObjects()) {
    if (auto result = qobject_cast<TYPE *>(object); result)
      objects.append(result);
  }

  return objects;
}

}  // namespace egnite::core::plugins

#endif  // EGNITE_CORE_PLUGINS_PLUGIN_MANAGER_H