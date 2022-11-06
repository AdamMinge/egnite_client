#ifndef EGNITE_QML_REST_API_H
#define EGNITE_QML_REST_API_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QObject>
#include <QQmlParserStatus>
#include <QtQml>
/* ----------------------------------- Local -------------------------------- */
#include "client.h"
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

 Q_SIGNALS:
  void pathChanged(const QString& path);
  void clientChanged(QmlClient* client);
  void globalQmlHeadersChanged(const QJSValue& object);
  void globalQmlParametersChanged(const QJSValue& object);

 private:
  void revaluateApi();

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