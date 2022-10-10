/* ----------------------------------- Local -------------------------------- */
#include "reply_factory.h"
/* ----------------------------------- Egnite ------------------------------- */
#include <egnite/rest/logger_reply.h>
#include <egnite/rest/reply_decorator_factory.h>
/* -------------------------------------------------------------------------- */

/* ------------------------------ QmlReplyFactory --------------------------- */

QmlReplyFactory::QmlReplyFactory(QObject* parent) : QObject(parent) {}

QmlReplyFactory::~QmlReplyFactory() = default;

/* ---------------------------- QmlDebugReplyFactory ------------------------ */

class DebugReplyDecoratorFactory : public egnite::rest::ReplyDecoratorFactory {
 public:
  explicit DebugReplyDecoratorFactory(QObject* parent = nullptr)
      : egnite::rest::ReplyDecoratorFactory(parent) {}

  [[nodiscard]] egnite::rest::Reply* create(
      egnite::rest::Reply* reply) override {
    auto decorated_reply = new egnite::rest::LoggerReply<QDebug>(
        qDebug().nospace(), reply, reply->parent());
    reply->setParent(decorated_reply);
    return decorated_reply;
  }
};

QmlDebugReplyFactory::QmlDebugReplyFactory(QObject* parent)
    : QmlReplyFactory(parent),
      m_factory(new DebugReplyDecoratorFactory(this)) {}

QmlDebugReplyFactory::~QmlDebugReplyFactory() = default;

egnite::rest::ReplyDecoratorFactory* QmlDebugReplyFactory::getFactory() const {
  return m_factory;
}
