/* ----------------------------------- Local -------------------------------- */
#include "egnite/rest/rest_reply.h"
/* -------------------------------------------------------------------------- */

using namespace std::chrono_literals;

namespace egnite::rest {

RestReply::RestReply(QNetworkReply* reply, QObject* parent)
    : QObject(parent), m_network_reply(reply), m_retry_delay(-1ms) {}

RestReply::~RestReply() {
  if (m_network_reply) m_network_reply->deleteLater();
}

void RestReply::abort() { m_network_reply->abort(); }

void RestReply::retry() { m_retry_delay = 0ms; }

void RestReply::retry(std::chrono::milliseconds msecs) {
  m_retry_delay = msecs;
}

}  // namespace egnite::rest
