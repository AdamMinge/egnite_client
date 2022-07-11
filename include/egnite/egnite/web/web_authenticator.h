#ifndef EGNITE_WEB_AUTHENTICATOR_H
#define EGNITE_WEB_AUTHENTICATOR_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QObject>
/* ----------------------------------- Local -------------------------------- */
#include "egnite/egnite/export.h"
/* -------------------------------------------------------------------------- */

namespace egnite::web
{

  class EGNITE_API WebAuthenticator : public QObject
  {
    Q_OBJECT

  public:
    explicit WebAuthenticator(QObject *parent = nullptr);
    ~WebAuthenticator() override;
  };

  class EGNITE_API SimpleJWTAuthenticator : public WebAuthenticator
  {
    Q_OBJECT

  public:
    Q_PROPERTY(unsigned accessTokenLifetime READ getAccessTokenLifetime WRITE
                 setAccessTokenLifetime NOTIFY accessTokenLifetimeChanged)
    Q_PROPERTY(unsigned refreshTokenLifetime READ getRefreshTokenLifetime WRITE
                 setRefreshTokenLifetime NOTIFY refreshTokenLifetimeChanged)

  public:
    explicit SimpleJWTAuthenticator(QObject *parent = nullptr);
    ~SimpleJWTAuthenticator() override;

    [[nodiscard]] unsigned getAccessTokenLifetime() const;
    void setAccessTokenLifetime(unsigned accessTokenLifetime);

    [[nodiscard]] unsigned getRefreshTokenLifetime() const;
    void setRefreshTokenLifetime(unsigned refreshTokenLifetime);

  Q_SIGNALS:
    void accessTokenLifetimeChanged(unsigned accessTokenLifetime);
    void refreshTokenLifetimeChanged(unsigned refreshTokenLifetime);

  private:
    unsigned m_accessTokenLifetime;
    unsigned m_refreshTokenLifetime;
  };

}// namespace egnite::web

#endif// EGNITE_WEB_AUTHENTICATOR_H
