#ifndef EGNITE_QML_REST_API_H
#define EGNITE_QML_REST_API_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QObject>
#include <QQmlParserStatus>
#include <QtQml>
/* ---------------------------------- Standard ------------------------------ */
#include <memory>
/* -------------------------------------------------------------------------- */

class QmlApi : public QObject, public QQmlParserStatus {
  Q_OBJECT
  QML_ELEMENT
  Q_INTERFACES(QQmlParserStatus)

 public:
  explicit QmlApi(QObject* parent = nullptr);
  ~QmlApi() override;

  void classBegin() override;
  void componentComplete() override;
};

#endif  // EGNITE_QML_REST_API_H