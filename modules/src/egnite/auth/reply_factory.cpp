/* ----------------------------------- Local -------------------------------- */
#include "egnite/auth/reply_factory.h"
/* -------------------------------------------------------------------------- */

namespace egnite::auth {

/* ----------------------- JwtAuthenticatorReplyFactory --------------------- */

JwtAuthenticatorReplyFactory::JwtAuthenticatorReplyFactory(
    JwtAuthenticator* authenticator, QObject* parent)
    : ParentReplyFactory(authenticator, parent) {}

}  // namespace egnite::auth
