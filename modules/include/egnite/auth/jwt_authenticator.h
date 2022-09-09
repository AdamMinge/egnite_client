#ifndef EGNITE_AUTH_JWT_AUTHENTICATOR_H
#define EGNITE_AUTH_JWT_AUTHENTICATOR_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QByteArray>
#include <QObject>
#include <QUrlQuery>
/* --------------------------------- Standard ------------------------------- */
#include <type_traits>
/* ----------------------------------- Egnite ------------------------------- */
#include <egnite/rest/reply.h>
/* ----------------------------------- Local -------------------------------- */
#include "egnite/auth/authenticator.h"
/* -------------------------------------------------------------------------- */

namespace egnite::auth {

namespace detail {
class JwtAuthenticatorPrivate;
class JwtAuthenticatorReplyPrivate;
}  // namespace detail

class EGNITE_AUTH_API JwtAuthenticator : public Authenticator {
  Q_OBJECT

 public:
  Q_PROPERTY(rest::Headers headers READ getHeaders WRITE setHeaders NOTIFY
                 headersChanged)
  Q_PROPERTY(QUrlQuery parameters READ getParameters WRITE setParameters NOTIFY
                 parametersChanged)

  Q_PROPERTY(
      QByteArray accessToken READ getAccessToken NOTIFY accessTokenChanged)
  Q_PROPERTY(
      QByteArray refreshToken READ getRefreshToken NOTIFY refreshTokenChanged)

 public:
  explicit JwtAuthenticator(rest::Client* client, const QString& subpath,
                            QObject* parent = nullptr);
  ~JwtAuthenticator() override;

  void login(const QString& username, const QString& password);
  void logout();

  [[nodiscard]] rest::Client* getClient() const override;

  void setHeaders(const rest::Headers& headers);
  [[nodiscard]] rest::Headers getHeaders() const;

  void setParameters(const QUrlQuery& parameters);
  [[nodiscard]] QUrlQuery getParameters() const;

  [[nodiscard]] QByteArray getAccessToken() const;
  [[nodiscard]] QByteArray getRefreshToken() const;

 Q_SIGNALS:
  void headersChanged(const egnite::rest::Headers& headers);
  void parametersChanged(const QUrlQuery& parameters);

  void accessTokenChanged(const QByteArray& token);
  void refreshTokenChanged(const QByteArray& token);

 protected:
  explicit JwtAuthenticator(detail::JwtAuthenticatorPrivate& impl,
                            QObject* parent = nullptr);

 private:
  Q_DECLARE_PRIVATE(detail::JwtAuthenticator)
};

class EGNITE_AUTH_API JwtAuthenticatorReply : public rest::Reply {
  Q_OBJECT

 public:
  JwtAuthenticatorReply(JwtAuthenticator* authenticator, rest::Reply* reply,
                        QObject* parent = nullptr);
  ~JwtAuthenticatorReply() override;

  void abort() override;
  void retry() override;

  [[nodiscard]] rest::Api* getApi() const override;
  [[nodiscard]] rest::Client* getClient() const override;
  [[nodiscard]] rest::DataSerializer* getDataSerializer() const override;

 protected:
  explicit JwtAuthenticatorReply(detail::JwtAuthenticatorReplyPrivate& impl,
                                 QObject* parent = nullptr);

 private:
  Q_DECLARE_PRIVATE(detail::JwtAuthenticatorReply)
};

}  // namespace egnite::auth

#endif  // EGNITE_AUTH_JWT_AUTHENTICATOR_H