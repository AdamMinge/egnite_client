#ifndef EGNITE_WEB_CLIENT_H
#define EGNITE_WEB_CLIENT_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QObject>
#include <QUrl>
/* ----------------------------------- Local -------------------------------- */
#include "egnite/egnite/export.h"
/* -------------------------------------------------------------------------- */

namespace egnite::web {

class WebAuthenticator;
class WebAuthorizator;

class EGNITE_API WebClient : public QObject {
  Q_OBJECT

public:
  Q_PROPERTY(
      QUrl baseUrl READ getBaseUrl WRITE setBaseUrl NOTIFY baseUrlChanged)
  Q_PROPERTY(WebAuthenticator *authenticator READ getAuthenticator WRITE
                 setAuthenticator NOTIFY authenticatorChanged)
  Q_PROPERTY(WebAuthorizator *authorizator READ getAuthorizator WRITE
                 setAuthorizator NOTIFY authorizatorChanged)

public:
  explicit WebClient(QObject *parent = nullptr);
  ~WebClient() override;

  Q_INVOKABLE void login(const QString &username, const QString &password);
  Q_INVOKABLE void logout();

  [[nodiscard]] QUrl getBaseUrl() const;
  void setBaseUrl(const QUrl &base_url);

  [[nodiscard]] WebAuthenticator *getAuthenticator() const;
  void setAuthenticator(WebAuthenticator *authenticator);

  [[nodiscard]] WebAuthorizator *getAuthorizator() const;
  void setAuthorizator(WebAuthorizator *authorizator);

Q_SIGNALS:
  void baseUrlChanged(const QUrl &base_url);
  void authenticatorChanged(egnite::web::WebAuthenticator *authenticator);
  void authorizatorChanged(egnite::web::WebAuthorizator *authorizator);

private:
  QUrl m_base_url;
  QNetworkAccessManager m_network_access_manager;
  WebAuthenticator *m_authenticator;
  WebAuthorizator *m_authorizator;
};

} // namespace egnite::web

#endif // EGNITE_WEB_CLIENT_H
