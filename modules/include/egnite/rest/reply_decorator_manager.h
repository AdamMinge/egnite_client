#ifndef EGNITE_REPLY_DECORATOR_MANAGER_H
#define EGNITE_REPLY_DECORATOR_MANAGER_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QObject>
/* ---------------------------------- Standard ------------------------------ */
#include <memory>
/* ----------------------------------- Local -------------------------------- */
#include "egnite/rest/export.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

namespace detail {
class ReplyDecoratorManagerPrivate;
class ReplyLogDecoratorPrivate;
}  // namespace detail

class Reply;

class ReplyDecorator {
 public:
  explicit ReplyDecorator();
  virtual ~ReplyDecorator();

  [[nodiscard]] virtual Reply* decorate(Reply* reply) const = 0;
};

class EGNITE_REST_API ReplyDecoratorManager : public QObject {
  Q_OBJECT

 public:
  explicit ReplyDecoratorManager(QObject* parent = nullptr);
  ~ReplyDecoratorManager() override;

  [[nodiscard]] Reply* decorate(Reply* reply) const;

  void addDecorator(ReplyDecorator* decorator, uint32_t priority);
  void removeDecorator(ReplyDecorator* decorator);
  void removeAllDecorators();

 protected:
  explicit ReplyDecoratorManager(detail::ReplyDecoratorManagerPrivate& impl,
                                 QObject* parent = nullptr);

 private:
  Q_DECLARE_PRIVATE(detail::ReplyDecoratorManager);
};

class EGNITE_REST_API ReplyLogDecorator : public ReplyDecorator {
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
  explicit ReplyLogDecorator(const ReplyLogDecorator& other);
  explicit ReplyLogDecorator();

  ~ReplyLogDecorator();

  [[nodiscard]] Reply* decorate(Reply* reply) const override;

  void setOptions(Options options);
  [[nodiscard]] Options getOptions() const;

 private:
  std::unique_ptr<detail::ReplyLogDecoratorPrivate> m_impl;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(ReplyLogDecorator::Options)

}  // namespace egnite::rest

#endif  // EGNITE_REPLY_DECORATOR_MANAGER_H