#ifndef EGNITE_REST_API_H
#define EGNITE_REST_API_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QObject>
/* ----------------------------------- Local -------------------------------- */
#include "egnite/export.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

class RestApiPrivate;
class RestClient;

class EGNITE_API RestApi : public QObject {
  Q_OBJECT

  friend RestClient;

 public:
  ~RestApi() override;

 protected:
  RestApi(RestClient* client, const QString& subpath,
          QObject* parent = nullptr);
  RestApi(RestApiPrivate& impl, QObject* parent = nullptr);

 private:
  Q_DECLARE_PRIVATE(RestApi);
};

}  // namespace egnite::rest

#endif  // EGNITE_REST_API_H