#ifndef EGNITE_REST_ENUMS_H
#define EGNITE_REST_ENUMS_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QNetworkReply>
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

namespace network_error {
Q_NAMESPACE
Q_ENUM_NS(QNetworkReply::NetworkError)
}  // namespace network_error

namespace ssl_error {
Q_NAMESPACE
Q_ENUM_NS(QSslError::SslError)
}  // namespace ssl_error

}  // namespace egnite::rest

#endif  // EGNITE_REST_ENUMS_H
