#ifndef EGNITE_REST_GLOBAL_H
#define EGNITE_REST_GLOBAL_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QByteArray>
#include <QCborValue>
#include <QHash>
#include <QJsonValue>
/* ---------------------------------- Standard ------------------------------ */
#include <type_traits>
#include <variant>
/* ----------------------------------- Local -------------------------------- */
#include "egnite/rest/export.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

using Headers = QHash<QByteArray, QByteArray>;
using Data = std::variant<std::nullopt_t, QJsonValue, QCborValue>;

}  // namespace egnite::rest

#endif  // EGNITE_REST_GLOBAL_H