/* ----------------------------------- Local -------------------------------- */
#include "reply_factory.h"
/* ----------------------------------- Egnite ------------------------------- */
#include <egnite/rest/reply_factory.h>
/* -------------------------------------------------------------------------- */

/* ----------------------------- QmlLoggerReplyFactory ---------------------- */

QmlLoggerReplyFactory::QmlLoggerReplyFactory(QObject* parent)
    : egnite::rest::IReplyFactory(parent),
      m_factory(new egnite::rest::LoggerReplyFactory(this)) {}

QmlLoggerReplyFactory::~QmlLoggerReplyFactory() = default;

egnite::rest::IReply* QmlLoggerReplyFactory::create(
    egnite::rest::IReply* reply) {
  return m_factory->create(reply);
}
