#ifndef EGNITE_REST_API_H
#define EGNITE_REST_API_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>
#include <QString>
#include <QUrlQuery>
/* ----------------------------------- Local -------------------------------- */
#include "egnite/rest/export.h"
#include "egnite/rest/generic_reply.h"
#include "egnite/rest/global.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

namespace detail {
class ApiPrivate;
}

class Client;
class ReplyDecoratorManager;

class EGNITE_REST_API Api : public QObject {
  Q_OBJECT

  friend Client;

 public:
  ~Api() override;

  [[nodiscard]] Client* getClient() const;
  [[nodiscard]] DataSerializer* getDataSerializer() const;
  [[nodiscard]] ReplyDecoratorManager* getReplyDecoratorManager() const;

  [[nodiscard]] Api* createSubApi(const QString& path,
                                  QObject* parent = nullptr);

  /* -------------------------------- Raw Api ----------------------------- */

  [[nodiscard]] Reply* get(const QString& path,
                           const QUrlQuery& parameters = {},
                           const Headers& headers = {},
                           QObject* parent = nullptr) const;
  [[nodiscard]] Reply* head(const QString& path,
                            const QUrlQuery& parameters = {},
                            const Headers& headers = {},
                            QObject* parent = nullptr) const;
  [[nodiscard]] Reply* deleteResource(const QString& path,
                                      const QUrlQuery& parameters = {},
                                      const Headers& headers = {},
                                      QObject* parent = nullptr) const;

  [[nodiscard]] Reply* post(const QString& path,
                            const QUrlQuery& parameters = {},
                            const Headers& headers = {},
                            QObject* parent = nullptr) const;
  [[nodiscard]] Reply* post(const QString& path, const QJsonValue& data,
                            const QUrlQuery& parameters = {},
                            const Headers& headers = {},
                            QObject* parent = nullptr) const;
  [[nodiscard]] Reply* post(const QString& path, const QCborValue& data,
                            const QUrlQuery& parameters = {},
                            const Headers& headers = {},
                            QObject* parent = nullptr) const;

  [[nodiscard]] Reply* put(const QString& path,
                           const QUrlQuery& parameters = {},
                           const Headers& headers = {},
                           QObject* parent = nullptr) const;
  [[nodiscard]] Reply* put(const QString& path, const QJsonValue& data,
                           const QUrlQuery& parameters = {},
                           const Headers& headers = {},
                           QObject* parent = nullptr) const;
  [[nodiscard]] Reply* put(const QString& path, const QCborValue& data,
                           const QUrlQuery& parameters = {},
                           const Headers& headers = {},
                           QObject* parent = nullptr) const;

  [[nodiscard]] Reply* patch(const QString& path,
                             const QUrlQuery& parameters = {},
                             const Headers& headers = {},
                             QObject* parent = nullptr) const;
  [[nodiscard]] Reply* patch(const QString& path, const QJsonValue& data,
                             const QUrlQuery& parameters = {},
                             const Headers& headers = {},
                             QObject* parent = nullptr) const;
  [[nodiscard]] Reply* patch(const QString& path, const QCborValue& data,
                             const QUrlQuery& parameters = {},
                             const Headers& headers = {},
                             QObject* parent = nullptr) const;

  /* ------------------------------ Generic Api --------------------------- */

  template <typename DataType, typename ErrorType>
  [[nodiscard]] GenericReply<DataType, ErrorType>* get(
      const QString& path, const QUrlQuery& parameters = {},
      const Headers& headers = {}, QObject* parent = nullptr);

  template <typename DataType, typename ErrorType>
  [[nodiscard]] GenericReply<DataType, ErrorType>* head(
      const QString& path, const QUrlQuery& parameters = {},
      const Headers& headers = {}, QObject* parent = nullptr);

  template <typename DataType, typename ErrorType>
  [[nodiscard]] GenericReply<DataType, ErrorType>* deleteResource(
      const QString& path, const QUrlQuery& parameters = {},
      const Headers& headers = {}, QObject* parent = nullptr);

  template <typename DataType, typename ErrorType>
  [[nodiscard]] GenericReply<DataType, ErrorType>* post(
      const QString& path, const QUrlQuery& parameters = {},
      const Headers& headers = {}, QObject* parent = nullptr);

  template <typename DataType, typename ErrorType, typename RequestDataType>
  [[nodiscard]] GenericReply<DataType, ErrorType>* post(
      const QString& path, const RequestDataType& data,
      const QUrlQuery& parameters = {}, const Headers& headers = {},
      QObject* parent = nullptr);

  template <typename DataType, typename ErrorType>
  [[nodiscard]] GenericReply<DataType, ErrorType>* put(
      const QString& path, const QUrlQuery& parameters = {},
      const Headers& headers = {}, QObject* parent = nullptr);

  template <typename DataType, typename ErrorType, typename RequestDataType>
  [[nodiscard]] GenericReply<DataType, ErrorType>* put(
      const QString& path, const RequestDataType& data,
      const QUrlQuery& parameters = {}, const Headers& headers = {},
      QObject* parent = nullptr);

  template <typename DataType, typename ErrorType>
  [[nodiscard]] GenericReply<DataType, ErrorType>* patch(
      const QString& path, const QUrlQuery& parameters = {},
      const Headers& headers = {}, QObject* parent = nullptr);

  template <typename DataType, typename ErrorType, typename RequestDataType>
  [[nodiscard]] GenericReply<DataType, ErrorType>* patch(
      const QString& path, const RequestDataType& data,
      const QUrlQuery& parameters = {}, const Headers& headers = {},
      QObject* parent = nullptr);

 protected:
  explicit Api(Client* client, QNetworkAccessManager* manager,
               const QString& subpath, QObject* parent = nullptr);
  explicit Api(detail::ApiPrivate& impl, QObject* parent = nullptr);

  [[nodiscard]] DataSerializer::Format getRequestDataFormat(
      const Headers& headers) const;

 private:
  Q_DECLARE_PRIVATE(detail::Api);
};

template <typename DataType, typename ErrorType>
GenericReply<DataType, ErrorType>* Api::get(const QString& path,
                                            const QUrlQuery& parameters,
                                            const Headers& headers,
                                            QObject* parent) {
  return new GenericReply<DataType, ErrorType>(get(path, parameters, headers),
                                               parent);
}

template <typename DataType, typename ErrorType>
GenericReply<DataType, ErrorType>* Api::head(const QString& path,
                                             const QUrlQuery& parameters,
                                             const Headers& headers,
                                             QObject* parent) {
  return new GenericReply<DataType, ErrorType>(head(path, parameters, headers),
                                               parent);
}

template <typename DataType, typename ErrorType>
GenericReply<DataType, ErrorType>* Api::deleteResource(
    const QString& path, const QUrlQuery& parameters, const Headers& headers,
    QObject* parent) {
  return new GenericReply<DataType, ErrorType>(
      deleteResource(path, parameters, headers), parent);
}

template <typename DataType, typename ErrorType>
GenericReply<DataType, ErrorType>* Api::post(const QString& path,
                                             const QUrlQuery& parameters,
                                             const Headers& headers,
                                             QObject* parent) {
  return new GenericReply<DataType, ErrorType>(post(path, parameters, headers),
                                               parent);
}

template <typename DataType, typename ErrorType, typename RequestDataType>
GenericReply<DataType, ErrorType>* Api::post(const QString& path,
                                             const RequestDataType& data,
                                             const QUrlQuery& parameters,
                                             const Headers& headers,
                                             QObject* parent) {
  auto serialized_data =
      getDataSerializer()->serialize(data, getRequestDataFormat(headers));
  auto reply = std::visit(
      core::utils::overloaded{[&](std::nullopt_t) -> Reply* {
                                return post(path, parameters, headers);
                              },
                              [&](QJsonValue& data) -> Reply* {
                                return post(path, data, parameters, headers);
                              },
                              [&](QCborValue& data) -> Reply* {
                                return post(path, data, parameters, headers);
                              }},
      serialized_data);
  return new GenericReply<DataType, ErrorType>(reply, parent);
}

template <typename DataType, typename ErrorType>
GenericReply<DataType, ErrorType>* Api::put(const QString& path,
                                            const QUrlQuery& parameters,
                                            const Headers& headers,
                                            QObject* parent) {
  return new GenericReply<DataType, ErrorType>(put(path, parameters, headers),
                                               parent);
}

template <typename DataType, typename ErrorType, typename RequestDataType>
GenericReply<DataType, ErrorType>* Api::put(const QString& path,
                                            const RequestDataType& data,
                                            const QUrlQuery& parameters,
                                            const Headers& headers,
                                            QObject* parent) {
  auto serialized_data =
      getDataSerializer()->serialize(data, getRequestDataFormat(headers));
  auto reply = std::visit(
      core::utils::overloaded{[&](std::nullopt_t) -> Reply* {
                                return put(path, parameters, headers);
                              },
                              [&](QJsonValue& data) -> Reply* {
                                return put(path, data, parameters, headers);
                              },
                              [&](QCborValue& data) -> Reply* {
                                return put(path, data, parameters, headers);
                              }},
      serialized_data);

  return new GenericReply<DataType, ErrorType>(reply, parent);
}

template <typename DataType, typename ErrorType>
GenericReply<DataType, ErrorType>* Api::patch(const QString& path,
                                              const QUrlQuery& parameters,
                                              const Headers& headers,
                                              QObject* parent) {
  return new GenericReply<DataType, ErrorType>(patch(path, parameters, headers),
                                               parent);
}

template <typename DataType, typename ErrorType, typename RequestDataType>
GenericReply<DataType, ErrorType>* Api::patch(const QString& path,
                                              const RequestDataType& data,
                                              const QUrlQuery& parameters,
                                              const Headers& headers,
                                              QObject* parent) {
  return new GenericReply<DataType, ErrorType>(
      patch(path,
            getDataSerializer()->serialize(data, getRequestDataFormat(headers)),
            parameters, headers),
      parent);
}

}  // namespace egnite::rest

#endif  // EGNITE_REST_API_H