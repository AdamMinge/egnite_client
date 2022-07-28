#ifndef EGNITE_WEB_ENUMS_H
#define EGNITE_WEB_ENUMS_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QNetworkReply>
/* -------------------------------------------------------------------------- */

namespace egnite::web {

namespace network_error {
Q_NAMESPACE
Q_ENUM_NS(QNetworkReply::NetworkError)
} // namespace network_error

namespace ssl_error {
Q_NAMESPACE
Q_ENUM_NS(QSslError::SslError)
} // namespace ssl_error

} // namespace egnite::web

#endif // EGNITE_WEB_ENUMS_H
