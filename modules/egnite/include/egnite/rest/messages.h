#ifndef EGNITE_REST_MESSAGES_H
#define EGNITE_REST_MESSAGES_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QByteArray>
#include <QNetworkReply>
#include <QString>
/* ----------------------------------- Boost -------------------------------- */
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/string.hpp>
/* ----------------------------------- Local -------------------------------- */
#include "egnite/archive/serializer/qt.h"
#include "egnite/archive/serializer/stl.h"
#include "egnite/export.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

/* ----------------------------------- Login ------------------------------ */

struct LoginRequest {
  QString username;
  QString password;

  std::strong_ordering operator<=>(const LoginRequest& other) const;
  bool operator==(const LoginRequest& other) const = default;
};

struct LoginResponse {
  QByteArray access_token;
  QByteArray refresh_token;

  std::strong_ordering operator<=>(const LoginResponse& other) const;
  bool operator==(const LoginResponse& other) const = default;
};

/* ----------------------------------- Logout ----------------------------- */

struct LogoutRequest {
  QByteArray refresh_token;

  std::strong_ordering operator<=>(const LogoutRequest& other) const;
  bool operator==(const LogoutRequest& other) const = default;
};

/* ----------------------------- Renew Access Token ----------------------- */

struct RenewAccessTokenRequest {
  QByteArray refresh_token;

  std::strong_ordering operator<=>(const RenewAccessTokenRequest& other) const;
  bool operator==(const RenewAccessTokenRequest& other) const = default;
};

struct RenewAccessTokenResponse {
  QByteArray access_token;

  std::strong_ordering operator<=>(const RenewAccessTokenResponse& other) const;
  bool operator==(const RenewAccessTokenResponse& other) const = default;
};

}  // namespace egnite::rest

namespace boost::serialization {

/* ----------------------------------- Login ------------------------------ */

template <class Archive>
void serialize(Archive& ar, egnite::rest::LoginRequest& request,
               const unsigned int version) {
  ar& boost::serialization::make_nvp("username", request.username);
  ar& boost::serialization::make_nvp("password", request.password);
}

template <class Archive>
void serialize(Archive& ar, egnite::rest::LoginResponse& response,
               const unsigned int version) {
  ar& boost::serialization::make_nvp("access_token", response.access_token);
  ar& boost::serialization::make_nvp("refresh_token", response.refresh_token);
}

/* ----------------------------------- Logout ----------------------------- */

template <class Archive>
void serialize(Archive& ar, egnite::rest::LogoutRequest& request,
               const unsigned int version) {
  ar& boost::serialization::make_nvp("refresh_token", request.refresh_token);
}

/* ----------------------------- Renew Access Token ----------------------- */

template <class Archive>
void serialize(Archive& ar, egnite::rest::RenewAccessTokenRequest& request,
               const unsigned int version) {
  ar& boost::serialization::make_nvp("refresh_token", request.refresh_token);
}

template <class Archive>
void serialize(Archive& ar, egnite::rest::RenewAccessTokenResponse& response,
               const unsigned int version) {
  ar& boost::serialization::make_nvp("access_token", response.access_token);
}

}  // namespace boost::serialization

#endif  // EGNITE_REST_MESSAGES_H
