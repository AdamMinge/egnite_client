#ifndef EGNITE_QML_AUTH_REPLY_FACTORY_H
#define EGNITE_QML_AUTH_REPLY_FACTORY_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QQmlParserStatus>
/* ----------------------------------- Egnite ------------------------------- */
#include <egnite/rest/reply_factory.h>
/* ------------------------------------ Local ------------------------------- */
#include "authenticator.h"
/* -------------------------------------------------------------------------- */

namespace egnite::auth {
class JwtAuthenticatorReplyFactory;
}

class QmlJwtAuthenticatorReplyFactory : public egnite::rest::IReplyFactory,
                                        public QQmlParserStatus {
  Q_OBJECT
  QML_ELEMENT

  Q_PROPERTY(QmlJwtAuthenticator* authenticator READ getAuthenticator WRITE
                 setAuthenticator NOTIFY authenticatorChanged)

 public:
  explicit QmlJwtAuthenticatorReplyFactory(QObject* parent = nullptr);
  ~QmlJwtAuthenticatorReplyFactory() override;

  void setAuthenticator(QmlJwtAuthenticator* authenticator);
  [[nodiscard]] QmlJwtAuthenticator* getAuthenticator() const;

  void classBegin() override;
  void componentComplete() override;

  egnite::rest::IReply* create(egnite::rest::IReply* reply) override;

 Q_SIGNALS:
  void authenticatorChanged(QmlJwtAuthenticator* authenticator);

 private:
  void revaluateFactory();

 private:
  struct RevaluateData {
    bool init = false;
    QmlJwtAuthenticator* authenticator = nullptr;
  };

 private:
  RevaluateData m_revaluate_data;
  egnite::auth::JwtAuthenticatorReplyFactory* m_factory;
};

#endif  // EGNITE_QML_AUTH_REPLY_FACTORY_H
