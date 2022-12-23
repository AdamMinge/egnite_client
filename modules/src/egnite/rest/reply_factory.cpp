/* ----------------------------------- Local -------------------------------- */
#include "egnite/rest/reply_factory.h"

#include "egnite/rest/detail/reply_factory_p.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

/* ------------------------------- IReplyFactory ---------------------------- */

IReplyFactory::IReplyFactory(QObject* parent) : QObject(parent) {}

IReplyFactory::IReplyFactory(QObjectPrivate& impl, QObject* parent)
    : QObject(impl, parent) {}

IReplyFactory::~IReplyFactory() = default;

/* ----------------------------- LoggerReplyFactory ------------------------- */

LoggerReplyFactory::LoggerReplyFactory(ILoggerReply::LogDetail log_detail,
                                       QObject* parent)
    : IReplyFactory(*new detail::LoggerReplyFactoryPrivate(log_detail),
                    parent) {}

LoggerReplyFactory::~LoggerReplyFactory() = default;

void LoggerReplyFactory::setLogDetail(ILoggerReply::LogDetail log_detail) {
  Q_D(detail::LoggerReplyFactory);
  return d->setLogDetail(log_detail);
}

ILoggerReply::LogDetail LoggerReplyFactory::getLogDetail() const {
  Q_D(const detail::LoggerReplyFactory);
  return d->getLogDetail();
}

IReply* LoggerReplyFactory::create(IReply* reply) {
  Q_D(detail::LoggerReplyFactory);
  return d->create(reply);
}

/* ------------------------ LoggerReplyFactoryPrivate ----------------------- */

namespace detail {

LoggerReplyFactoryPrivate::LoggerReplyFactoryPrivate(
    ILoggerReply::LogDetail log_detail)
    : m_log_detail(log_detail) {}

void LoggerReplyFactoryPrivate::setLogDetail(
    ILoggerReply::LogDetail log_detail) {
  Q_Q(LoggerReplyFactory);
  if (m_log_detail == log_detail) return;

  m_log_detail = log_detail;
  Q_EMIT q->logDetailChanged(m_log_detail);
}

ILoggerReply::LogDetail LoggerReplyFactoryPrivate::getLogDetail() const {
  return m_log_detail;
}

IReply* LoggerReplyFactoryPrivate::create(IReply* reply) {
  auto decorated_reply =
      new LoggerReply<QTextStream>(std::make_unique<QTextStream>(stdout), reply,
                                   m_log_detail, reply->parent());
  reply->setParent(decorated_reply);
  return decorated_reply;
}

}  // namespace detail

}  // namespace egnite::rest
