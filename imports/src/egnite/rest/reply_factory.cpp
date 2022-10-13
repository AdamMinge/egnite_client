/* ----------------------------------- Local -------------------------------- */
#include "reply_factory.h"
/* ----------------------------------- Egnite ------------------------------- */
#include <egnite/rest/reply_factory.h>
/* -------------------------------------------------------------------------- */

/* ----------------------------- QmlDebugReplyFactory ----------------------- */

QmlDebugReplyFactory::QmlDebugReplyFactory(QObject* parent)
    : egnite::rest::IReplyFactory(parent),
      m_factory(new egnite::rest::DebugReplyFactory(this)) {}

QmlDebugReplyFactory::~QmlDebugReplyFactory() = default;

egnite::rest::IReply* QmlDebugReplyFactory::create(
    egnite::rest::IReply* reply) {
  return m_factory->create(reply);
}
