/* ----------------------------------- Local -------------------------------- */
#include "egnite/rest/rest_api.h"

#include "egnite/rest/private/rest_api_p.h"
#include "egnite/rest/rest_client.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

/* -------------------------------- RestApi --------------------------------- */

const QByteArray RestApi::GetVerb = QByteArray{"GET"};
const QByteArray RestApi::PostVerb = QByteArray{"POST"};
const QByteArray RestApi::PutVerb = QByteArray{"PUT"};
const QByteArray RestApi::PatchVerb = QByteArray{"PATCH"};
const QByteArray RestApi::DeleteVerb = QByteArray{"DELETE"};
const QByteArray RestApi::HeadVerb = QByteArray{"HEAD"};

RestApi::RestApi(RestClient* client, const QString& subpath, QObject* parent)
    : RestApi(*new RestApiPrivate(client, subpath), parent) {}

RestApi::RestApi(RestApiPrivate& impl, QObject* parent)
    : QObject(impl, parent) {}

RestApi::~RestApi() = default;

/* ----------------------------- RestApiPrivate ----------------------------- */

RestApiPrivate::RestApiPrivate(RestClient* client, const QString& subpath)
    : client(client), subpath(subpath) {}

}  // namespace egnite::rest