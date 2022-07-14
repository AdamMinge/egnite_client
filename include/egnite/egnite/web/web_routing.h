#ifndef EGNITE_WEB_ROUTING_H
#define EGNITE_WEB_ROUTING_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QObject>
/* ----------------------------------- Local -------------------------------- */
#include "egnite/egnite/export.h"
#include "egnite/egnite/macros/map.h"
/* -------------------------------------------------------------------------- */

#define EGNITE_ROUTING_PROPERTY(path)                                                              \
  Q_PROPERTY(QString path READ get##path WRITE set##path NOTIFY on##path##Changed)

#define EGNITE_ROUTING_PROPERTY_READ(path)                                                         \
  const QString& get##path() const { return m_##path; }

#define EGNITE_ROUTING_PROPERTY_WRITE(path)                                                        \
  void set##path(const QString& value) {                                                           \
    if (m_##path == value)                                                                         \
      return;                                                                                      \
                                                                                                   \
    m_##path = value;                                                                              \
    Q_EMIT on##path##Changed(m_##path);                                                            \
  }

#define EGNITE_ROUTING_PROPERTY_NOTIFY(path) Q_SIGNAL void on##path##Changed(const QString& value);

#define EGNITE_ROUTING_PROPERTY_VARIABLE(path) QString m_##path = QLatin1String("");

#define EGNITE_ROUTING(name, ...)                                                                  \
  class EGNITE_API name : public QObject {                                                         \
    Q_OBJECT                                                                                       \
  public:                                                                                          \
    name(QObject* parent = nullptr) : QObject(parent) {}                                           \
                                                                                                   \
    EGNITE_MAP(EGNITE_ROUTING_PROPERTY, __VA_ARGS__)                                               \
                                                                                                   \
    EGNITE_MAP(EGNITE_ROUTING_PROPERTY_READ, __VA_ARGS__)                                          \
    EGNITE_MAP(EGNITE_ROUTING_PROPERTY_WRITE, __VA_ARGS__)                                         \
                                                                                                   \
    EGNITE_MAP(EGNITE_ROUTING_PROPERTY_NOTIFY, __VA_ARGS__)                                        \
                                                                                                   \
  private:                                                                                         \
    EGNITE_MAP(EGNITE_ROUTING_PROPERTY_VARIABLE, __VA_ARGS__)                                      \
  };

#endif // EGNITE_WEB_ROUTING_H
