#ifndef EGNITE_REST_API_P_H
#define EGNITE_REST_API_P_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QtCore/private/qobject_p.h>

#include <QNetworkReply>
/* ------------------------------------ Local ------------------------------- */
#include "egnite/rest/rest_api.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

class RestClient;

namespace detail {

class RestApiPrivate : public QObjectPrivate {
 public:
  Q_DECLARE_PUBLIC(RestApi)

 public:
  static const QByteArray GetVerb;
  static const QByteArray PostVerb;
  static const QByteArray PutVerb;
  static const QByteArray PatchVerb;
  static const QByteArray DeleteVerb;
  static const QByteArray HeadVerb;

 public:
  RestApiPrivate(RestClient* client, QNetworkAccessManager* manager,
                 const QString& path);

  [[nodiscard]] RestClient* getClient() const;
  [[nodiscard]] QString getPath() const;

  QNetworkReply* create(const QByteArray& verb, const QString& path,
                        const QUrlQuery& parameters,
                        const RestHeaders& headers) const;
  QNetworkReply* create(const QByteArray& verb, const QString& path,
                        const QJsonValue& data, const QUrlQuery& parameters,
                        const RestHeaders& headers) const;
  QNetworkReply* create(const QByteArray& verb, const QString& path,
                        const QCborValue& data, const QUrlQuery& parameters,
                        const RestHeaders& headers) const;

 private:
  QByteArray convertData(const QJsonValue& body) const;
  QByteArray convertData(const QCborValue& body) const;

 private:
  RestClient* m_client;
  QNetworkAccessManager* m_manager;
  QString m_path;
};

}  // namespace detail

}  // namespace egnite::rest

#endif  // EGNITE_REST_API_P_H