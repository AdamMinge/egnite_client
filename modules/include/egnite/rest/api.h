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
class IPagingDataFactory;

/* ----------------------------------- IApi --------------------------------- */

class EGNITE_REST_API IApi : public QObject {
  Q_OBJECT

 public:
  Q_PROPERTY(Headers globalHeaders READ getGlobalHeaders WRITE setGlobalHeaders
                 NOTIFY globalHeadersChanged)
  Q_PROPERTY(QUrlQuery globalParameters READ getGlobalParameters WRITE
                 setGlobalParameters NOTIFY globalParametersChanged)

 public:
  static const QByteArray GetVerb;
  static const QByteArray PostVerb;
  static const QByteArray PutVerb;
  static const QByteArray PatchVerb;
  static const QByteArray DeleteVerb;
  static const QByteArray HeadVerb;

 public:
  ~IApi() override;

  [[nodiscard]] virtual IClient* getClient() const = 0;
  [[nodiscard]] virtual IReplyDecorator* getReplyDecorator() const = 0;
  [[nodiscard]] virtual DataSerializer* getDataSerializer() const = 0;
  [[nodiscard]] virtual IPagingDataFactory* getPagingDataFactory() const = 0;

  virtual void setGlobalHeaders(const Headers& headers) = 0;
  [[nodiscard]] virtual Headers getGlobalHeaders() const = 0;

  virtual void setGlobalParameters(const QUrlQuery& parameters) = 0;
  [[nodiscard]] virtual QUrlQuery getGlobalParameters() const = 0;

  [[nodiscard]] virtual DataSerializer::Format getRequestDataFormat(
      const Headers& headers = Headers{}) const = 0;

  [[nodiscard]] virtual IApi* createSubApi(const QString& path,
                                           QObject* parent = nullptr) = 0;

  [[nodiscard]] virtual IReply* callRaw(const QByteArray& verb,
                                        const QString& path,
                                        const QUrlQuery& parameters = {},
                                        const Headers& headers = {},
                                        QObject* parent = nullptr) const = 0;

  [[nodiscard]] virtual IReply* callRaw(const QByteArray& verb,
                                        const QString& path,
                                        const QJsonValue& data,
                                        const QUrlQuery& parameters = {},
                                        const Headers& headers = {},
                                        QObject* parent = nullptr) const = 0;

  [[nodiscard]] virtual IReply* callRaw(const QByteArray& verb,
                                        const QString& path,
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

 Q_SIGNALS:
  void globalHeadersChanged(const egnite::rest::Headers& headers);
  void globalParametersChanged(const QUrlQuery& parameters);

 protected:
  explicit IApi(QObject* parent = nullptr);
  explicit IApi(QObjectPrivate& impl, QObject* parent = nullptr);
};

template <typename DataType, typename ErrorType>
GenericReply<DataType, ErrorType>* IApi::get(const QString& path,
                                             const QUrlQuery& parameters,
                                             const Headers& headers,
                                             QObject* parent) {
  return new GenericReply<DataType, ErrorType>(
      callRaw(IApi::GetVerb, path, parameters, headers), parent);
}

template <typename DataType, typename ErrorType>
GenericReply<DataType, ErrorType>* IApi::head(const QString& path,
                                              const QUrlQuery& parameters,
                                              const Headers& headers,
                                              QObject* parent) {
  return new GenericReply<DataType, ErrorType>(
      callRaw(IApi::HeadVerb, path, parameters, headers), parent);
}

template <typename DataType, typename ErrorType>
GenericReply<DataType, ErrorType>* IApi::deleteResource(
    const QString& path, const QUrlQuery& parameters, const Headers& headers,
    QObject* parent) {
  return new GenericReply<DataType, ErrorType>(
      callRaw(IApi::DeleteVerb, path, parameters, headers), parent);
}

template <typename DataType, typename ErrorType>
GenericReply<DataType, ErrorType>* IApi::post(const QString& path,
                                              const QUrlQuery& parameters,
                                              const Headers& headers,
                                              QObject* parent) {
  return new GenericReply<DataType, ErrorType>(
      callRaw(IApi::PostVerb, path, parameters, headers), parent);
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
      core::utils::overloaded{
          [&](std::nullopt_t) -> IReply* {
            return callRaw(IApi::PostVerb, path, parameters, headers);
          },
          [&](QJsonValue& data) -> IReply* {
            return callRaw(IApi::PostVerb, path, data, parameters, headers);
          },
          [&](QCborValue& data) -> IReply* {
            return callRaw(IApi::PostVerb, path, data, parameters, headers);
          }},
      serialized_data);
  return new GenericReply<DataType, ErrorType>(reply, parent);
}

template <typename DataType, typename ErrorType>
GenericReply<DataType, ErrorType>* IApi::put(const QString& path,
                                             const QUrlQuery& parameters,
                                             const Headers& headers,
                                             QObject* parent) {
  return new GenericReply<DataType, ErrorType>(
      callRaw(IApi::PutVerb, path, parameters, headers), parent);
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
      core::utils::overloaded{
          [&](std::nullopt_t) -> IReply* {
            return callRaw(IApi::PutVerb, path, parameters, headers);
          },
          [&](QJsonValue& data) -> IReply* {
            return callRaw(IApi::PutVerb, path, data, parameters, headers);
          },
          [&](QCborValue& data) -> IReply* {
            return callRaw(IApi::PutVerb, path, data, parameters, headers);
          }},
      serialized_data);

  return new GenericReply<DataType, ErrorType>(reply, parent);
}

template <typename DataType, typename ErrorType>
GenericReply<DataType, ErrorType>* IApi::patch(const QString& path,
                                               const QUrlQuery& parameters,
                                               const Headers& headers,
                                               QObject* parent) {
  return new GenericReply<DataType, ErrorType>(
      callRaw(IApi::PatchVerb, path, parameters, headers), parent);
}

template <typename DataType, typename ErrorType, typename RequestDataType>
GenericReply<DataType, ErrorType>* IApi::patch(const QString& path,
                                               const RequestDataType& data,
                                               const QUrlQuery& parameters,
                                               const Headers& headers,
                                               QObject* parent) {
  auto serialized_data =
      getDataSerializer()->serialize(data, getRequestDataFormat(headers));
  auto reply = std::visit(
      core::utils::overloaded{
          [&](std::nullopt_t) -> IReply* {
            return callRaw(IApi::PatchVerb, path, parameters, headers);
          },
          [&](QJsonValue& data) -> IReply* {
            return callRaw(IApi::PatchVerb, path, data, parameters, headers);
          },
          [&](QCborValue& data) -> IReply* {
            return callRaw(IApi::PatchVerb, path, data, parameters, headers);
          }},
      serialized_data);

  return new GenericReply<DataType, ErrorType>(reply, parent);
}

/* ------------------------------------ Api --------------------------------- */

class EGNITE_REST_API Api : public IApi {
  Q_OBJECT

 public:
  explicit Api(IClient* client, const QString& subpath,
               QObject* parent = nullptr);
  ~Api() override;

  [[nodiscard]] IClient* getClient() const override;
  [[nodiscard]] IReplyDecorator* getReplyDecorator() const override;
  [[nodiscard]] DataSerializer* getDataSerializer() const override;
  [[nodiscard]] IPagingDataFactory* getPagingDataFactory() const override;

  void setGlobalHeaders(const Headers& headers) override;
  [[nodiscard]] Headers getGlobalHeaders() const override;

  void setGlobalParameters(const QUrlQuery& parameters) override;
  [[nodiscard]] QUrlQuery getGlobalParameters() const override;

  [[nodiscard]] IApi* createSubApi(const QString& path,
                                   QObject* parent = nullptr) override;

  [[nodiscard]] IReply* callRaw(const QByteArray& verb, const QString& path,
                                const QUrlQuery& parameters = {},
                                const Headers& headers = {},
                                QObject* parent = nullptr) const override;

  [[nodiscard]] IReply* callRaw(const QByteArray& verb, const QString& path,
                                const QJsonValue& data,
                                const QUrlQuery& parameters = {},
                                const Headers& headers = {},
                                QObject* parent = nullptr) const override;

  [[nodiscard]] IReply* callRaw(const QByteArray& verb, const QString& path,
                                const QCborValue& data,
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