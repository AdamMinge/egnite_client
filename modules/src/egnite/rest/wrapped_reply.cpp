/* ----------------------------------- Local -------------------------------- */
#include "egnite/rest/wrapped_reply.h"

#include "egnite/rest/detail/wrapped_reply_p.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

/* ------------------------------- WrappedReply ----------------------------- */

WrappedReply::WrappedReply(Reply* reply, QObject* parent)
    : Reply(*new detail::WrappedReplyPrivate(reply), parent) {}

WrappedReply::~WrappedReply() = default;

void WrappedReply::abort() {
  Q_D(detail::WrappedReply);
  d->abort();
}

void WrappedReply::retry() {
  Q_D(detail::WrappedReply);
  d->retry();
}

Api* WrappedReply::getApi() const {
  Q_D(const detail::WrappedReply);
  return d->getApi();
}

Client* WrappedReply::getClient() const {
  Q_D(const detail::WrappedReply);
  return d->getClient();
}

DataSerializer* WrappedReply::getDataSerializer() const {
  Q_D(const detail::WrappedReply);
  return d->getDataSerializer();
}

void WrappedReply::setAutoDelete(bool enable) {
  Q_D(detail::WrappedReply);
  d->setAutoDelete(enable);
}

bool WrappedReply::isAutoDelete() const {
  Q_D(const detail::WrappedReply);
  return d->isAutoDelete();
}

/* ------------------------- WrappedReplyPrivate ------------------------ */

namespace detail {

WrappedReplyPrivate::WrappedReplyPrivate(Reply* reply)
    : m_reply(reply), m_auto_delete(false) {
  Q_Q(WrappedReply);

  m_reply->setParent(q);

  QObject::connect(m_reply, &Reply::completed, q, &Reply::completed);
  QObject::connect(m_reply, &Reply::succeeded, q, &Reply::succeeded);
  QObject::connect(m_reply, &Reply::failed, q, &Reply::failed);
  QObject::connect(m_reply, &Reply::error, q, &Reply::error);

  QObject::connect(m_reply, &Reply::downloadProgress, q,
                   &Reply::downloadProgress);
  QObject::connect(m_reply, &Reply::uploadProgress, q, &Reply::uploadProgress);
}

WrappedReplyPrivate::~WrappedReplyPrivate() = default;

void WrappedReplyPrivate::abort() { m_reply->abort(); }

void WrappedReplyPrivate::retry() { m_reply->retry(); }

Api* WrappedReplyPrivate::getApi() const { return m_reply->getApi(); }

Client* WrappedReplyPrivate::getClient() const { return m_reply->getClient(); }

DataSerializer* WrappedReplyPrivate::getDataSerializer() const {
  return m_reply->getDataSerializer();
}

void WrappedReplyPrivate::setAutoDelete(bool enable) {
  Q_Q(WrappedReply);
  if (m_auto_delete == enable) return;

  m_auto_delete = enable;

  if (m_auto_delete) {
    QObject::connect(q, &Reply::succeeded, q, &QObject::deleteLater);
    QObject::connect(q, &Reply::error, q, &QObject::deleteLater);
    QObject::connect(q, &Reply::failed, q, &QObject::deleteLater);
  } else {
    QObject::disconnect(q, &Reply::succeeded, q, &QObject::deleteLater);
    QObject::disconnect(q, &Reply::error, q, &QObject::deleteLater);
    QObject::disconnect(q, &Reply::failed, q, &QObject::deleteLater);
  }

  Q_EMIT q->autoDeleteChanged(m_auto_delete);
}

bool WrappedReplyPrivate::isAutoDelete() const { return m_auto_delete; }

}  // namespace detail

}  // namespace egnite::rest

#include "egnite/rest/moc_wrapped_reply.cpp"