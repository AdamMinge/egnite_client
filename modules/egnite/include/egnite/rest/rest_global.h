#ifndef EGNITE_REST_GLOBAL_H
#define EGNITE_REST_GLOBAL_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QByteArray>
#include <QCborValue>
#include <QHash>
#include <QJsonValue>
/* ---------------------------------- Standard ------------------------------ */
#include <variant>
/* ----------------------------------- Local -------------------------------- */
#include "egnite/export.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

using RestHeaders = QHash<QByteArray, QByteArray>;
using RestData = std::variant<std::nullopt_t, QJsonValue, QCborValue>;

}  // namespace egnite::rest

#endif  // EGNITE_REST_GLOBAL_H