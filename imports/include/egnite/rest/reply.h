#ifndef EGNITE_QML_REST_REPLY_H
#define EGNITE_QML_REST_REPLY_H

/* ------------------------------------- Qt --------------------------------- */
#include <QtQml/QJSEngine>
#include <QtQml/QJSValue>
#include <QtQml>
/* ----------------------------------- Egnite ------------------------------- */
#include <egnite/rest/reply.h>
/* -------------------------------------------------------------------------- */

/* ----------------------------- QmlReplyDecorator -------------------------- */

class QmlReply : public egnite::rest::WrappedReply {
  Q_OBJECT
  QML_ELEMENT

 public:
  explicit QmlReply(QQmlEngine* engine, egnite::rest::IReply* reply,
                    QObject* parent = nullptr);
  ~QmlReply() override;

 public Q_SLOTS:
  void addCompletedHandler(const QJSValue& handler);
  void addSucceededHandler(const QJSValue& handler);
  void addFailedHandler(const QJSValue& handler);
  void addErrorHandler(const QJSValue& handler);
  void addUnsucceededHandler(const QJSValue& handler);

  void addDownloadProgressHandler(const QJSValue& handler);
  void addUploadProgressHandler(const QJSValue& handler);

  void setAutoDelete(bool enable) override;
  [[nodiscard]] bool isAutoDelete() const override;

 private:
  bool validate(const QJSValue& handler) const;

 private:
  QQmlEngine* m_engine;
};

#endif  // EGNITE_QML_REST_REPLY_H