#ifndef EGNITE_REST_API_H
#define EGNITE_REST_API_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QObject>
#include <QString>
/* ----------------------------------- Local -------------------------------- */
#include "egnite/export.h"
#include "egnite/rest/rest_client.h"
#include "egnite/rest/rest_reply.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

namespace detail {
class RestApiPrivate;
}

class EGNITE_API RestApi : public QObject {
  Q_OBJECT

  friend RestClient;

 public:
  ~RestApi() override;

  [[nodiscard]] RestClient* getClient() const;
  [[nodiscard]] RestApi* createSubApi(const QString& path,
                                      QObject* parent = nullptr);

  [[nodiscard]] RestReply* get(const QString& path,
                               const QUrlQuery& parameters = {},
                               const RestClient::Headers& headers = {}) const;
  [[nodiscard]] RestReply* head(const QString& path,
                                const QUrlQuery& parameters = {},
                                const RestClient::Headers& headers = {}) const;
  [[nodiscard]] RestReply* deleteResource(
      const QString& path, const QUrlQuery& parameters = {},
      const RestClient::Headers& headers = {}) const;

  [[nodiscard]] RestReply* post(const QString& path,
                                const QUrlQuery& parameters = {},
                                const RestClient::Headers& headers = {}) const;
  [[nodiscard]] RestReply* post(const QString& path, const QJsonValue& data,
                                const QUrlQuery& parameters = {},
                                const RestClient::Headers& headers = {}) const;
  [[nodiscard]] RestReply* post(const QString& path, const QCborValue& data,
                                const QUrlQuery& parameters = {},
                                const RestClient::Headers& headers = {}) const;

  [[nodiscard]] RestReply* put(const QString& path,
                               const QUrlQuery& parameters = {},
                               const RestClient::Headers& headers = {}) const;
  [[nodiscard]] RestReply* put(const QString& path, const QJsonValue& data,
                               const QUrlQuery& parameters = {},
                               const RestClient::Headers& headers = {}) const;
  [[nodiscard]] RestReply* put(const QString& path, const QCborValue& data,
                               const QUrlQuery& parameters = {},
                               const RestClient::Headers& headers = {}) const;

  [[nodiscard]] RestReply* patch(const QString& path,
                                 const QUrlQuery& parameters = {},
                                 const RestClient::Headers& headers = {}) const;
  [[nodiscard]] RestReply* patch(const QString& path, const QJsonValue& data,
                                 const QUrlQuery& parameters = {},
                                 const RestClient::Headers& headers = {}) const;
  [[nodiscard]] RestReply* patch(const QString& path, const QCborValue& data,
                                 const QUrlQuery& parameters = {},
                                 const RestClient::Headers& headers = {}) const;

 protected:
  RestApi(RestClient* client, const QString& subpath,
          QObject* parent = nullptr);
  RestApi(detail::RestApiPrivate& impl, QObject* parent = nullptr);

 private:
  Q_DECLARE_PRIVATE(detail::RestApi);
};

}  // namespace egnite::rest

#endif  // EGNITE_REST_API_H