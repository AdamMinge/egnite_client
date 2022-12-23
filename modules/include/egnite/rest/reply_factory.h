#ifndef EGNITE_REPLY_FACTORY_H
#define EGNITE_REPLY_FACTORY_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QObject>
/* ----------------------------------- Local -------------------------------- */
#include "egnite/rest/export.h"
#include "egnite/rest/reply.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

namespace detail {
class LoggerReplyFactoryPrivate;
}  // namespace detail

/* -------------------------------- IReplyFactory --------------------------- */

class EGNITE_REST_API IReplyFactory : public QObject {
  Q_OBJECT

 public:
  ~IReplyFactory() override;

  [[nodiscard]] virtual IReply* create(IReply* reply) = 0;

 protected:
  explicit IReplyFactory(QObject* parent = nullptr);
  explicit IReplyFactory(QObjectPrivate& impl, QObject* parent = nullptr);
};

/* --------------------------- ReplyFactoryWithArgs ------------------------- */

template <typename Type, typename... Args>
requires std::is_base_of_v<IReply, Type>
class ReplyFactoryWithArgs : public IReplyFactory {
 public:
  ReplyFactoryWithArgs(Args... args, QObject* parent = nullptr)
      : IReplyFactory(parent), m_values(std::make_tuple(std::move(args)...)) {}
  ~ReplyFactoryWithArgs() override = default;

  [[nodiscard]] IReply* create(IReply* reply) override {
    auto decorated_reply = std::apply(
        [reply](const Args&... args) {
          return new Type(args..., reply, reply->parent());
        },
        m_values);

    reply->setParent(decorated_reply);
    return decorated_reply;
  }

 private:
  std::tuple<Args...> m_values;
};

/* ----------------------------- LoggerReplyFactory ------------------------- */

class EGNITE_REST_API LoggerReplyFactory : public IReplyFactory {
  Q_OBJECT

  Q_PROPERTY(ILoggerReply::LogDetail logDetail READ getLogDetail WRITE
                 setLogDetail NOTIFY logDetailChanged)

 public:
  explicit LoggerReplyFactory(
      ILoggerReply::LogDetail log_detail = ILoggerReply::LogDetail::LogAll,
      QObject* parent = nullptr);
  ~LoggerReplyFactory() override;

  void setLogDetail(ILoggerReply::LogDetail log_detail);
  [[nodiscard]] ILoggerReply::LogDetail getLogDetail() const;

  [[nodiscard]] IReply* create(IReply* reply) override;

 Q_SIGNALS:
  void logDetailChanged(ILoggerReply::LogDetail log_detail);

 private:
  Q_DECLARE_PRIVATE(detail::LoggerReplyFactory);
};

}  // namespace egnite::rest

#endif  // EGNITE_REPLY_FACTORY_H