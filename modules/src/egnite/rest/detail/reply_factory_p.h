#ifndef EGNITE_REPLY_FACTORY_P_H
#define EGNITE_REPLY_FACTORY_P_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QtCore/private/qobject_p.h>
/* ------------------------------------ Local ------------------------------- */
#include "egnite/rest/reply_factory.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest::detail {

/* ------------------------- LoggerReplyFactoryPrivate ---------------------- */

class LoggerReplyFactoryPrivate : public QObjectPrivate {
 public:
  Q_DECLARE_PUBLIC(LoggerReplyFactory)

 public:
  explicit LoggerReplyFactoryPrivate(ILoggerReply::LogDetail log_detail);

  void setLogDetail(ILoggerReply::LogDetail log_detail);
  [[nodiscard]] ILoggerReply::LogDetail getLogDetail() const;

  [[nodiscard]] IReply* create(IReply* reply);

 private:
  ILoggerReply::LogDetail m_log_detail;
};

}  // namespace egnite::rest::detail

#endif  // EGNITE_REPLY_FACTORY_P_H