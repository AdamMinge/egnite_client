#ifndef EGNITE_WEB_SERIALIZER_H
#define EGNITE_WEB_SERIALIZER_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QObject>
/* ----------------------------------- Local -------------------------------- */
#include "egnite/egnite/export.h"
/* -------------------------------------------------------------------------- */

namespace egnite::web {

class EGNITE_API WebSerializer : public QObject {
  Q_OBJECT
public:
  explicit WebSerializer();
  ~WebSerializer();
};

class EGNITE_API JsonSerializer : public WebSerializer {
  Q_OBJECT
public:
  explicit JsonSerializer();
  ~JsonSerializer();
};

} // namespace egnite::web

#endif // EGNITE_WEB_SERIALIZER_H
