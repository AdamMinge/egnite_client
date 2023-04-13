#ifndef EGNITE_CORE_PLUGINS_PLUGIN_INTERFACE_H
#define EGNITE_CORE_PLUGINS_PLUGIN_INTERFACE_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QObject>
/* -------------------------------------------------------------------------- */

namespace egnite::core::plugins {

namespace detail {
class PluginInterfacePrivate;
};

class PluginInterface : public QObject {
 public:
  explicit PluginInterface();
  ~PluginInterface() override;

  virtual void init();

 protected:
  explicit PluginInterface(detail::PluginInterfacePrivate& impl);

  void addObject(QObject* object);
  void removeObject(QObject* object);

 private:
  Q_DECLARE_PRIVATE(detail::PluginInterface)
};

}  // namespace egnite::core::plugins

#endif  // EGNITE_CORE_PLUGINS_PLUGIN_INTERFACE_H