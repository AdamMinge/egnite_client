/* ----------------------------------- Local -------------------------------- */
#include "egnite/core/plugins/plugin_manager.h"

#include "egnite/core/plugins/detail/plugin_manager_p.h"
/* -------------------------------------------------------------------------- */

namespace egnite::core::plugins {

/* ------------------------------ PluginManager ----------------------------- */

QScopedPointer<PluginManager> PluginManager::m_instance =
    QScopedPointer<PluginManager>(nullptr);

PluginManager &PluginManager::getInstance() {
  if (!m_instance) m_instance.reset(new PluginManager);
  return *m_instance;
}

PluginManager::PluginManager()
    : PluginManager(*new detail::PluginManagerPrivate()) {}

PluginManager::PluginManager(detail::PluginManagerPrivate &impl)
    : QObject(impl) {}

PluginManager::~PluginManager() = default;

void PluginManager::addObject(QObject *object) {
  Q_D(detail::PluginManager);
  return d->addObject(object);
}

void PluginManager::removeObject(QObject *object) {
  Q_D(detail::PluginManager);
  return d->addObject(object);
}

QObjectList PluginManager::getObjects() const {
  Q_D(const detail::PluginManager);
  return d->getObjects();
}

/* -------------------------- PluginManagerPrivate -------------------------- */

namespace detail {

PluginManagerPrivate::PluginManagerPrivate() = default;

PluginManagerPrivate::~PluginManagerPrivate() = default;

void PluginManagerPrivate::addObject(QObject *object) {
  Q_Q(PluginManager);
  m_objects.append(object);
  Q_EMIT q->objectAdded(object);
}

void PluginManagerPrivate::removeObject(QObject *object) {
  Q_Q(PluginManager);
  m_objects.removeOne(object);
  Q_EMIT q->objectRemoved(object);
}

QObjectList PluginManagerPrivate::getObjects() const { return m_objects; }

}  // namespace detail

}  // namespace egnite::core::plugins
