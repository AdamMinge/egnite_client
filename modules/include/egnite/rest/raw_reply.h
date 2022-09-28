#ifndef EGNITE_REST_RAW_REPLY_H
#define EGNITE_REST_RAW_REPLY_H

/* ----------------------------------- Local -------------------------------- */
#include "egnite/rest/export.h"
#include "egnite/rest/reply.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

namespace detail {
class RawReplyPrivate;
}

class EGNITE_REST_API RawReply : public Reply {
  Q_OBJECT

  friend Api;

 public:
  ~RawReply() override;

  void abort() override;
  void retry() override;

  [[nodiscard]] Api* getApi() const override;
  [[nodiscard]] Client* getClient() const override;
  [[nodiscard]] DataSerializer* getDataSerializer() const override;

 protected:
  RawReply(Api* api, QNetworkReply* network_reply, QObject* parent = nullptr);

 private:
  Q_DECLARE_PRIVATE(detail::RawReply)
};

}  // namespace egnite::rest

#endif  // EGNITE_REST_RAW_REPLY_H