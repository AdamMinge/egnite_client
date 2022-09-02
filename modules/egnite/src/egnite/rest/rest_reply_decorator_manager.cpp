/* ----------------------------------- Local -------------------------------- */
#include "egnite/rest/rest_reply_decorator_manager.h"

#include "egnite/rest/detail/rest_api_p.h"
#include "egnite/rest/detail/rest_reply_decorator_manager_p.h"
#include "egnite/rest/rest_reply.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

/* ------------------------ RestReplyDecoratorManager------------------------ */

RestReplyDecoratorManager::RestReplyDecoratorManager(QObject* parent)
    : RestReplyDecoratorManager(*new detail::RestReplyDecoratorManagerPrivate(),
                                parent) {}

RestReplyDecoratorManager::~RestReplyDecoratorManager() = default;

RestReplyDecoratorManager::RestReplyDecoratorManager(
    detail::RestReplyDecoratorManagerPrivate& impl, QObject* parent)
    : QObject(impl, parent) {}

RestReply* RestReplyDecoratorManager::decorate(RestReply* reply) const {
  Q_D(const detail::RestReplyDecoratorManager);
  return d->decorate(reply);
}

void RestReplyDecoratorManager::addDecorator(const QString& name,
                                             Decorator decorator) {
  Q_D(detail::RestReplyDecoratorManager);
  d->addDecorator(name, decorator);
}

void RestReplyDecoratorManager::removeDecorator(const QString& name) {
  Q_D(detail::RestReplyDecoratorManager);
  d->removeDecorator(name);
}

void RestReplyDecoratorManager::removeAllDecorators() {
  Q_D(detail::RestReplyDecoratorManager);
  d->removeAllDecorators();
}

/* -------------------------- RestReplyLogDecorator ------------------------- */

RestReplyLogDecorator::RestReplyLogDecorator(const RestReplyLogDecorator& other)
    : m_impl(std::make_unique<detail::RestReplyLogDecoratorPrivate>(
          *other.m_impl)) {}

RestReplyLogDecorator::RestReplyLogDecorator()
    : m_impl(std::make_unique<detail::RestReplyLogDecoratorPrivate>(
          Option::LogAll)) {}

RestReplyLogDecorator::~RestReplyLogDecorator() = default;

RestReply* RestReplyLogDecorator::operator()(RestReply* reply) const {
  return m_impl->decorate(reply);
}

void RestReplyLogDecorator::setOptions(Options log_detail) {
  m_impl->setOptions(log_detail);
}

RestReplyLogDecorator::Options RestReplyLogDecorator::getOptions() const {
  return m_impl->getOptions();
}

/* -------------------- RestReplyDecoratorManagerPrivate -------------------- */

namespace detail {

RestReplyDecoratorManagerPrivate::RestReplyDecoratorManagerPrivate() {}

RestReply* RestReplyDecoratorManagerPrivate::decorate(RestReply* reply) const {
  for (auto& decorator : m_decorators) reply = decorator(reply);
  return reply;
}

void RestReplyDecoratorManagerPrivate::addDecorator(
    const QString& name, RestReplyDecoratorManager::Decorator decorator) {
  m_decorators.insert(name, decorator);
}

void RestReplyDecoratorManagerPrivate::removeDecorator(const QString& name) {
  m_decorators.remove(name);
}

void RestReplyDecoratorManagerPrivate::removeAllDecorators() {
  m_decorators.clear();
}

/* ---------------------- RestReplyLogDecoratorPrivate ---------------------- */

RestReplyLogDecoratorPrivate::RestReplyLogDecoratorPrivate(
    RestReplyLogDecorator::Options options)
    : m_options(options) {}

RestReply* RestReplyLogDecoratorPrivate::decorate(RestReply* reply) const {
  if (m_options.testFlag(RestReplyLogDecorator::Option::LogSucceeded))
    reply->onSucceeded([reply](int http_code, const RestData& data) {
      RestReplyLogDecoratorPrivate::onSucceeded(reply, http_code, data);
    });

  if (m_options.testFlag(RestReplyLogDecorator::Option::LogFailed))
    reply->onFailed([reply](int http_code, const RestData& data) {
      RestReplyLogDecoratorPrivate::onFailed(reply, http_code, data);
    });

  if (m_options.testFlag(RestReplyLogDecorator::Option::LogError))
    reply->onError([reply](const QString& err_str, RestReply::Error err_type) {
      RestReplyLogDecoratorPrivate::onError(reply, err_str, err_type);
    });

  if (m_options.testFlag(RestReplyLogDecorator::Option::LogDownloadProgress))
    reply->onDownloadProgress(
        [reply](qint64 bytes_received, qint64 bytes_total) {
          RestReplyLogDecoratorPrivate::onDownloadProgress(
              reply, bytes_received, bytes_total);
        });

  if (m_options.testFlag(RestReplyLogDecorator::Option::LogUploadProgress))
    reply->onUploadProgress([reply](qint64 bytes_received, qint64 bytes_total) {
      RestReplyLogDecoratorPrivate::onUploadProgress(reply, bytes_received,
                                                     bytes_total);
    });

  return reply;
}

void RestReplyLogDecoratorPrivate::setOptions(
    RestReplyLogDecorator::Options options) {
  m_options = options;
}

RestReplyLogDecorator::Options RestReplyLogDecoratorPrivate::getOptions()
    const {
  return m_options;
}

void RestReplyLogDecoratorPrivate::onSucceeded(RestReply* reply, int http_code,
                                               const RestData& data) {
  qDebug()
      << QString("[reply %1] succeeded")
             .arg(QString::number((std::uintptr_t)(reply)))
      << QString("\t code: %1").arg(http_code)
      << QString("\t data: %1").arg(detail::RestApiPrivate::convertData(data));
}

void RestReplyLogDecoratorPrivate::onFailed(RestReply* reply, int http_code,
                                            const RestData& data) {
  qDebug()
      << QString("[reply %1] failed")
             .arg(QString::number((std::uintptr_t)(reply)))
      << QString("\t code: %1").arg(http_code)
      << QString("\t data: %1").arg(detail::RestApiPrivate::convertData(data));
}

void RestReplyLogDecoratorPrivate::onError(RestReply* reply,
                                           const QString& err_str,
                                           RestReply::Error err_type) {
  qDebug() << QString("[reply #%1] error")
                  .arg(QString::number((std::uintptr_t)(reply)))
           << QString("\t error: %1").arg((int)err_type)
           << QString("\t info: %1").arg(err_str);
}

void RestReplyLogDecoratorPrivate::onDownloadProgress(RestReply* reply,
                                                      qint64 bytes_received,
                                                      qint64 bytes_total) {
  qDebug() << QString("[reply #%1] download progress")
                  .arg(QString::number((std::uintptr_t)(reply)))
           << QString("\t bytes_received: %1").arg(bytes_received)
           << QString("\t bytes_total: %1").arg(bytes_total);
}

void RestReplyLogDecoratorPrivate::onUploadProgress(RestReply* reply,
                                                    qint64 bytes_received,
                                                    qint64 bytes_total) {
  qDebug() << QString("[reply #%1] upload progress")
                  .arg(QString::number((std::uintptr_t)(reply)))
           << QString("\t bytes_received: %1").arg(bytes_received)
           << QString("\t bytes_total: %1").arg(bytes_total);
}

}  // namespace detail

}  // namespace egnite::rest

#include "egnite/rest/moc_rest_reply_decorator_manager.cpp"