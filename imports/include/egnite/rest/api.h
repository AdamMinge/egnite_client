#ifndef EGNITE_QML_REST_API_H
#define EGNITE_QML_REST_API_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QObject>
#include <QQmlParserStatus>
#include <QtQml/QJSValue>
#include <QtQml>
/* ----------------------------------- Local -------------------------------- */
#include "client.h"
#include "reply.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest {
class IApi;
}  // namespace egnite::rest

/* ----------------------------------- QmlApi ------------------------------- */

class QmlApi : public QObject, public QQmlParserStatus {
  Q_OBJECT
  QML_ELEMENT
  Q_INTERFACES(QQmlParserStatus)

  Q_PROPERTY(QString path READ getPath WRITE setPath NOTIFY pathChanged)
  Q_PROPERTY(
      QmlClient* client READ getClient WRITE setClient NOTIFY clientChanged)

  Q_PROPERTY(QJSValue globalHeaders READ getGlobalQmlHeaders WRITE
                 setGlobalQmlHeaders NOTIFY globalQmlHeadersChanged)
  Q_PROPERTY(QJSValue globalParameters READ getGlobalQmlParameters WRITE
                 setGlobalQmlParameters NOTIFY globalQmlParametersChanged)

 public:
  explicit QmlApi(QObject* parent = nullptr);
  ~QmlApi() override;

  void setPath(const QString& path);
  [[nodiscard]] QString getPath() const;

  void setClient(QmlClient* client);
  [[nodiscard]] QmlClient* getClient() const;

  [[nodiscard]] QJSValue getGlobalQmlHeaders() const;
  [[nodiscard]] QJSValue getGlobalQmlParameters() const;

  void setGlobalQmlHeaders(QJSValue object);
  void setGlobalQmlParameters(QJSValue object);

  void classBegin() override;
  void componentComplete() override;

  Q_INVOKABLE [[nodiscard]] QmlReply* get(QJSValue path,
                                          QJSValue parameters = {},
                                          QJSValue headers = {}) const;
  Q_INVOKABLE [[nodiscard]] QmlReply* head(QJSValue path,
                                           QJSValue parameters = {},
                                           QJSValue headers = {}) const;
  Q_INVOKABLE [[nodiscard]] QmlReply* deleteResource(
      QJSValue path, QJSValue parameters = {}, QJSValue headers = {}) const;
  Q_INVOKABLE [[nodiscard]] QmlReply* post(QJSValue path, QJSValue data,
                                           QJSValue parameters = {},
                                           QJSValue headers = {}) const;
  Q_INVOKABLE [[nodiscard]] QmlReply* put(QJSValue path, QJSValue data,
                                          QJSValue parameters = {},
                                          QJSValue headers = {}) const;
  Q_INVOKABLE [[nodiscard]] QmlReply* patch(QJSValue path, QJSValue data,
                                            QJSValue parameters = {},
                                            QJSValue headers = {}) const;

 Q_SIGNALS:
  void pathChanged(const QString& path);
  void clientChanged(QmlClient* client);
  void globalQmlHeadersChanged(const QJSValue& object);
  void globalQmlParametersChanged(const QJSValue& object);

 private:
  void revaluateApi();
  QmlReply* createQmlReply(egnite::rest::IReply* reply) const;
  QmlReply* callImpl(const QByteArray& verb, const QJSValue& path,
                     const QJSValue& parameters, const QJSValue& headers,
                     const QJSValue& data = {}) const;

  std::optional<QString> getPath(const QJSValue& object) const;
  std::optional<egnite::rest::Data> getBody(const QJSValue& object) const;
  std::optional<QUrlQuery> getParameters(const QJSValue& object) const;
  std::optional<egnite::rest::Headers> getHeaders(const QJSValue& object) const;

 private:
  struct RevaluateData {
    bool init = false;
    QString path = "";
    QmlClient* client = nullptr;
  };

 private:
  RevaluateData m_revaluate_data;
  egnite::rest::IApi* m_api;
};

#endif  // EGNITE_QML_REST_API_H