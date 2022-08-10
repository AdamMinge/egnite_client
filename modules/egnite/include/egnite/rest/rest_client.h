#ifndef EGNITE_REST_CLIENT_H
#define EGNITE_REST_CLIENT_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QObject>
#include <QUrl>
#include <QVersionNumber>
/* ----------------------------------- Local -------------------------------- */
#include "egnite/export.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

class RestApi;

class EGNITE_API RestClient : public QObject {
  Q_OBJECT

 public:
  Q_PROPERTY(
      QUrl baseUrl READ getBaseUrl WRITE setBaseUrl NOTIFY baseUrlChanged)
  Q_PROPERTY(QVersionNumber version READ getVersion WRITE setVersion NOTIFY
                 versionChanged)

 public:
  RestClient(QObject* parent = nullptr);
  ~RestClient() override;

  [[nodiscard]] RestApi* createApi(const QString& path);

  void setBaseUrl(const QUrl& url);
  [[nodiscard]] QUrl getBaseUrl() const;

  void setVersion(const QVersionNumber& version);
  [[nodiscard]] QVersionNumber getVersion() const;

 Q_SIGNALS:
  void baseUrlChanged(const QUrl& url);
  void versionChanged(const QVersionNumber& version);

 private:
  QUrl m_base_url;
  QVersionNumber m_version;
};

}  // namespace egnite::rest

#endif  // EGNITE_REST_CLIENT_H