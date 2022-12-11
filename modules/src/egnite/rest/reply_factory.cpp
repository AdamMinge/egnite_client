/* ----------------------------------- Local -------------------------------- */
#include "egnite/rest/reply_factory.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

/* ------------------------------- IReplyFactory ---------------------------- */

IReplyFactory::IReplyFactory(QObject* parent) : QObject(parent) {}

IReplyFactory::IReplyFactory(QObjectPrivate& impl, QObject* parent)
    : QObject(impl, parent) {}

IReplyFactory::~IReplyFactory() = default;

/* ----------------------------- LoggerReplyFactory ------------------------- */

LoggerReplyFactory::LoggerReplyFactory(QObject* parent)
    : IReplyFactory(parent) {}

LoggerReplyFactory::~LoggerReplyFactory() = default;

IReply* LoggerReplyFactory::create(IReply* reply) {
  auto decorated_reply = new LoggerReply<QTextStream>(
      std::make_unique<QTextStream>(stdout), reply, reply->parent());
  reply->setParent(decorated_reply);
  return decorated_reply;
}

}  // namespace egnite::rest
