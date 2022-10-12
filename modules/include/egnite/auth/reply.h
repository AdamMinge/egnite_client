#ifndef EGNITE_AUTH_REPLY_H
#define EGNITE_AUTH_REPLY_H

/* ----------------------------------- Egnite ------------------------------- */
#include <egnite/rest/reply.h>
/* ----------------------------------- Local -------------------------------- */
#include "egnite/auth/export.h"
/* -------------------------------------------------------------------------- */

namespace egnite::auth {

class IJwtAuthenticator;

class EGNITE_AUTH_API JwtAuthenticatorReply : public rest::WrappedReply {
  Q_OBJECT

 public:
  JwtAuthenticatorReply(IJwtAuthenticator* authenticator, rest::IReply* reply,
                        QObject* parent = nullptr);
  ~JwtAuthenticatorReply() override;
};

}  // namespace egnite::auth

#endif  // EGNITE_AUTH_REPLY_H