#ifndef EGNITE_REST_WRAPPED_REPLY_H
#define EGNITE_REST_WRAPPED_REPLY_H

/* ----------------------------------- Local -------------------------------- */
#include "egnite/rest/export.h"
#include "egnite/rest/reply.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

namespace detail {
class WrappedReplyPrivate;
}

class EGNITE_REST_API WrappedReply : public Reply {
  Q_OBJECT

 public:
  ~WrappedReply() override;

  void abort() override;
  void retry() override;

  [[nodiscard]] Api* getApi() const override;
  [[nodiscard]] Client* getClient() const override;
  [[nodiscard]] DataSerializer* getDataSerializer() const override;

  void setAutoDelete(bool enable) override;
  [[nodiscard]] bool isAutoDelete() const override;

 protected:
  WrappedReply(Reply* reply, QObject* parent = nullptr);

 private:
  Q_DECLARE_PRIVATE(detail::WrappedReply)
};

}  // namespace egnite::rest

#endif  // EGNITE_REST_WRAPPED_REPLY_H