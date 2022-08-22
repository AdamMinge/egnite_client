#ifndef EGNITE_REST_CLIENT_H
#define EGNITE_REST_CLIENT_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QObject>
#include <QUrl>
#include <QUrlQuery>
#include <QVersionNumber>
/* ----------------------------------- Local -------------------------------- */
#include "egnite/export.h"
#include "egnite/rest/rest_global.h"
#include "egnite/rest/rest_request_builder.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

namespace detail {
class RestClientPrivate;
}

class RestApi;
class RestDataSerializer;

class EGNITE_API RestClient : public QObject {
  Q_OBJECT

 public:
  Q_PROPERTY(
      QUrl baseUrl READ getBaseUrl WRITE setBaseUrl NOTIFY baseUrlChanged)
  Q_PROPERTY(QVersionNumber version READ getVersion WRITE setVersion NOTIFY
                 versionChanged)
  Q_PROPERTY(RestHeaders globalHeaders READ getGlobalHeaders WRITE
                 setGlobalHeaders NOTIFY globalHeadersChanged)
  Q_PROPERTY(QUrlQuery globalParameters READ getGlobalParameters WRITE
                 setGlobalParameters NOTIFY globalParametersChanged)

 public:
  RestClient(QObject* parent = nullptr);
  ~RestClient() override;

  [[nodiscard]] RestApi* createApi(const QString& path,
                                   QObject* parent = nullptr);

  void setBaseUrl(const QUrl& url);
  [[nodiscard]] QUrl getBaseUrl() const;

  void setVersion(const QVersionNumber& version);
  [[nodiscard]] QVersionNumber getVersion() const;

  void setGlobalHeaders(const RestHeaders& headers);
  [[nodiscard]] RestHeaders getGlobalHeaders() const;

  void setGlobalParameters(const QUrlQuery& parameters);
  [[nodiscard]] QUrlQuery getGlobalParameters() const;

  [[nodiscard]] RestRequestBuilder getRequestBuilder() const;
  [[nodiscard]] RestDataSerializer* getDataSerializer() const;

 Q_SIGNALS:
  void baseUrlChanged(const QUrl& url);
  void versionChanged(const QVersionNumber& version);
  void globalHeadersChanged(const egnite::rest::RestHeaders& headers);
  void globalParametersChanged(const QUrlQuery& parameters);

 protected:
  RestClient(detail::RestClientPrivate& impl, QObject* parent = nullptr);

 private:
  Q_DECLARE_PRIVATE(detail::RestClient);
};

}  // namespace egnite::rest

#endif  // EGNITE_REST_CLIENT_H