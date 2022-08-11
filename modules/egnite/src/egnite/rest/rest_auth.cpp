/* ----------------------------------- Local -------------------------------- */
#include "egnite/rest/rest_auth.h"

#include "egnite/rest/private/rest_auth_p.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

/* -------------------------------- RestAuth -------------------------------- */

RestAuth::RestAuth(RestAuthPrivate& impl, QObject* parent)
    : QObject(impl, parent) {}

/* ----------------------------- RestAuthPrivate ---------------------------- */

RestAuthPrivate::RestAuthPrivate() {}

}  // namespace egnite::rest

#include "egnite/rest/moc_rest_auth.cpp"
