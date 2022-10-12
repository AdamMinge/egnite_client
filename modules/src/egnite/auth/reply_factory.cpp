/* ----------------------------------- Local -------------------------------- */
#include "egnite/auth/reply_factory.h"
/* -------------------------------------------------------------------------- */

namespace egnite::auth {

/* ----------------------- JwtAuthenticatorReplyFactory --------------------- */

JwtAuthenticatorReplyFactory::JwtAuthenticatorReplyFactory(
    IJwtAuthenticator* authenticator, QObject* parent)
    : ParentReplyFactory(authenticator, parent) {}

}  // namespace egnite::auth
