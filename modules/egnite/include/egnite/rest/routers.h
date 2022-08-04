#ifndef EGNITE_REST_ROUTERS_H
#define EGNITE_REST_ROUTERS_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QObject>
#include <QUrl>
/* ----------------------------------- Local -------------------------------- */
#include "egnite/export.h"
#include "egnite/macros/map.h"
/* -------------------------------------------------------------------------- */

#define EGNITE_ROUTING_PROPERTY(path) \
  Q_PROPERTY(QUrl path READ get##path WRITE set##path NOTIFY path##Changed)

#define EGNITE_ROUTING_PROPERTY_READ(path) \
  const QUrl &get##path() const { return m_##path; }

#define EGNITE_ROUTING_PROPERTY_WRITE(path) \
  void set##path(const QUrl &value) {       \
    if (m_##path == value) return;          \
                                            \
    m_##path = value;                       \
    Q_EMIT path##Changed(m_##path);         \
  }

#define EGNITE_ROUTING_PROPERTY_NOTIFY(path) \
  Q_SIGNAL void path##Changed(const QUrl &value);

#define EGNITE_ROUTING_PROPERTY_VARIABLE(path) QUrl m_##path = QUrl{};

#define EGNITE_REGISTER_ROUTING(name, ...)                    \
  class EGNITE_API name : public QObject {                    \
    Q_OBJECT                                                  \
   public:                                                    \
    name(QObject *parent = nullptr) : QObject(parent) {}      \
                                                              \
    EGNITE_MAP(EGNITE_ROUTING_PROPERTY, __VA_ARGS__)          \
                                                              \
    EGNITE_MAP(EGNITE_ROUTING_PROPERTY_READ, __VA_ARGS__)     \
    EGNITE_MAP(EGNITE_ROUTING_PROPERTY_WRITE, __VA_ARGS__)    \
                                                              \
    EGNITE_MAP(EGNITE_ROUTING_PROPERTY_NOTIFY, __VA_ARGS__)   \
                                                              \
   private:                                                   \
    EGNITE_MAP(EGNITE_ROUTING_PROPERTY_VARIABLE, __VA_ARGS__) \
  };

#define EGNITE_ROUTING(name)                                                \
 public:                                                                    \
  Q_PROPERTY(                                                               \
      name *routing READ getRouting WRITE setRouting NOTIFY routingChanged) \
  [[nodiscard]] name *getRouting() const { return m_routing; }              \
  void setRouting(name *routing) {                                          \
    if (m_routing == routing) return;                                       \
                                                                            \
    m_routing = routing;                                                    \
    Q_EMIT routingChanged(m_routing);                                       \
  }                                                                         \
 Q_SIGNALS:                                                                 \
  void routingChanged(name *routing);                                       \
                                                                            \
 private:                                                                   \
  name *m_routing;

namespace egnite::rest {

EGNITE_REGISTER_ROUTING(SimpleJWTAuthenticatorRouting, tokenCreate,
                        tokenRefresh, tokenBlacklist)

}  // namespace egnite::rest

#endif  // EGNITE_REST_ROUTERS_H
