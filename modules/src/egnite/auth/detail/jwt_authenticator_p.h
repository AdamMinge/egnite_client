#ifndef EGNITE_AUTH_JWT_AUTHENTICATOR_P_H
#define EGNITE_AUTH_JWT_AUTHENTICATOR_P_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QtCore/private/qobject_p.h>
/* --------------------------------- Standard ------------------------------- */
#include <memory>
/* ----------------------------------- Egnite ------------------------------- */
#include <egnite/rest/reply_decorator_manager.h>
/* ------------------------------------ Local ------------------------------- */
#include "egnite/auth/jwt_authenticator.h"
/* -------------------------------------------------------------------------- */

namespace egnite {

namespace rest {
class Api;
}

namespace auth::detail {

class JwtAuthenticatorPrivate : public QObjectPrivate,
                                public rest::ReplyDecorator {
 public:
  Q_DECLARE_PUBLIC(JwtAuthenticator)

 public:
  explicit JwtAuthenticatorPrivate(rest::Client* client, const QString& path);
  ~JwtAuthenticatorPrivate() override;

  void login(const QString& username, const QString& password);
  void logout();

  [[nodiscard]] rest::Client* getClient() const;

  void setHeaders(const rest::Headers& headers);
  [[nodiscard]] rest::Headers getHeaders() const;

  void setParameters(const QUrlQuery& parameters);
  [[nodiscard]] QUrlQuery getParameters() const;

  [[nodiscard]] QByteArray getAccessToken() const;
  [[nodiscard]] QByteArray getRefreshToken() const;

  [[nodiscard]] rest::Reply* decorate(rest::Reply* reply) const override;

 private:
  std::unique_ptr<rest::Api> m_api;
  rest::Headers m_headers;
  QUrlQuery m_parameters;
  QByteArray m_access_token;
  QByteArray m_refresh_token;
};

class JwtAuthenticatorReplyPrivate : public QObjectPrivate {
  Q_DECLARE_PUBLIC(JwtAuthenticatorReply)

 public:
  JwtAuthenticatorReplyPrivate(JwtAuthenticator* authenticator,
                               rest::Reply* reply);

  void abort();
  void retry();

  [[nodiscard]] rest::Api* getApi() const;
  [[nodiscard]] rest::Client* getClient() const;
  [[nodiscard]] rest::DataSerializer* getDataSerializer() const;

 private:
  JwtAuthenticator* m_authenticator;
  rest::Reply* m_reply;
};

}  // namespace auth::detail

}  // namespace egnite

#endif  // EGNITE_AUTH_JWT_AUTHENTICATOR_P_H