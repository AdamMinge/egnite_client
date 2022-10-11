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
#include "egnite/rest/global.h"
#include "egnite/rest/reply.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

namespace detail {
class ApiPrivate;
}

class IClient;
class IReplyDecorator;

/* ----------------------------------- IApi --------------------------------- */

class EGNITE_REST_API IApi : public QObject {
  Q_OBJECT

 public:
  ~IApi() override;

  [[nodiscard]] virtual IClient* getClient() const = 0;
  [[nodiscard]] virtual IReplyDecorator* getReplyDecorator() const = 0;
  [[nodiscard]] virtual DataSerializer* getDataSerializer() const = 0;

  [[nodiscard]] virtual IApi* createSubApi(const QString& path,
                                           QObject* parent = nullptr) = 0;

  [[nodiscard]] virtual IReply* get(const QString& path,
                                    const QUrlQuery& parameters = {},
                                    const Headers& headers = {},
                                    QObject* parent = nullptr) const = 0;
  [[nodiscard]] virtual IReply* head(const QString& path,
                                     const QUrlQuery& parameters = {},
                                     const Headers& headers = {},
                                     QObject* parent = nullptr) const = 0;
  [[nodiscard]] virtual IReply* deleteResource(
      const QString& path, const QUrlQuery& parameters = {},
      const Headers& headers = {}, QObject* parent = nullptr) const = 0;

  [[nodiscard]] virtual IReply* post(const QString& path,
                                     const QUrlQuery& parameters = {},
                                     const Headers& headers = {},
                                     QObject* parent = nullptr) const = 0;
  [[nodiscard]] virtual IReply* post(const QString& path,
                                     const QJsonValue& data,
                                     const QUrlQuery& parameters = {},
                                     const Headers& headers = {},
                                     QObject* parent = nullptr) const = 0;
  [[nodiscard]] virtual IReply* post(const QString& path,
                                     const QCborValue& data,
                                     const QUrlQuery& parameters = {},
                                     const Headers& headers = {},
                                     QObject* parent = nullptr) const = 0;

  [[nodiscard]] virtual IReply* put(const QString& path,
                                    const QUrlQuery& parameters = {},
                                    const Headers& headers = {},
                                    QObject* parent = nullptr) const = 0;
  [[nodiscard]] virtual IReply* put(const QString& path, const QJsonValue& data,
                                    const QUrlQuery& parameters = {},
                                    const Headers& headers = {},
                                    QObject* parent = nullptr) const = 0;
  [[nodiscard]] virtual IReply* put(const QString& path, const QCborValue& data,
                                    const QUrlQuery& parameters = {},
                                    const Headers& headers = {},
                                    QObject* parent = nullptr) const = 0;

  [[nodiscard]] virtual IReply* patch(const QString& path,
                                      const QUrlQuery& parameters = {},
                                      const Headers& headers = {},
                                      QObject* parent = nullptr) const = 0;
  [[nodiscard]] virtual IReply* patch(const QString& path,
                                      const QJsonValue& data,
                                      const QUrlQuery& parameters = {},
                                      const Headers& headers = {},
                                      QObject* parent = nullptr) const = 0;
  [[nodiscard]] virtual IReply* patch(const QString& path,
                                      const QCborValue& data,
                                      const QUrlQuery& parameters = {},
                                      const Headers& headers = {},
                                      QObject* parent = nullptr) const = 0;

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
  [[nodiscard]] virtual DataSerializer::Format getRequestDataFormat(
      const Headers& headers) const = 0;

 protected:
  explicit IApi(QObject* parent = nullptr);
  explicit IApi(QObjectPrivate& impl, QObject* parent = nullptr);
};

template <typename DataType, typename ErrorType>
GenericReply<DataType, ErrorType>* IApi::get(const QString& path,
                                             const QUrlQuery& parameters,
                                             const Headers& headers,
                                             QObject* parent) {
  return new GenericReply<DataType, ErrorType>(get(path, parameters, headers),
                                               parent);
}

template <typename DataType, typename ErrorType>
GenericReply<DataType, ErrorType>* IApi::head(const QString& path,
                                              const QUrlQuery& parameters,
                                              const Headers& headers,
                                              QObject* parent) {
  return new GenericReply<DataType, ErrorType>(head(path, parameters, headers),
                                               parent);
}

template <typename DataType, typename ErrorType>
GenericReply<DataType, ErrorType>* IApi::deleteResource(
    const QString& path, const QUrlQuery& parameters, const Headers& headers,
    QObject* parent) {
  return new GenericReply<DataType, ErrorType>(
      deleteResource(path, parameters, headers), parent);
}

template <typename DataType, typename ErrorType>
GenericReply<DataType, ErrorType>* IApi::post(const QString& path,
                                              const QUrlQuery& parameters,
                                              const Headers& headers,
                                              QObject* parent) {
  return new GenericReply<DataType, ErrorType>(post(path, parameters, headers),
                                               parent);
}

template <typename DataType, typename ErrorType, typename RequestDataType>
GenericReply<DataType, ErrorType>* IApi::post(const QString& path,
                                              const RequestDataType& data,
                                              const QUrlQuery& parameters,
                                              const Headers& headers,
                                              QObject* parent) {
  auto serialized_data =
      getDataSerializer()->serialize(data, getRequestDataFormat(headers));
  auto reply = std::visit(
      core::utils::overloaded{[&](std::nullopt_t) -> IReply* {
                                return post(path, parameters, headers);
                              },
                              [&](QJsonValue& data) -> IReply* {
                                return post(path, data, parameters, headers);
                              },
                              [&](QCborValue& data) -> IReply* {
                                return post(path, data, parameters, headers);
                              }},
      serialized_data);
  return new GenericReply<DataType, ErrorType>(reply, parent);
}

template <typename DataType, typename ErrorType>
GenericReply<DataType, ErrorType>* IApi::put(const QString& path,
                                             const QUrlQuery& parameters,
                                             const Headers& headers,
                                             QObject* parent) {
  return new GenericReply<DataType, ErrorType>(put(path, parameters, headers),
                                               parent);
}

template <typename DataType, typename ErrorType, typename RequestDataType>
GenericReply<DataType, ErrorType>* IApi::put(const QString& path,
                                             const RequestDataType& data,
                                             const QUrlQuery& parameters,
                                             const Headers& headers,
                                             QObject* parent) {
  auto serialized_data =
      getDataSerializer()->serialize(data, getRequestDataFormat(headers));
  auto reply = std::visit(
      core::utils::overloaded{[&](std::nullopt_t) -> IReply* {
                                return put(path, parameters, headers);
                              },
                              [&](QJsonValue& data) -> IReply* {
                                return put(path, data, parameters, headers);
                              },
                              [&](QCborValue& data) -> IReply* {
                                return put(path, data, parameters, headers);
                              }},
      serialized_data);

  return new GenericReply<DataType, ErrorType>(reply, parent);
}

template <typename DataType, typename ErrorType>
GenericReply<DataType, ErrorType>* IApi::patch(const QString& path,
                                               const QUrlQuery& parameters,
                                               const Headers& headers,
                                               QObject* parent) {
  return new GenericReply<DataType, ErrorType>(patch(path, parameters, headers),
                                               parent);
}

template <typename DataType, typename ErrorType, typename RequestDataType>
GenericReply<DataType, ErrorType>* IApi::patch(const QString& path,
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

/* ------------------------------------ Api --------------------------------- */

class EGNITE_REST_API Api : public IApi {
  Q_OBJECT

 public:
  explicit Api(IClient* client, QNetworkAccessManager* manager,
               const QString& subpath, QObject* parent = nullptr);
  ~Api() override;

  [[nodiscard]] IClient* getClient() const override;
  [[nodiscard]] IReplyDecorator* getReplyDecorator() const override;
  [[nodiscard]] DataSerializer* getDataSerializer() const override;

  [[nodiscard]] IApi* createSubApi(const QString& path,
                                   QObject* parent = nullptr) override;

  [[nodiscard]] IReply* get(const QString& path,
                            const QUrlQuery& parameters = {},
                            const Headers& headers = {},
                            QObject* parent = nullptr) const override;
  [[nodiscard]] IReply* head(const QString& path,
                             const QUrlQuery& parameters = {},
                             const Headers& headers = {},
                             QObject* parent = nullptr) const override;
  [[nodiscard]] IReply* deleteResource(
      const QString& path, const QUrlQuery& parameters = {},
      const Headers& headers = {}, QObject* parent = nullptr) const override;

  [[nodiscard]] IReply* post(const QString& path,
                             const QUrlQuery& parameters = {},
                             const Headers& headers = {},
                             QObject* parent = nullptr) const override;
  [[nodiscard]] IReply* post(const QString& path, const QJsonValue& data,
                             const QUrlQuery& parameters = {},
                             const Headers& headers = {},
                             QObject* parent = nullptr) const override;
  [[nodiscard]] IReply* post(const QString& path, const QCborValue& data,
                             const QUrlQuery& parameters = {},
                             const Headers& headers = {},
                             QObject* parent = nullptr) const override;

  [[nodiscard]] IReply* put(const QString& path,
                            const QUrlQuery& parameters = {},
                            const Headers& headers = {},
                            QObject* parent = nullptr) const override;
  [[nodiscard]] IReply* put(const QString& path, const QJsonValue& data,
                            const QUrlQuery& parameters = {},
                            const Headers& headers = {},
                            QObject* parent = nullptr) const override;
  [[nodiscard]] IReply* put(const QString& path, const QCborValue& data,
                            const QUrlQuery& parameters = {},
                            const Headers& headers = {},
                            QObject* parent = nullptr) const override;

  [[nodiscard]] IReply* patch(const QString& path,
                              const QUrlQuery& parameters = {},
                              const Headers& headers = {},
                              QObject* parent = nullptr) const override;
  [[nodiscard]] IReply* patch(const QString& path, const QJsonValue& data,
                              const QUrlQuery& parameters = {},
                              const Headers& headers = {},
                              QObject* parent = nullptr) const override;
  [[nodiscard]] IReply* patch(const QString& path, const QCborValue& data,
                              const QUrlQuery& parameters = {},
                              const Headers& headers = {},
                              QObject* parent = nullptr) const override;

 protected:
  [[nodiscard]] DataSerializer::Format getRequestDataFormat(
      const Headers& headers) const override;

 protected:
  explicit Api(detail::ApiPrivate& impl, QObject* parent = nullptr);

 private:
  Q_DECLARE_PRIVATE(detail::Api);
};

}  // namespace egnite::rest

#endif  // EGNITE_REST_API_H