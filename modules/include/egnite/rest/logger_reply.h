#ifndef EGNITE_REST_DEBUG_REPLY_H
#define EGNITE_REST_DEBUG_REPLY_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QString>
/* ----------------------------------- Local -------------------------------- */
#include "egnite/rest/wrapped_reply.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

template <typename Logger>
class LoggerReply : public WrappedReply {
 public:
  explicit LoggerReply(Logger logger, Reply* reply, QObject* parent = nullptr);
  ~LoggerReply() override;

 protected:
  void logCompleted(int http_code, const Data& data);
  void logSucceeded(int http_code, const Data& data);
  void logFailed(int http_code, const Data& data);
  void logError(const QString& error_str, Error error_type);

  void logDownloadProgress(qint64 bytes_received, qint64 bytes_total);
  void logUploadProgress(qint64 bytes_sent, qint64 bytes_total);

 private:
  void logTheme(const QString& action);
  void logData(const Data& data);

 private:
  Logger m_logger;
};

template <typename Logger>
LoggerReply<Logger>::LoggerReply(Logger logger, Reply* reply, QObject* parent)
    : WrappedReply(reply, parent), m_logger(logger) {
  onCompleted([this](int code, const Data& data) { logCompleted(code, data); });
  onSucceeded([this](int code, const Data& data) { logSucceeded(code, data); });
  onFailed([this](int code, const Data& data) { logFailed(code, data); });
  onError([this](const QString& str, Error type) { logError(str, type); });
  onDownloadProgress([this](qint64 r, qint64 t) { logDownloadProgress(r, t); });
  onUploadProgress([this](qint64 s, qint64 t) { logUploadProgress(s, t); });
}

template <typename Logger>
LoggerReply<Logger>::~LoggerReply() = default;

template <typename Logger>
void LoggerReply<Logger>::logCompleted(int http_code, const Data& data) {
  logTheme("Completed");
  m_logger << "\t" << QString("http_code: %1").arg(http_code);
  logData(data);
}

template <typename Logger>
void LoggerReply<Logger>::logSucceeded(int http_code, const Data& data) {
  logTheme("Succeeded");
  m_logger << "\t" << QString("http_code: %1").arg(http_code);
  logData(data);
}

template <typename Logger>
void LoggerReply<Logger>::logFailed(int http_code, const Data& data) {
  logTheme("Failed");
  m_logger << "\t" << QString("http code: %1").arg(http_code);
  logData(data);
}

template <typename Logger>
void LoggerReply<Logger>::logError(const QString& error_str, Error error_type) {
  logTheme("Error");
  m_logger << "\t" << QString("detail: %1").arg(error_str) << "\n"
           << "\t" << QString("type: %1").arg(static_cast<size_t>(error_type))
           << "\n";
}

template <typename Logger>
void LoggerReply<Logger>::logDownloadProgress(qint64 bytes_received,
                                              qint64 bytes_total) {
  logTheme("Download Progress");
  m_logger << "\t" << QString("bytes received: %1").arg(bytes_received) << "\n"
           << "\t" << QString("bytes total: %1").arg(bytes_total) << "\n";
}

template <typename Logger>
void LoggerReply<Logger>::logUploadProgress(qint64 bytes_sent,
                                            qint64 bytes_total) {
  logTheme("Upload Progress");
  m_logger << "\t" << QString("bytes sent: %1").arg(bytes_sent) << "\n"
           << "\t" << QString("bytes total: %1").arg(bytes_total) << "\n";
}

template <typename Logger>
void LoggerReply<Logger>::logTheme(const QString& action) {
  m_logger << QString("%1 Reply ").arg(action) << "[" << this << "]: \n";
}

template <typename Logger>
void LoggerReply<Logger>::logData(const Data& data) {
  m_logger << "\t"
           << "data: ";
  std::visit(core::utils::overloaded{
                 [this](std::nullopt_t) { m_logger << "null"; },
                 [this](const QJsonValue& body) { m_logger << body; },
                 [this](const QCborValue& body) { m_logger << body; }},
             data);
  m_logger << "\n";
}

}  // namespace egnite::rest

#endif  // EGNITE_REST_DEBUG_REPLY_H