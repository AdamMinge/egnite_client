/* ----------------------------------- Local -------------------------------- */
#include "egnite/rest/rest_auth.h"

#include "egnite/rest/detail/rest_auth_p.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

/* -------------------------------- RestAuth -------------------------------- */

RestAuth::RestAuth(detail::RestAuthPrivate& impl, QObject* parent)
    : QObject(impl, parent) {}

/* ----------------------------- RestAuthPrivate ---------------------------- */

namespace detail {

RestAuthPrivate::RestAuthPrivate() {}

}  // namespace detail

}  // namespace egnite::rest

#include "egnite/rest/moc_rest_auth.cpp"
