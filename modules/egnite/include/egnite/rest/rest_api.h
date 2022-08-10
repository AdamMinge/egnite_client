#ifndef EGNITE_REST_API_H
#define EGNITE_REST_API_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QObject>
/* ----------------------------------- Local -------------------------------- */
#include "egnite/export.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

class RestClient;

class EGNITE_API RestApi : public QObject {
  Q_OBJECT

  friend RestClient;

 public:
  static const QByteArray GetVerb;
  static const QByteArray PostVerb;
  static const QByteArray PutVerb;
  static const QByteArray PatchVerb;
  static const QByteArray DeleteVerb;
  static const QByteArray HeadVerb;

 public:
  ~RestApi() override;

 protected:
  RestApi(RestClient* client, const QString& subpath);

 private:
  RestClient* m_client;
  QString m_subpath;
};

}  // namespace egnite::rest

#endif  // EGNITE_REST_API_H