/* ----------------------------------- Local -------------------------------- */
#include "egnite/rest/reply.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

Reply::Reply(QObject* parent) : QObject(parent) {}

Reply::Reply(QObjectPrivate& impl, QObject* parent) : QObject(impl, parent) {}

Reply::~Reply() = default;

}  // namespace egnite::rest
