#ifndef EGNITE_REST_PAGINATION_H
#define EGNITE_REST_PAGINATION_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QObject>
/* ---------------------------------- Standard ------------------------------ */
#include <memory>
/* ----------------------------------- Local -------------------------------- */
#include "egnite/rest/export.h"
#include "egnite/rest/global.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

namespace detail {
class StandardPagingDataPrivate;
}  // namespace detail

template <typename DataType, typename ErrorType>
class GenericReply;
class DataSerializer;
class IApi;

/* -------------------------------- IPagingData ----------------------------- */

class IPagingData {
 public:
  explicit IPagingData();
  virtual ~IPagingData();

  [[nodiscard]] virtual bool valid() const = 0;

  [[nodiscard]] virtual qint64 total() const = 0;

  [[nodiscard]] virtual Data items() const = 0;
  [[nodiscard]] virtual Data data() const = 0;

  [[nodiscard]] virtual bool hasNext() const = 0;
  [[nodiscard]] virtual bool hasPrev() const = 0;

  [[nodiscard]] virtual QUrl nextUrl() const = 0;
  [[nodiscard]] virtual QUrl prevUrl() const = 0;
};

/* ----------------------------- StandardPagingData ------------------------- */

class StandardPagingData : public IPagingData {
 public:
  explicit StandardPagingData(const Data& data);
  ~StandardPagingData() override;

  [[nodiscard]] bool valid() const override;

  [[nodiscard]] qint64 total() const override;

  [[nodiscard]] Data items() const override;
  [[nodiscard]] Data data() const override;

  [[nodiscard]] bool hasNext() const override;
  [[nodiscard]] bool hasPrev() const override;

  [[nodiscard]] QUrl nextUrl() const override;
  [[nodiscard]] QUrl prevUrl() const override;

 private:
  std::unique_ptr<detail::StandardPagingDataPrivate> m_impl;
};

/* ---------------------------- IPagingDataFactory -------------------------- */

class IPagingDataFactory {
 public:
  explicit IPagingDataFactory();
  virtual ~IPagingDataFactory();

  [[nodiscard]] virtual std::unique_ptr<IPagingData> create(
      const Data& data) = 0;
};

/* ------------------------ StandardPagingDataFactory ----------------------- */

class StandardPagingDataFactory : public IPagingDataFactory {
 public:
  explicit StandardPagingDataFactory();
  ~StandardPagingDataFactory() override;

  [[nodiscard]] std::unique_ptr<IPagingData> create(const Data& data) override;
};

/* ---------------------------------- Paging -------------------------------- */

template <typename DataType>
class Paging {
 public:
  explicit Paging(IApi* api, DataSerializer* serializer,
                  std::unique_ptr<IPagingData> paging_data);

  [[nodiscard]] qint64 total() const;

  [[nodiscard]] bool hasNext() const;
  [[nodiscard]] bool hasPrev() const;

  [[nodiscard]] QUrl nextUrl() const;
  [[nodiscard]] QUrl prevUrl() const;

  [[nodiscard]] const QList<DataType>& items() const;

  template <typename ErrorType>
  GenericReply<DataType, ErrorType>* next() const;

  template <typename ErrorType>
  GenericReply<DataType, ErrorType>* prev() const;

 private:
  IApi* m_api;
  DataSerializer* m_serializer;
  std::unique_ptr<IPagingData> m_paging_data;
  std::optional<QList<DataType>> m_items;
};

template <typename DataType>
Paging<DataType>::Paging(IApi* api, DataSerializer* serializer,
                         std::unique_ptr<IPagingData> paging_data)
    : m_api(api),
      m_serializer(serializer),
      m_paging_data(std::move(paging_data)) {}

template <typename DataType>
qint64 Paging<DataType>::total() const {
  return m_paging_data->total();
}

template <typename DataType>
bool Paging<DataType>::hasNext() const {
  return m_paging_data->hasNext();
}

template <typename DataType>
bool Paging<DataType>::hasPrev() const {
  return m_paging_data->hasPrev();
}

template <typename DataType>
QUrl Paging<DataType>::nextUrl() const {
  return m_paging_data->nextUrl();
}

template <typename DataType>
QUrl Paging<DataType>::prevUrl() const {
  return m_paging_data->prevUrl();
}

}  // namespace egnite::rest

#endif  // EGNITE_REST_PAGINATION_H