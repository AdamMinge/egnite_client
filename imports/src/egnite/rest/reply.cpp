/* ----------------------------------- Local -------------------------------- */
#include "reply.h"
/* ----------------------------------- Egnite ------------------------------- */
#include <egnite/rest/reply.h>
/* -------------------------------------------------------------------------- */

/* ---------------------------------- QmlReply ------------------------------ */

QmlReply::QmlReply(QQmlEngine* engine, egnite::rest::IReply* reply,
                   QObject* parent)
    : QObject(parent), m_engine(engine), m_reply(reply) {}

QmlReply::~QmlReply() = default;

void QmlReply::addCompletedHandler(const QJSValue& handler) {
  if (!validate(handler)) return;

  m_reply->onCompleted([this, handler](int http_code,
                                       const egnite::rest::Data& data) {
    std::visit(egnite::core::utils::overloaded{
                   [&](std::nullopt_t) {
                     handler.call({http_code, QJSValue{}});
                   },
                   [&](const auto& data) {
                     handler.call({http_code,
                                   m_engine->toScriptValue(data.toVariant())});
                   }},
               data);
  });
}

void QmlReply::addSucceededHandler(const QJSValue& handler) {
  if (!validate(handler)) return;

  m_reply->onSucceeded([this, handler](int http_code,
                                       const egnite::rest::Data& data) {
    std::visit(egnite::core::utils::overloaded{
                   [&](std::nullopt_t) {
                     handler.call({http_code, QJSValue{}});
                   },
                   [&](const auto& data) {
                     handler.call({http_code,
                                   m_engine->toScriptValue(data.toVariant())});
                   }},
               data);
  });
}

void QmlReply::addFailedHandler(const QJSValue& handler) {
  if (!validate(handler)) return;

  m_reply->onFailed([this, handler](int http_code,
                                    const egnite::rest::Data& data) {
    std::visit(egnite::core::utils::overloaded{
                   [&](std::nullopt_t) {
                     handler.call({http_code, QJSValue{}});
                   },
                   [&](const auto& data) {
                     handler.call({http_code,
                                   m_engine->toScriptValue(data.toVariant())});
                   }},
               data);
  });
}

void QmlReply::addErrorHandler(const QJSValue& handler) {
  if (!validate(handler)) return;

  m_reply->onError([handler](const QString& error_str,
                             egnite::rest::IReply::Error error_type) {
    handler.call({error_str, static_cast<int>(error_type)});
  });
}

void QmlReply::addUnsucceededHandler(const QJSValue& handler) {
  if (!validate(handler)) return;
  m_reply->OnUnsucceeded([handler]() { handler.call(); });
}

void QmlReply::addDownloadProgressHandler(const QJSValue& handler) {
  if (!validate(handler)) return;
  m_reply->onDownloadProgress(
      [handler](qint64 bytes_received, qint64 bytes_total) {
        handler.call(
            {static_cast<int>(bytes_received), static_cast<int>(bytes_total)});
      });
}

void QmlReply::addUploadProgressHandler(const QJSValue& handler) {
  if (!validate(handler)) return;
  m_reply->onUploadProgress([handler](qint64 bytes_sent, qint64 bytes_total) {
    handler.call({static_cast<int>(bytes_sent), static_cast<int>(bytes_total)});
  });
}

bool QmlReply::validate(const QJSValue& handler) const {
  if (!handler.isCallable()) {
    qmlWarning(this) << "Passed JS object is not a callable function!";
    return false;
  }

  return true;
}