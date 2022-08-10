/* ----------------------------------- Local -------------------------------- */
#include "egnite/rest/rest_api.h"

#include "egnite/rest/rest_client.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

const QByteArray RestApi::GetVerb = QByteArray{"GET"};
const QByteArray RestApi::PostVerb = QByteArray{"POST"};
const QByteArray RestApi::PutVerb = QByteArray{"PUT"};
const QByteArray RestApi::PatchVerb = QByteArray{"PATCH"};
const QByteArray RestApi::DeleteVerb = QByteArray{"DELETE"};
const QByteArray RestApi::HeadVerb = QByteArray{"HEAD"};

RestApi::RestApi(RestClient* client, const QString& subpath)
    : m_client(client), m_subpath(subpath) {}

RestApi::~RestApi() = default;

}  // namespace egnite::rest