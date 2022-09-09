/* ----------------------------------- Local -------------------------------- */
#include "egnite/auth/authenticator.h"
/* -------------------------------------------------------------------------- */

namespace egnite::auth {

Authenticator::Authenticator(QObject* parent) : QObject(parent) {}

Authenticator::Authenticator(QObjectPrivate& impl, QObject* parent)
    : QObject(impl, parent) {}

Authenticator::~Authenticator() = default;

}  // namespace egnite::auth
