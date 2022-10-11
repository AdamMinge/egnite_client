#ifndef EGNITE_QML_REST_JWT_AUTHENTICATOR_REPLY_H
#define EGNITE_QML_REST_JWT_AUTHENTICATOR_REPLY_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QQmlParserStatus>
/* --------------------------------- Egnite Qml ----------------------------- */
#include <reply_factory.h>
/* ------------------------------------ Local ------------------------------- */
#include "jwt_authenticator.h"
/* -------------------------------------------------------------------------- */

namespace egnite::auth {
class JwtAuthenticatorReplyFactory;
}

class QmlJwtAuthenticatorReplyFactory : public QmlReplyFactory,
                                        public QQmlParserStatus {
  Q_OBJECT
  QML_ELEMENT

  Q_PROPERTY(QmlJwtAuthenticator* authenticator READ getAuthenticator WRITE
                 setAuthenticator NOTIFY authenticatorChanged)

 public:
  explicit QmlJwtAuthenticatorReplyFactory(QObject* parent = nullptr);
  ~QmlJwtAuthenticatorReplyFactory() override;

  void classBegin() override;
  void componentComplete() override;

  void setAuthenticator(QmlJwtAuthenticator* authenticator);
  [[nodiscard]] QmlJwtAuthenticator* getAuthenticator() const;

  egnite::rest::Reply* create(egnite::rest::Reply* reply) override;

 Q_SIGNALS:
  void authenticatorChanged(QmlJwtAuthenticator* authenticator);

 private:
  void revaluateFactory();

 private:
  bool m_init;
  QmlJwtAuthenticator* m_authenticator;
  egnite::auth::JwtAuthenticatorReplyFactory* m_factory;
};

#endif  // EGNITE_QML_REST_JWT_AUTHENTICATOR_REPLY_H
