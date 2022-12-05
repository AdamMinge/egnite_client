#ifndef EGNITE_QML_REST_UTILS_H
#define EGNITE_QML_REST_UTILS_H

/* ------------------------------------- Qt --------------------------------- */
#include <QJSValue>
#include <QQmlEngine>
#include <QUrlQuery>
/* ---------------------------------- standard ------------------------------ */
#include <optional>
/* ----------------------------------- Egnite ------------------------------- */
#include <egnite/rest/global.h>
/* -------------------------------------------------------------------------- */

namespace utils {

[[nodiscard]] QJSValue headersToJSValue(const egnite::rest::Headers& headers,
                                        QQmlEngine* engine);
[[nodiscard]] QJSValue parametersToJSValue(const QUrlQuery& headers,
                                           QQmlEngine* engine);

[[nodiscard]] std::optional<egnite::rest::Headers> JSValueToHeaders(
    const QJSValue& object);
[[nodiscard]] std::optional<QUrlQuery> JSValueToParameters(
    const QJSValue& object);

}  // namespace utils

#endif  // EGNITE_QML_REST_UTILS_H