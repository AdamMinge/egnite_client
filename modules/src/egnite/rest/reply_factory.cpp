/* ----------------------------------- Local -------------------------------- */
#include "egnite/rest/reply_factory.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

/* ------------------------------- IReplyFactory ---------------------------- */

IReplyFactory::IReplyFactory(QObject* parent) : QObject(parent) {}

IReplyFactory::IReplyFactory(QObjectPrivate& impl, QObject* parent)
    : QObject(impl, parent) {}

IReplyFactory::~IReplyFactory() = default;

/* ----------------------------- DebugReplyFactory -------------------------- */

DebugReplyFactory::DebugReplyFactory(QObject* parent) : IReplyFactory(parent) {}

DebugReplyFactory::~DebugReplyFactory() = default;

IReply* DebugReplyFactory::create(IReply* reply) {
  auto decorated_reply =
      new LoggerReply<QDebug>(qDebug().nospace(), reply, reply->parent());
  reply->setParent(decorated_reply);
  return decorated_reply;
}

}  // namespace egnite::rest
