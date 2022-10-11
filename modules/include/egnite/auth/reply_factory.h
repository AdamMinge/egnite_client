#ifndef EGNITE_AUTH_REPLY_FACTORY_H
#define EGNITE_AUTH_REPLY_FACTORY_H

/* ----------------------------------- Egnite ------------------------------- */
#include <egnite/rest/reply_factory.h>
/* ----------------------------------- Local -------------------------------- */
#include "egnite/auth/export.h"
#include "egnite/auth/reply.h"
/* -------------------------------------------------------------------------- */

namespace egnite::auth {

class JwtAuthenticator;

class JwtAuthenticatorReplyFactory
    : public rest::ReplyFactoryWithArgs<JwtAuthenticatorReply,
                                        JwtAuthenticator*> {
  using ParentReplyFactory =
      rest::ReplyFactoryWithArgs<JwtAuthenticatorReply, JwtAuthenticator*>;

 public:
  explicit JwtAuthenticatorReplyFactory(JwtAuthenticator* authenticator,
                                        QObject* parent = nullptr);
};

}  // namespace egnite::auth

#endif  // EGNITE_AUTH_REPLY_FACTORY_H