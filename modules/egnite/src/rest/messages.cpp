/* ----------------------------------- Local -------------------------------- */
#include "egnite/rest/messages.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

std::strong_ordering LoginRequest::operator<=>(
    const LoginRequest& other) const {
  if (username != other.username) {
    return (username < other.username) ? std::strong_ordering::less
                                       : std::strong_ordering::greater;
  }

  if (password != other.password) {
    return (password < other.password) ? std::strong_ordering::less
                                       : std::strong_ordering::greater;
  }

  return std::strong_ordering::equal;
}

std::strong_ordering LoginResponse::operator<=>(
    const LoginResponse& other) const {
  if (access_token != other.access_token) {
    return (access_token < other.access_token) ? std::strong_ordering::less
                                               : std::strong_ordering::greater;
  }

  if (refresh_token != other.refresh_token) {
    return (refresh_token < other.refresh_token)
               ? std::strong_ordering::less
               : std::strong_ordering::greater;
  }

  return std::strong_ordering::equal;
}

std::strong_ordering LogoutRequest::operator<=>(
    const LogoutRequest& other) const {
  if (refresh_token != other.refresh_token) {
    return (refresh_token < other.refresh_token)
               ? std::strong_ordering::less
               : std::strong_ordering::greater;
  }

  return std::strong_ordering::equal;
}

std::strong_ordering RenewAccessTokenRequest::operator<=>(
    const RenewAccessTokenRequest& other) const {
  if (refresh_token != other.refresh_token) {
    return (refresh_token < other.refresh_token)
               ? std::strong_ordering::less
               : std::strong_ordering::greater;
  }

  return std::strong_ordering::equal;
}

std::strong_ordering RenewAccessTokenResponse::operator<=>(
    const RenewAccessTokenResponse& other) const {
  if (access_token != other.access_token) {
    return (access_token < other.access_token) ? std::strong_ordering::less
                                               : std::strong_ordering::greater;
  }

  return std::strong_ordering::equal;
}

}  // namespace egnite::rest
