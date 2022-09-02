#ifndef EGNITE_REPLY_DECORATOR_MANAGER_H
#define EGNITE_REPLY_DECORATOR_MANAGER_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QObject>
/* ---------------------------------- Standard ------------------------------ */
#include <memory>
/* ----------------------------------- Local -------------------------------- */
#include "egnite/export.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

namespace detail {
class RestReplyDecoratorManagerPrivate;
class RestReplyLogDecoratorPrivate;
}  // namespace detail

class RestReply;

class EGNITE_API RestReplyDecoratorManager : public QObject {
  Q_OBJECT

 public:
  using Decorator = std::function<RestReply*(RestReply*)>;

 public:
  explicit RestReplyDecoratorManager(QObject* parent = nullptr);
  ~RestReplyDecoratorManager() override;

  [[nodiscard]] RestReply* decorate(RestReply* reply) const;

  void addDecorator(const QString& name, Decorator decorator);
  void removeDecorator(const QString& name);
  void removeAllDecorators();

 protected:
  explicit RestReplyDecoratorManager(
      detail::RestReplyDecoratorManagerPrivate& impl,
      QObject* parent = nullptr);

 private:
  Q_DECLARE_PRIVATE(detail::RestReplyDecoratorManager);
};

class EGNITE_API RestReplyLogDecorator {
 public:
  enum class Option {
    LogSucceeded = 0x1,
    LogFailed = 0x2,
    LogError = 0x3,
    LogDownloadProgress = 0x4,
    LogUploadProgress = 0x5,

    LogAll = LogSucceeded | LogFailed | LogError | LogDownloadProgress |
             LogUploadProgress
  };
  Q_DECLARE_FLAGS(Options, Option);

 public:
  explicit RestReplyLogDecorator(const RestReplyLogDecorator& other);
  explicit RestReplyLogDecorator();

  ~RestReplyLogDecorator();

  [[nodiscard]] RestReply* operator()(RestReply* reply) const;

  void setOptions(Options options);
  [[nodiscard]] Options getOptions() const;

 private:
  std::unique_ptr<detail::RestReplyLogDecoratorPrivate> m_impl;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(RestReplyLogDecorator::Options)

}  // namespace egnite::rest

#endif  // EGNITE_REPLY_DECORATOR_MANAGER_H