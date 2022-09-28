#ifndef EGNITE_AUTH_AUTHENTICATOR_H
#define EGNITE_AUTH_AUTHENTICATOR_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QObject>
#include <QString>
#include <QUrlQuery>
/* ------------------------------------ Boost ------------------------------- */
#include <boost/serialization/nvp.hpp>
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
  struct ErrorResponse;

 public:
  ~Authenticator() override;

  [[nodiscard]] virtual rest::Client* getClient() const = 0;

 protected:
  explicit Authenticator(QObject* parent = nullptr);
  explicit Authenticator(QObjectPrivate& impl, QObject* parent = nullptr);
};

struct EGNITE_AUTH_API Authenticator::ErrorResponse {
  template <class Archive>
  void serialize(Archive& ar, const unsigned int version) {
    ar& BOOST_NVP(detail);
  }

  QString detail;
};

}  // namespace auth

}  // namespace egnite

#endif  // EGNITE_AUTH_AUTHENTICATOR_H