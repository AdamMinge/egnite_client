#ifndef EGNITE_AUTH_AUTHENTICATOR_H
#define EGNITE_AUTH_AUTHENTICATOR_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QObject>
#include <QString>
#include <QUrl>
#include <QUrlQuery>
/* ----------------------------------- Boost -------------------------------- */
#include <boost/serialization/nvp.hpp>
/* ----------------------------------- Egnite ------------------------------- */
#include <egnite/core/macros/binder.h>
/* ----------------------------------- Local -------------------------------- */
#include "egnite/auth/export.h"
/* -------------------------------------------------------------------------- */

namespace egnite {

namespace rest {
class IClient;
}  // namespace rest

namespace auth {

namespace detail {
class JwtAuthenticatorPrivate;
}  // namespace detail

/* ------------------------------- Authenticator ---------------------------- */

class EGNITE_AUTH_API IAuthenticator : public QObject {
  Q_OBJECT

 public:
  struct ErrorResponse;

 public:
  ~IAuthenticator() override;

  [[nodiscard]] virtual rest::IClient* getClient() const = 0;

 protected:
  explicit IAuthenticator(QObject* parent = nullptr);
  explicit IAuthenticator(QObjectPrivate& impl, QObject* parent = nullptr);
};

struct EGNITE_AUTH_API IAuthenticator::ErrorResponse {
  template <class Archive>
  void serialize(Archive& ar, const unsigned int version) {
    ar& BOOST_NVP(detail);
  }

  QString detail;
};

/* ------------------------------ IJwtAuthenticator ------------------------- */

class EGNITE_AUTH_API IJwtAuthenticator : public IAuthenticator {
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
  ~IJwtAuthenticator() override;

  virtual void login(const QString& username, const QString& password) = 0;
  virtual void refresh() = 0;
  virtual void logout() = 0;

  [[nodiscard]] virtual QByteArray getAccessToken() const = 0;
  [[nodiscard]] virtual QByteArray getRefreshToken() const = 0;

  virtual void setRouting(const Routing& routing) = 0;
  [[nodiscard]] virtual Routing getRouting() const = 0;

  EGNITE_DEFINE_BINDER(IJwtAuthenticator, onLoginSucceeded, loginSucceeded);
  EGNITE_DEFINE_BINDER(IJwtAuthenticator, onLoginFailed, loginFailed);
  EGNITE_DEFINE_BINDER(IJwtAuthenticator, onLogoutSucceeded, logoutSucceeded);
  EGNITE_DEFINE_BINDER(IJwtAuthenticator, onLogoutFailed, logoutFailed);
  EGNITE_DEFINE_BINDER(IJwtAuthenticator, onRefreshSucceeded, refreshSucceeded);
  EGNITE_DEFINE_BINDER(IJwtAuthenticator, onRefreshFailed, refreshFailed);

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
  explicit IJwtAuthenticator(QObject* parent = nullptr);
  explicit IJwtAuthenticator(QObjectPrivate& impl, QObject* parent = nullptr);
};

struct EGNITE_AUTH_API IJwtAuthenticator::Routing {
  Q_GADGET
  Q_PROPERTY(QString obtain MEMBER obtain)
  Q_PROPERTY(QString refresh MEMBER refresh)
  Q_PROPERTY(QString blacklist MEMBER blacklist)

 public:
  Routing() = default;
  auto operator<=>(const Routing&) const = default;

  QString obtain;
  QString refresh;
  QString blacklist;
};

struct EGNITE_AUTH_API IJwtAuthenticator::ObtainTokenRequest {
  Q_GADGET
  Q_PROPERTY(QString username MEMBER username)
  Q_PROPERTY(QString password MEMBER password)

 public:
  QString username;
  QString password;
};

struct EGNITE_AUTH_API IJwtAuthenticator::ObtainTokenResponse {
  Q_GADGET
  Q_PROPERTY(QByteArray access MEMBER access)
  Q_PROPERTY(QByteArray refresh MEMBER refresh)

 public:
  QByteArray access;
  QByteArray refresh;
};

struct EGNITE_AUTH_API IJwtAuthenticator::TokenBlacklistRequest {
  Q_GADGET
  Q_PROPERTY(QByteArray refresh MEMBER refresh)

 public:
  QByteArray refresh;
};

struct EGNITE_AUTH_API IJwtAuthenticator::TokenRefreshRequest {
  Q_GADGET
  Q_PROPERTY(QByteArray refresh MEMBER refresh)

 public:
  QByteArray refresh;
};

struct EGNITE_AUTH_API IJwtAuthenticator::TokenRefreshResponse {
  Q_GADGET
  Q_PROPERTY(QByteArray access MEMBER access)

 public:
  QByteArray access;
};

/* ------------------------------ JwtAuthenticator -------------------------- */

class EGNITE_AUTH_API JwtAuthenticator : public IJwtAuthenticator {
  Q_OBJECT

 public:
  explicit JwtAuthenticator(rest::IClient* client, const QString& subpath,
                            QObject* parent = nullptr);
  ~JwtAuthenticator() override;

  void login(const QString& username, const QString& password) override;
  void refresh() override;
  void logout() override;

  [[nodiscard]] rest::IClient* getClient() const override;

  [[nodiscard]] QByteArray getAccessToken() const override;
  [[nodiscard]] QByteArray getRefreshToken() const override;

  void setRouting(const Routing& routing) override;
  [[nodiscard]] Routing getRouting() const override;

 protected:
  explicit JwtAuthenticator(detail::JwtAuthenticatorPrivate& impl,
                            QObject* parent = nullptr);

 private:
  Q_DECLARE_PRIVATE(detail::JwtAuthenticator)
};

}  // namespace auth

}  // namespace egnite

#endif  // EGNITE_AUTH_AUTHENTICATOR_H