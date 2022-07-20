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

/* ----------------------------------- Login ------------------------------ */

struct LoginRequest {
  QString username;
  QString password;
};

struct LoginResponse {
  QByteArray access_token;
  QByteArray refresh_token;
};

/* ----------------------------------- Logout ----------------------------- */

struct LogoutRequest {
  QByteArray refresh_token;
};

/* ----------------------------- Renew Access Token ----------------------- */

struct RenewAccessTokenRequest {
  QByteArray refresh_token;
};

struct RenewAccessTokenResponse {
  QByteArray access_token;
};

} // namespace egnite::web

namespace boost::serialization {

/* ----------------------------------- Login ------------------------------ */

template <class Archive>
void serialize(Archive& ar, egnite::web::LoginRequest& request, const unsigned int version) {
  ar& boost::serialization::make_nvp("username", request.username);
  ar& boost::serialization::make_nvp("password", request.password);
}

template <class Archive>
void serialize(Archive& ar, egnite::web::LoginResponse& request, const unsigned int version) {
  ar& boost::serialization::make_nvp("access_token", request.access_token);
  ar& boost::serialization::make_nvp("refresh_token", request.refresh_token);
}

/* ----------------------------------- Logout ----------------------------- */

template <class Archive>
void serialize(Archive& ar, egnite::web::LogoutRequest& request, const unsigned int version) {
  ar& boost::serialization::make_nvp("refresh_token", request.refresh_token);
}

/* ----------------------------- Renew Access Token ----------------------- */

template <class Archive>
void serialize(Archive& ar, egnite::web::RenewAccessTokenRequest& request,
               const unsigned int version) {
  ar& boost::serialization::make_nvp("refresh_token", request.refresh_token);
}

template <class Archive>
void serialize(Archive& ar, egnite::web::RenewAccessTokenResponse& request,
               const unsigned int version) {
  ar& boost::serialization::make_nvp("access_token", request.access_token);
}

} // namespace boost::serialization

#endif // EGNITE_WEB_MESSAGES_H
