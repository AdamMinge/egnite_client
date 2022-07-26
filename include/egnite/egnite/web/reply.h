#ifndef EGNITE_WEB_REPLY_H
#define EGNITE_WEB_REPLY_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QNetworkReply>
/* ----------------------------------- Local -------------------------------- */
#include "egnite/egnite/export.h"
#include "egnite/egnite/web/serializer.h"
/* -------------------------------------------------------------------------- */

namespace egnite::web {

template <typename PARENT_TYPE> class SuccessReply {

public:
  virtual ~SuccessReply() = default;

  template <typename SUCCESS_TYPE>
      PARENT_TYPE& onSuccess(std::function<void(const SUCCESS_TYPE&)>&& method) {
    m_reply->connect(m_reply, &QNetworkReply::finished, m_reply,
                     [reply = m_reply, serializer = m_serializer, method = std::move(method)]() {
                       if (reply->error() == QNetworkReply::NoError) {
                           method(serializer->deserialize<SUCCESS_TYPE>(reply->readAll()));
                       }
                     });

    return dynamic_cast<PARENT_TYPE&>(*this);
  }

  PARENT_TYPE& onSuccess(std::function<void(void)>&& method) {
    m_reply->connect(m_reply, &QNetworkReply::finished, m_reply,
                     [reply = m_reply, serializer = m_serializer, method = std::move(method)]() {
                       if (reply->error() == QNetworkReply::NoError) {
                         method();
                       }
                     });

    return dynamic_cast<PARENT_TYPE&>(*this);
  }

protected:
  explicit SuccessReply(QNetworkReply* reply, Serializer* serializer)
      : m_reply(reply), m_serializer(serializer) {}

private:
  QNetworkReply* m_reply;
  Serializer* m_serializer;
};

template <typename PARENT_TYPE> class ErrorReply {

public:
  virtual ~ErrorReply() = default;

  PARENT_TYPE& onError(std::function<void(QNetworkReply::NetworkError)>&& method) {
    m_reply->connect(m_reply, &QNetworkReply::errorOccurred, m_reply, std::move(method));
    return dynamic_cast<PARENT_TYPE&>(*this);
  }

protected:
  explicit ErrorReply(QNetworkReply* reply) : m_reply(reply) {}

private:
  QNetworkReply* m_reply;
};

template <typename PARENT_TYPE> class SslErrorReply {

public:
  virtual ~SslErrorReply() = default;

  PARENT_TYPE& onSslError(std::function<void(const QList<QSslError>&)>&& method) {
    m_reply->connect(m_reply, &QNetworkReply::sslErrors, m_reply, std::move(method));
    return dynamic_cast<PARENT_TYPE&>(*this);
  }

protected:
  explicit SslErrorReply(QNetworkReply* reply) : m_reply(reply) {}

private:
  QNetworkReply* m_reply;
};

class EGNITE_API Reply : public SuccessReply<Reply>,
                         public ErrorReply<Reply>,
                         public SslErrorReply<Reply> {

  friend class Client;

protected:
  explicit Reply(QNetworkReply* reply, Serializer* serializer)
      : SuccessReply<Reply>(reply, serializer), ErrorReply<Reply>(reply), SslErrorReply<Reply>(
                                                                              reply) {}
};

} // namespace egnite::web

#endif // EGNITE_WEB_REPLY_H
