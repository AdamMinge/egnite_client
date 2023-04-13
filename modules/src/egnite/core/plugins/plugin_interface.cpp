/* ----------------------------------- Local -------------------------------- */
#include "egnite/core/plugins/plugin_interface.h"

#include "egnite/core/plugins/detail/plugin_interface_p.h"
#include "egnite/core/plugins/plugin_manager.h"
/* -------------------------------------------------------------------------- */

namespace egnite::core::plugins {

/* ----------------------------- PluginInterface ---------------------------- */

PluginInterface::PluginInterface()
    : PluginInterface(*new detail::PluginInterfacePrivate) {}

PluginInterface::PluginInterface(detail::PluginInterfacePrivate& impl)
    : QObject(impl) {}

PluginInterface::~PluginInterface() = default;

void PluginInterface::init() {}

void PluginInterface::addObject(QObject* object) {
  Q_D(detail::PluginInterface);
  return d->addObject(object);
}

void PluginInterface::removeObject(QObject* object) {
  Q_D(detail::PluginInterface);
  return d->removeObject(object);
}

/* ------------------------- PluginInterfacePrivate ------------------------- */

namespace detail {

PluginInterfacePrivate::PluginInterfacePrivate() = default;

PluginInterfacePrivate::~PluginInterfacePrivate() {
  for (auto object : m_added_objects) removeObject(object);
}

void PluginInterfacePrivate::addObject(QObject* object) {
  PluginManager::getInstance().addObject(object);
  m_added_objects.append(object);
}

void PluginInterfacePrivate::removeObject(QObject* object) {
  PluginManager::getInstance().removeObject(object);
  m_added_objects.removeOne(object);
}

}  // namespace detail

}  // namespace egnite::core::plugins
