/* ----------------------------------- Local -------------------------------- */
#include "egnite/rest/rest_reply.h"

#include "egnite/rest/private/rest_reply_p.h"
/* -------------------------------------------------------------------------- */

using namespace std::chrono_literals;

namespace egnite::rest {

/* -------------------------------- RestReply ------------------------------- */

RestReply::RestReply(QNetworkReply* network_reply, QObject* parent)
    : RestReply(*new RestReplyPrivate(network_reply, -1ms), parent) {}

RestReply::RestReply(RestReplyPrivate& impl, QObject* parent)
    : QObject(impl, parent) {}

RestReply::~RestReply() {
  Q_D(RestReply);
  if (d->network_reply) d->network_reply->deleteLater();
}

void RestReply::abort() {
  Q_D(RestReply);
  d->network_reply->abort();
}

void RestReply::retry() {
  Q_D(RestReply);
  d->retry_delay = 0ms;
}

void RestReply::retry(std::chrono::milliseconds msecs) {
  Q_D(RestReply);
  d->retry_delay = msecs;
}

/* ---------------------------- RestReplyPrivate ---------------------------- */

RestReplyPrivate::RestReplyPrivate(QNetworkReply* network_reply,
                                   const std::chrono::milliseconds& retry_delay)
    : network_reply(network_reply), retry_delay(retry_delay) {}

}  // namespace egnite::rest
