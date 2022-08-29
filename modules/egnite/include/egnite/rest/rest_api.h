#ifndef EGNITE_REST_API_H
#define EGNITE_REST_API_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>
#include <QString>
#include <QUrlQuery>
/* ----------------------------------- Local -------------------------------- */
#include "egnite/export.h"
#include "egnite/rest/rest_generic_reply.h"
#include "egnite/rest/rest_global.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

namespace detail {
class RestApiPrivate;
}

class RestClient;

class EGNITE_API RestApi : public QObject {
  Q_OBJECT

  friend RestClient;

 public:
  ~RestApi() override;

  [[nodiscard]] RestClient* getClient() const;
  [[nodiscard]] RestDataSerializer* getDataSerializer() const;

  [[nodiscard]] RestApi* createSubApi(const QString& path,
                                      QObject* parent = nullptr);

  /* -------------------------------- Raw Api ----------------------------- */

  [[nodiscard]] RawRestReply* get(const QString& path,
                                  const QUrlQuery& parameters = {},
                                  const RestHeaders& headers = {},
                                  QObject* parent = nullptr) const;
  [[nodiscard]] RawRestReply* head(const QString& path,
                                   const QUrlQuery& parameters = {},
                                   const RestHeaders& headers = {},
                                   QObject* parent = nullptr) const;
  [[nodiscard]] RawRestReply* deleteResource(const QString& path,
                                             const QUrlQuery& parameters = {},
                                             const RestHeaders& headers = {},
                                             QObject* parent = nullptr) const;

  [[nodiscard]] RawRestReply* post(const QString& path,
                                   const QUrlQuery& parameters = {},
                                   const RestHeaders& headers = {},
                                   QObject* parent = nullptr) const;
  [[nodiscard]] RawRestReply* post(const QString& path, const QJsonValue& data,
                                   const QUrlQuery& parameters = {},
                                   const RestHeaders& headers = {},
                                   QObject* parent = nullptr) const;
  [[nodiscard]] RawRestReply* post(const QString& path, const QCborValue& data,
                                   const QUrlQuery& parameters = {},
                                   const RestHeaders& headers = {},
                                   QObject* parent = nullptr) const;

  [[nodiscard]] RawRestReply* put(const QString& path,
                                  const QUrlQuery& parameters = {},
                                  const RestHeaders& headers = {},
                                  QObject* parent = nullptr) const;
  [[nodiscard]] RawRestReply* put(const QString& path, const QJsonValue& data,
                                  const QUrlQuery& parameters = {},
                                  const RestHeaders& headers = {},
                                  QObject* parent = nullptr) const;
  [[nodiscard]] RawRestReply* put(const QString& path, const QCborValue& data,
                                  const QUrlQuery& parameters = {},
                                  const RestHeaders& headers = {},
                                  QObject* parent = nullptr) const;

  [[nodiscard]] RawRestReply* patch(const QString& path,
                                    const QUrlQuery& parameters = {},
                                    const RestHeaders& headers = {},
                                    QObject* parent = nullptr) const;
  [[nodiscard]] RawRestReply* patch(const QString& path, const QJsonValue& data,
                                    const QUrlQuery& parameters = {},
                                    const RestHeaders& headers = {},
                                    QObject* parent = nullptr) const;
  [[nodiscard]] RawRestReply* patch(const QString& path, const QCborValue& data,
                                    const QUrlQuery& parameters = {},
                                    const RestHeaders& headers = {},
                                    QObject* parent = nullptr) const;

  /* ------------------------------ Generic Api --------------------------- */

  template <typename DataType, typename ErrorType>
  [[nodiscard]] GenericRestReply<DataType, ErrorType>* get(
      const QString& path, const QUrlQuery& parameters = {},
      const RestHeaders& headers = {}, QObject* parent = nullptr);

  template <typename DataType, typename ErrorType>
  [[nodiscard]] GenericRestReply<DataType, ErrorType>* head(
      const QString& path, const QUrlQuery& parameters = {},
      const RestHeaders& headers = {}, QObject* parent = nullptr);

  template <typename DataType, typename ErrorType>
  [[nodiscard]] GenericRestReply<DataType, ErrorType>* deleteResource(
      const QString& path, const QUrlQuery& parameters = {},
      const RestHeaders& headers = {}, QObject* parent = nullptr);

  template <typename DataType, typename ErrorType>
  [[nodiscard]] GenericRestReply<DataType, ErrorType>* post(
      const QString& path, const QUrlQuery& parameters = {},
      const RestHeaders& headers = {}, QObject* parent = nullptr);

  template <typename DataType, typename ErrorType, typename RequestDataType>
  [[nodiscard]] GenericRestReply<DataType, ErrorType>* post(
      const QString& path, const RequestDataType& data,
      const QUrlQuery& parameters = {}, const RestHeaders& headers = {},
      QObject* parent = nullptr);

  template <typename DataType, typename ErrorType>
  [[nodiscard]] GenericRestReply<DataType, ErrorType>* put(
      const QString& path, const QUrlQuery& parameters = {},
      const RestHeaders& headers = {}, QObject* parent = nullptr);

  template <typename DataType, typename ErrorType, typename RequestDataType>
  [[nodiscard]] GenericRestReply<DataType, ErrorType>* put(
      const QString& path, const RequestDataType& data,
      const QUrlQuery& parameters = {}, const RestHeaders& headers = {},
      QObject* parent = nullptr);

  template <typename DataType, typename ErrorType>
  [[nodiscard]] GenericRestReply<DataType, ErrorType>* patch(
      const QString& path, const QUrlQuery& parameters = {},
      const RestHeaders& headers = {}, QObject* parent = nullptr);

  template <typename DataType, typename ErrorType, typename RequestDataType>
  [[nodiscard]] GenericRestReply<DataType, ErrorType>* patch(
      const QString& path, const RequestDataType& data,
      const QUrlQuery& parameters = {}, const RestHeaders& headers = {},
      QObject* parent = nullptr);

 protected:
  RestApi(RestClient* client, QNetworkAccessManager* manager,
          const QString& subpath, QObject* parent = nullptr);
  RestApi(detail::RestApiPrivate& impl, QObject* parent = nullptr);

  [[nodiscard]] RestDataSerializer::Format getRequestDataFormat(
      const RestHeaders& headers) const;

 private:
  Q_DECLARE_PRIVATE(detail::RestApi);
};

template <typename DataType, typename ErrorType>
GenericRestReply<DataType, ErrorType>* RestApi::get(const QString& path,
                                                    const QUrlQuery& parameters,
                                                    const RestHeaders& headers,
                                                    QObject* parent) {
  return new GenericRestReply<DataType, ErrorType>(
      get(path, parameters, headers), parent);
}

template <typename DataType, typename ErrorType>
GenericRestReply<DataType, ErrorType>* RestApi::head(
    const QString& path, const QUrlQuery& parameters,
    const RestHeaders& headers, QObject* parent) {
  return new GenericRestReply<DataType, ErrorType>(
      head(path, parameters, headers), parent);
}

template <typename DataType, typename ErrorType>
GenericRestReply<DataType, ErrorType>* RestApi::deleteResource(
    const QString& path, const QUrlQuery& parameters,
    const RestHeaders& headers, QObject* parent) {
  return new GenericRestReply<DataType, ErrorType>(
      deleteResource(path, parameters, headers), parent);
}

template <typename DataType, typename ErrorType>
GenericRestReply<DataType, ErrorType>* RestApi::post(
    const QString& path, const QUrlQuery& parameters,
    const RestHeaders& headers, QObject* parent) {
  return new GenericRestReply<DataType, ErrorType>(
      post(path, parameters, headers), parent);
}

template <typename DataType, typename ErrorType, typename RequestDataType>
GenericRestReply<DataType, ErrorType>* RestApi::post(
    const QString& path, const RequestDataType& data,
    const QUrlQuery& parameters, const RestHeaders& headers, QObject* parent) {
  return new GenericRestReply<DataType, ErrorType>(
      post(path,
           getDataSerializer()->serialize(data, getRequestDataFormat(headers)),
           parameters, headers),
      parent);
}

template <typename DataType, typename ErrorType>
GenericRestReply<DataType, ErrorType>* RestApi::put(const QString& path,
                                                    const QUrlQuery& parameters,
                                                    const RestHeaders& headers,
                                                    QObject* parent) {
  return new GenericRestReply<DataType, ErrorType>(
      put(path, parameters, headers), parent);
}

template <typename DataType, typename ErrorType, typename RequestDataType>
GenericRestReply<DataType, ErrorType>* RestApi::put(const QString& path,
                                                    const RequestDataType& data,
                                                    const QUrlQuery& parameters,
                                                    const RestHeaders& headers,
                                                    QObject* parent) {
  return new GenericRestReply<DataType, ErrorType>(
      put(path,
          getDataSerializer()->serialize(data, getRequestDataFormat(headers)),
          parameters, headers),
      parent);
}

template <typename DataType, typename ErrorType>
GenericRestReply<DataType, ErrorType>* RestApi::patch(
    const QString& path, const QUrlQuery& parameters,
    const RestHeaders& headers, QObject* parent) {
  return new GenericRestReply<DataType, ErrorType>(
      patch(path, parameters, headers), parent);
}

template <typename DataType, typename ErrorType, typename RequestDataType>
GenericRestReply<DataType, ErrorType>* RestApi::patch(
    const QString& path, const RequestDataType& data,
    const QUrlQuery& parameters, const RestHeaders& headers, QObject* parent) {
  return new GenericRestReply<DataType, ErrorType>(
      patch(path,
            getDataSerializer()->serialize(data, getRequestDataFormat(headers)),
            parameters, headers),
      parent);
}

}  // namespace egnite::rest

#endif  // EGNITE_REST_API_H