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

class IApi;
class IReplyDecorator;

class DataSerializer;

/* ---------------------------------- IClient ------------------------------- */

class EGNITE_REST_API IClient : public QObject {
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
  ~IClient() override;

  [[nodiscard]] virtual IApi* createApi(const QString& path,
                                        QObject* parent = nullptr) = 0;

  virtual void setBaseUrl(const QUrl& url) = 0;
  [[nodiscard]] virtual QUrl getBaseUrl() const = 0;

  virtual void setVersion(const QVersionNumber& version) = 0;
  [[nodiscard]] virtual QVersionNumber getVersion() const = 0;

  virtual void setGlobalHeaders(const Headers& headers) = 0;
  [[nodiscard]] virtual Headers getGlobalHeaders() const = 0;

  virtual void setGlobalParameters(const QUrlQuery& parameters) = 0;
  [[nodiscard]] virtual QUrlQuery getGlobalParameters() const = 0;

  [[nodiscard]] virtual RequestBuilder getRequestBuilder() const = 0;
  [[nodiscard]] virtual DataSerializer* getDataSerializer() const = 0;
  [[nodiscard]] virtual IReplyDecorator* getReplyDecorator() const = 0;

 Q_SIGNALS:
  void baseUrlChanged(const QUrl& url);
  void versionChanged(const QVersionNumber& version);
  void globalHeadersChanged(const egnite::rest::Headers& headers);
  void globalParametersChanged(const QUrlQuery& parameters);

 protected:
  explicit IClient(QObject* parent = nullptr);
  explicit IClient(QObjectPrivate& impl, QObject* parent = nullptr);
};

/* ---------------------------------- Client -------------------------------- */

class EGNITE_REST_API Client : public IClient {
  Q_OBJECT

 public:
  explicit Client(QObject* parent = nullptr);
  ~Client() override;

  [[nodiscard]] IApi* createApi(const QString& path,
                                QObject* parent = nullptr) override;

  void setBaseUrl(const QUrl& url) override;
  [[nodiscard]] QUrl getBaseUrl() const override;

  void setVersion(const QVersionNumber& version) override;
  [[nodiscard]] QVersionNumber getVersion() const override;

  void setGlobalHeaders(const Headers& headers) override;
  [[nodiscard]] Headers getGlobalHeaders() const override;

  void setGlobalParameters(const QUrlQuery& parameters) override;
  [[nodiscard]] QUrlQuery getGlobalParameters() const override;

  [[nodiscard]] RequestBuilder getRequestBuilder() const override;
  [[nodiscard]] DataSerializer* getDataSerializer() const override;
  [[nodiscard]] IReplyDecorator* getReplyDecorator() const override;

 protected:
  explicit Client(detail::ClientPrivate& impl, QObject* parent = nullptr);

 private:
  Q_DECLARE_PRIVATE(detail::Client);
};

}  // namespace egnite::rest

#endif  // EGNITE_REST_CLIENT_H