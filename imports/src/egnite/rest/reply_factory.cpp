/* ----------------------------------- Local -------------------------------- */
#include "reply_factory.h"
/* ----------------------------------- Egnite ------------------------------- */
#include <egnite/rest/reply_factory.h>
/* -------------------------------------------------------------------------- */

/* ----------------------------- QmlLoggerReplyFactory ---------------------- */

QmlLoggerReplyFactory::QmlLoggerReplyFactory(
    egnite::rest::ILoggerReply::LogDetail log_detail, QObject* parent)
    : egnite::rest::LoggerReplyFactory(log_detail, parent) {}

QmlLoggerReplyFactory::~QmlLoggerReplyFactory() = default;
