#ifndef EGNITE_REST_WRAPPED_REPLY_P_H
#define EGNITE_REST_WRAPPED_REPLY_P_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QtCore/private/qobject_p.h>
/* ------------------------------------ Local ------------------------------- */
#include "egnite/rest/wrapped_reply.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest::detail {

class WrappedReplyPrivate : public QObjectPrivate {
 public:
  Q_DECLARE_PUBLIC(WrappedReply)

 public:
  explicit WrappedReplyPrivate(Reply* reply);
  ~WrappedReplyPrivate() override;

  void abort();
  void retry();

  [[nodiscard]] Api* getApi() const;
  [[nodiscard]] Client* getClient() const;
  [[nodiscard]] DataSerializer* getDataSerializer() const;

  void setAutoDelete(bool enable);
  [[nodiscard]] bool isAutoDelete() const;

  void connectReply();

 private:
  Reply* m_reply;
  bool m_auto_delete;
};

}  // namespace egnite::rest::detail

#endif  // EGNITE_REST_WRAPPED_REPLY_P_H