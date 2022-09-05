#ifndef EGNITE_REST_CLIENT_H
#define EGNITE_REST_CLIENT_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QObject>
#include <QUrl>
#include <QUrlQuery>
#include <QVersionNumber>
/* ----------------------------------- Local -------------------------------- */
#include "egnite/rest/export.h"
#include "egnite/rest/global.h"
#include "egnite/rest/request_builder.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

namespace detail {
class ClientPrivate;
}

class Api;
class DataSerializer;
class ReplyDecoratorManager;

class EGNITE_REST_API Client : public QObject {
  Q_OBJECT

 public:
  Q_PROPERTY(
      QUrl baseUrl READ getBaseUrl WRITE setBaseUrl NOTIFY baseUrlChanged)
  Q_PROPERTY(QVersionNumber version READ getVersion WRITE setVersion NOTIFY
                 versionChanged)
  Q_PROPERTY(Headers globalHeaders READ getGlobalHeaders WRITE setGlobalHeaders
                 NOTIFY globalHeadersChanged)
  Q_PROPERTY(QUrlQuery globalParameters READ getGlobalParameters WRITE
                 setGlobalParameters NOTIFY globalParametersChanged)

 public:
  explicit Client(QObject* parent = nullptr);
  ~Client() override;

  [[nodiscard]] Api* createApi(const QString& path, QObject* parent = nullptr);

  void setBaseUrl(const QUrl& url);
  [[nodiscard]] QUrl getBaseUrl() const;

  void setVersion(const QVersionNumber& version);
  [[nodiscard]] QVersionNumber getVersion() const;

  void setGlobalHeaders(const Headers& headers);
  [[nodiscard]] Headers getGlobalHeaders() const;

  void setGlobalParameters(const QUrlQuery& parameters);
  [[nodiscard]] QUrlQuery getGlobalParameters() const;

  [[nodiscard]] RequestBuilder getRequestBuilder() const;
  [[nodiscard]] DataSerializer* getDataSerializer() const;
  [[nodiscard]] ReplyDecoratorManager* getReplyDecoratorManager() const;

 Q_SIGNALS:
  void baseUrlChanged(const QUrl& url);
  void versionChanged(const QVersionNumber& version);
  void globalHeadersChanged(const egnite::rest::Headers& headers);
  void globalParametersChanged(const QUrlQuery& parameters);

 protected:
  explicit Client(detail::ClientPrivate& impl, QObject* parent = nullptr);

 private:
  Q_DECLARE_PRIVATE(detail::Client);
};

}  // namespace egnite::rest

#endif  // EGNITE_REST_CLIENT_H