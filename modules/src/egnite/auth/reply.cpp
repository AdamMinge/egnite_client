/* ----------------------------------- Local -------------------------------- */
#include "egnite/auth/reply.h"

#include "egnite/auth/authenticator.h"
/* -------------------------------------------------------------------------- */

namespace egnite::auth {

/* --------------------------- JwtAuthenticatorReply ------------------------ */

JwtAuthenticatorReply::JwtAuthenticatorReply(IJwtAuthenticator* authenticator,
                                             rest::IReply* reply,
                                             QObject* parent)
    : rest::WrappedReply(reply, parent) {
  disconnect(reply, &rest::IReply::failed, this, &rest::IReply::failed);
  reply->onFailed([this, authenticator](int code, const rest::Data& data) {
    if (code == 401) {
      authenticator
          ->onRefreshFailed([this, code, data]() { Q_EMIT failed(code, data); })
          ->onRefreshSucceeded([this]() { retry(); });
      authenticator->refresh();
    } else {
      Q_EMIT failed(code, data);
    }
  });
}

JwtAuthenticatorReply::~JwtAuthenticatorReply() = default;

}  // namespace egnite::auth
