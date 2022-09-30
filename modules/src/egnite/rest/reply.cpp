/* ----------------------------------- Local -------------------------------- */
#include "egnite/rest/reply.h"
/* ------------------------------------ Qt ---------------------------------- */
#include <QMetaMethod>
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

Reply::Reply(QObject* parent) : QObject(parent) {}

Reply::Reply(QObjectPrivate& impl, QObject* parent) : QObject(impl, parent) {
  connect(this, &Reply::succeeded, this, &Reply::completed);
  connect(this, &Reply::failed, this, &Reply::completed);
}

Reply::~Reply() = default;

}  // namespace egnite::rest
