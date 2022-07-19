#ifndef EGNITE_WEB_MESSAGES_H
#define EGNITE_WEB_MESSAGES_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QByteArray>
#include <QString>
/* ----------------------------------- Boost -------------------------------- */
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/string.hpp>
/* ----------------------------------- Utils -------------------------------- */
#include <egnite/utils/qt_boost_serializer/qt_boost_serializer.h>
/* ----------------------------------- Local -------------------------------- */
#include "egnite/egnite/export.h"
/* -------------------------------------------------------------------------- */

namespace egnite::web {

struct LoginRequest {
  QString username;
  QString password;
};

struct LoginResponse {
  QByteArray access_token;
  QByteArray refresh_token;
};

} // namespace egnite::web

namespace boost::serialization {

template <class Archive>
void serialize(Archive& ar, egnite::web::LoginRequest& login_request, const unsigned int version) {
  ar& boost::serialization::make_nvp("username", login_request.username);
  ar& boost::serialization::make_nvp("password", login_request.password);
}

template <class Archive>
void serialize(Archive& ar, egnite::web::LoginResponse& login_response,
               const unsigned int version) {
  ar& boost::serialization::make_nvp("access_token", login_response.access_token);
  ar& boost::serialization::make_nvp("refresh_token", login_response.refresh_token);
}

} // namespace boost::serialization

#endif // EGNITE_WEB_MESSAGES_H
