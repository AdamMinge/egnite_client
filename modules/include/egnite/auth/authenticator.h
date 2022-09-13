#ifndef EGNITE_AUTH_AUTHENTICATOR_H
#define EGNITE_AUTH_AUTHENTICATOR_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QObject>
#include <QUrlQuery>
/* ----------------------------------- Egnite ------------------------------- */
#include <egnite/rest/global.h>
/* ----------------------------------- Local -------------------------------- */
#include "egnite/auth/export.h"
/* -------------------------------------------------------------------------- */

namespace egnite {

namespace rest {
class Client;
}  // namespace rest

namespace auth {

class EGNITE_AUTH_API Authenticator : public QObject {
  Q_OBJECT

 public:
  ~Authenticator() override;

  [[nodiscard]] virtual rest::Client* getClient() const = 0;

  virtual void setHeaders(const rest::Headers& headers) = 0;
  [[nodiscard]] virtual rest::Headers getHeaders() const = 0;

  virtual void setParameters(const QUrlQuery& parameters) = 0;
  [[nodiscard]] virtual QUrlQuery getParameters() const = 0;

 protected:
  explicit Authenticator(QObject* parent = nullptr);
  explicit Authenticator(QObjectPrivate& impl, QObject* parent = nullptr);
};

}  // namespace auth

}  // namespace egnite

#endif  // EGNITE_AUTH_AUTHENTICATOR_H