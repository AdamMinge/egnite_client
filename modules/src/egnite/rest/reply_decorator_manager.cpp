/* ----------------------------------- Local -------------------------------- */
#include "egnite/rest/reply_decorator_manager.h"

#include "egnite/rest/detail/api_p.h"
#include "egnite/rest/detail/reply_decorator_manager_p.h"
#include "egnite/rest/reply.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

/* -------------------------- ReplyDecoratorManager-------------------------- */

ReplyDecoratorManager::ReplyDecoratorManager(QObject* parent)
    : ReplyDecoratorManager(*new detail::ReplyDecoratorManagerPrivate(),
                            parent) {}

ReplyDecoratorManager::~ReplyDecoratorManager() = default;

ReplyDecoratorManager::ReplyDecoratorManager(
    detail::ReplyDecoratorManagerPrivate& impl, QObject* parent)
    : QObject(impl, parent) {}

Reply* ReplyDecoratorManager::decorate(Reply* reply) const {
  Q_D(const detail::ReplyDecoratorManager);
  return d->decorate(reply);
}

void ReplyDecoratorManager::addDecorator(const QString& name,
                                         Decorator decorator) {
  Q_D(detail::ReplyDecoratorManager);
  d->addDecorator(name, decorator);
}

void ReplyDecoratorManager::removeDecorator(const QString& name) {
  Q_D(detail::ReplyDecoratorManager);
  d->removeDecorator(name);
}

void ReplyDecoratorManager::removeAllDecorators() {
  Q_D(detail::ReplyDecoratorManager);
  d->removeAllDecorators();
}

/* -------------------------- ReplyLogDecorator ------------------------- */

ReplyLogDecorator::ReplyLogDecorator(const ReplyLogDecorator& other)
    : m_impl(
          std::make_unique<detail::ReplyLogDecoratorPrivate>(*other.m_impl)) {}

ReplyLogDecorator::ReplyLogDecorator()
    : m_impl(
          std::make_unique<detail::ReplyLogDecoratorPrivate>(Option::LogAll)) {}

ReplyLogDecorator::~ReplyLogDecorator() = default;

Reply* ReplyLogDecorator::operator()(Reply* reply) const {
  return m_impl->decorate(reply);
}

void ReplyLogDecorator::setOptions(Options log_detail) {
  m_impl->setOptions(log_detail);
}

ReplyLogDecorator::Options ReplyLogDecorator::getOptions() const {
  return m_impl->getOptions();
}

/* -------------------- ReplyDecoratorManagerPrivate -------------------- */

namespace detail {

ReplyDecoratorManagerPrivate::ReplyDecoratorManagerPrivate() {}

Reply* ReplyDecoratorManagerPrivate::decorate(Reply* reply) const {
  for (auto& decorator : m_decorators) reply = decorator(reply);
  return reply;
}

void ReplyDecoratorManagerPrivate::addDecorator(
    const QString& name, ReplyDecoratorManager::Decorator decorator) {
  m_decorators.insert(name, decorator);
}

void ReplyDecoratorManagerPrivate::removeDecorator(const QString& name) {
  m_decorators.remove(name);
}

void ReplyDecoratorManagerPrivate::removeAllDecorators() {
  m_decorators.clear();
}

/* ------------------------ ReplyLogDecoratorPrivate ------------------------ */

ReplyLogDecoratorPrivate::ReplyLogDecoratorPrivate(
    ReplyLogDecorator::Options options)
    : m_options(options) {}

Reply* ReplyLogDecoratorPrivate::decorate(Reply* reply) const {
  if (m_options.testFlag(ReplyLogDecorator::Option::LogSucceeded))
    reply->onSucceeded([reply](int http_code, const Data& data) {
      ReplyLogDecoratorPrivate::onSucceeded(reply, http_code, data);
    });

  if (m_options.testFlag(ReplyLogDecorator::Option::LogFailed))
    reply->onFailed([reply](int http_code, const Data& data) {
      ReplyLogDecoratorPrivate::onFailed(reply, http_code, data);
    });

  if (m_options.testFlag(ReplyLogDecorator::Option::LogError))
    reply->onError([reply](const QString& err_str, Reply::Error err_type) {
      ReplyLogDecoratorPrivate::onError(reply, err_str, err_type);
    });

  if (m_options.testFlag(ReplyLogDecorator::Option::LogDownloadProgress))
    reply->onDownloadProgress(
        [reply](qint64 bytes_received, qint64 bytes_total) {
          ReplyLogDecoratorPrivate::onDownloadProgress(reply, bytes_received,
                                                       bytes_total);
        });

  if (m_options.testFlag(ReplyLogDecorator::Option::LogUploadProgress))
    reply->onUploadProgress([reply](qint64 bytes_received, qint64 bytes_total) {
      ReplyLogDecoratorPrivate::onUploadProgress(reply, bytes_received,
                                                 bytes_total);
    });

  return reply;
}

void ReplyLogDecoratorPrivate::setOptions(ReplyLogDecorator::Options options) {
  m_options = options;
}

ReplyLogDecorator::Options ReplyLogDecoratorPrivate::getOptions() const {
  return m_options;
}

void ReplyLogDecoratorPrivate::onSucceeded(Reply* reply, int http_code,
                                           const Data& data) {
  qDebug() << QString("[reply %1] succeeded")
                  .arg(QString::number((std::uintptr_t)(reply)))
           << QString("\t code: %1").arg(http_code)
           << QString("\t data: %1").arg(detail::ApiPrivate::convertData(data));
}

void ReplyLogDecoratorPrivate::onFailed(Reply* reply, int http_code,
                                        const Data& data) {
  qDebug() << QString("[reply %1] failed")
                  .arg(QString::number((std::uintptr_t)(reply)))
           << QString("\t code: %1").arg(http_code)
           << QString("\t data: %1").arg(detail::ApiPrivate::convertData(data));
}

void ReplyLogDecoratorPrivate::onError(Reply* reply, const QString& err_str,
                                       Reply::Error err_type) {
  qDebug() << QString("[reply #%1] error")
                  .arg(QString::number((std::uintptr_t)(reply)))
           << QString("\t error: %1").arg((int)err_type)
           << QString("\t info: %1").arg(err_str);
}

void ReplyLogDecoratorPrivate::onDownloadProgress(Reply* reply,
                                                  qint64 bytes_received,
                                                  qint64 bytes_total) {
  qDebug() << QString("[reply #%1] download progress")
                  .arg(QString::number((std::uintptr_t)(reply)))
           << QString("\t bytes_received: %1").arg(bytes_received)
           << QString("\t bytes_total: %1").arg(bytes_total);
}

void ReplyLogDecoratorPrivate::onUploadProgress(Reply* reply,
                                                qint64 bytes_received,
                                                qint64 bytes_total) {
  qDebug() << QString("[reply #%1] upload progress")
                  .arg(QString::number((std::uintptr_t)(reply)))
           << QString("\t bytes_received: %1").arg(bytes_received)
           << QString("\t bytes_total: %1").arg(bytes_total);
}

}  // namespace detail

}  // namespace egnite::rest

#include "egnite/rest/moc_reply_decorator_manager.cpp"