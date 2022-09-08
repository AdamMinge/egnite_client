#ifndef EGNITE_QML_REST_API_H
#define EGNITE_QML_REST_API_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QObject>
#include <QPointer>
#include <QQmlParserStatus>
#include <QtQml>
/* -------------------------------------------------------------------------- */

namespace egnite::rest {
class Api;
}

class QmlApi : public QObject, public QQmlParserStatus {
  Q_OBJECT
  QML_ELEMENT
  Q_INTERFACES(QQmlParserStatus)

  Q_PROPERTY(QString path READ getPath WRITE setPath NOTIFY pathChanged)

 public:
  explicit QmlApi(QObject* parent = nullptr);
  ~QmlApi() override;

  void classBegin() override;
  void componentComplete() override;

  void setPath(const QString& path);
  [[nodiscard]] QString getPath() const;

 Q_SIGNALS:
  void pathChanged(const QString& path);

 private:
  void revaluateApi();

 private:
  bool m_init;
  QString m_path;
  QPointer<egnite::rest::Api> m_api;
};

#endif  // EGNITE_QML_REST_API_H