#ifndef EGNITE_AUTH_AUTHENTICATOR_P_H
#define EGNITE_AUTH_AUTHENTICATOR_P_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QtCore/private/qobject_p.h>
/* --------------------------------- Standard ------------------------------- */
#include <memory>
/* ------------------------------------ Local ------------------------------- */
#include "egnite/auth/authenticator.h"
/* -------------------------------------------------------------------------- */

namespace egnite {

namespace rest {
class IApi;
}

namespace auth::detail {

/* -------------------------- JwtAuthenticatorPrivate ----------------------- */

class JwtAuthenticatorPrivate : public QObjectPrivate {
 public:
  Q_DECLARE_PUBLIC(JwtAuthenticator)

 public:
  static const QByteArray TokenHeader;
  static const QByteArray TokenPrefix;

 public:
  explicit JwtAuthenticatorPrivate(rest::IClient* client, const QString& path);
  ~JwtAuthenticatorPrivate() override;

  void login(const QString& username, const QString& password);
  void refresh();
  void logout();

  [[nodiscard]] rest::IClient* getClient() const;

  [[nodiscard]] QByteArray getAccessToken() const;
  [[nodiscard]] QByteArray getRefreshToken() const;

  void setRouting(const JwtAuthenticator::Routing& routing);
  [[nodiscard]] JwtAuthenticator::Routing getRouting() const;

 private:
  void updateAccessToken(const QByteArray& token);
  void updateRefreshToken(const QByteArray& token);

 private:
  std::unique_ptr<rest::IApi> m_api;
  JwtAuthenticator::Routing m_routing;
  QByteArray m_access_token;
  QByteArray m_refresh_token;
};

}  // namespace auth::detail

}  // namespace egnite

#endif  // EGNITE_AUTH_AUTHENTICATOR_P_H