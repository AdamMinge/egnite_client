/* ----------------------------------- Local -------------------------------- */
#include "egnite/rest/rest_api.h"

#include "egnite/rest/private/rest_api_p.h"
#include "egnite/rest/rest_client.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

/* -------------------------------- RestApi --------------------------------- */

RestApi::RestApi(RestClient* client, const QString& subpath, QObject* parent)
    : RestApi(*new RestApiPrivate(client, subpath), parent) {}

RestApi::RestApi(RestApiPrivate& impl, QObject* parent)
    : QObject(impl, parent) {}

RestApi::~RestApi() = default;

/* ----------------------------- RestApiPrivate ----------------------------- */

const QByteArray RestApiPrivate::GetVerb = QByteArray{"GET"};
const QByteArray RestApiPrivate::PostVerb = QByteArray{"POST"};
const QByteArray RestApiPrivate::PutVerb = QByteArray{"PUT"};
const QByteArray RestApiPrivate::PatchVerb = QByteArray{"PATCH"};
const QByteArray RestApiPrivate::DeleteVerb = QByteArray{"DELETE"};
const QByteArray RestApiPrivate::HeadVerb = QByteArray{"HEAD"};

RestApiPrivate::RestApiPrivate(RestClient* client, const QString& subpath)
    : client(client), subpath(subpath) {}

}  // namespace egnite::rest

#include "egnite/rest/moc_rest_api.cpp"