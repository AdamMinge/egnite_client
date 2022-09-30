#ifndef EGNITE_AUTH_JWT_AUTHENTICATOR_H
#define EGNITE_AUTH_JWT_AUTHENTICATOR_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QObject>
#include <QUrl>
/* ----------------------------------- Egnite ------------------------------- */
#include <egnite/rest/wrapped_reply.h>
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
  struct Routing;
  struct ObtainTokenRequest;
  struct ObtainTokenResponse;
  struct TokenBlacklistRequest;
  struct TokenRefreshRequest;
  struct TokenRefreshResponse;

 public:
  Q_PROPERTY(
      QByteArray accessToken READ getAccessToken NOTIFY accessTokenChanged)
  Q_PROPERTY(
      QByteArray refreshToken READ getRefreshToken NOTIFY refreshTokenChanged)

 public:
  explicit JwtAuthenticator(rest::Client* client, const QString& subpath,
                            QObject* parent = nullptr);
  ~JwtAuthenticator() override;

  void login(const QString& username, const QString& password);
  void refresh();
  void logout();

  [[nodiscard]] rest::Client* getClient() const override;

  [[nodiscard]] QByteArray getAccessToken() const;
  [[nodiscard]] QByteArray getRefreshToken() const;

  void setRouting(const Routing& routing);
  [[nodiscard]] Routing getRouting() const;

  EGNITE_DEFINE_BINDER(JwtAuthenticator, onLoginSucceeded, loginSucceeded);
  EGNITE_DEFINE_BINDER(JwtAuthenticator, onLoginFailed, loginFailed);
  EGNITE_DEFINE_BINDER(JwtAuthenticator, onLogoutSucceeded, logoutSucceeded);
  EGNITE_DEFINE_BINDER(JwtAuthenticator, onLogoutFailed, logoutFailed);
  EGNITE_DEFINE_BINDER(JwtAuthenticator, onRefreshSucceeded, refreshSucceeded);
  EGNITE_DEFINE_BINDER(JwtAuthenticator, onRefreshFailed, refreshFailed);

 Q_SIGNALS:
  void accessTokenChanged(const QByteArray& token);
  void refreshTokenChanged(const QByteArray& token);

  void routingChanged(const Routing& routing);

  void loginSucceeded();
  void logoutSucceeded();
  void refreshSucceeded();

  void loginFailed(const QString& detail);
  void logoutFailed(const QString& detail);
  void refreshFailed(const QString& detail);

 protected:
  explicit JwtAuthenticator(detail::JwtAuthenticatorPrivate& impl,
                            QObject* parent = nullptr);

 private:
  Q_DECLARE_PRIVATE(detail::JwtAuthenticator)
};

struct EGNITE_AUTH_API JwtAuthenticator::Routing {
  auto operator<=>(const Routing&) const = default;

  QString obtain;
  QString refresh;
  QString blacklist;
};

struct EGNITE_AUTH_API JwtAuthenticator::ObtainTokenRequest {
  template <class Archive>
  void serialize(Archive& ar, const unsigned int version) {
    boost::serialization::serialize(ar, *this, version);
    ar& BOOST_NVP(username);
    ar& BOOST_NVP(password);
  }

  QString username;
  QString password;
};

struct EGNITE_AUTH_API JwtAuthenticator::ObtainTokenResponse {
  template <class Archive>
  void serialize(Archive& ar, const unsigned int version) {
    ar& BOOST_NVP(access);
    ar& BOOST_NVP(refresh);
  }

  QByteArray access;
  QByteArray refresh;
};

struct EGNITE_AUTH_API JwtAuthenticator::TokenBlacklistRequest {
  template <class Archive>
  void serialize(Archive& ar, const unsigned int version) {
    ar& BOOST_NVP(refresh);
  }

  QByteArray refresh;
};

struct EGNITE_AUTH_API JwtAuthenticator::TokenRefreshRequest {
  template <class Archive>
  void serialize(Archive& ar, const unsigned int version) {
    ar& BOOST_NVP(refresh);
  }

  QByteArray refresh;
};

struct EGNITE_AUTH_API JwtAuthenticator::TokenRefreshResponse {
  template <class Archive>
  void serialize(Archive& ar, const unsigned int version) {
    ar& BOOST_NVP(access);
  }

  QByteArray access;
};

class EGNITE_AUTH_API JwtAuthenticatorReply : public rest::WrappedReply {
  Q_OBJECT

 public:
  JwtAuthenticatorReply(JwtAuthenticator* authenticator, rest::Reply* reply,
                        QObject* parent = nullptr);
  ~JwtAuthenticatorReply() override;
};

}  // namespace egnite::auth

#endif  // EGNITE_AUTH_JWT_AUTHENTICATOR_H