#ifndef EGNITE_REST_PAGINATION_H
#define EGNITE_REST_PAGINATION_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QAbstractTableModel>
#include <QObject>
/* ---------------------------------- Standard ------------------------------ */
#include <memory>
/* ----------------------------------- Local -------------------------------- */
#include "egnite/rest/data_serializer.h"
#include "egnite/rest/export.h"
#include "egnite/rest/global.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

namespace detail {
class StandardPagingDataPrivate;
class PagingPrivate;
class PagingModelPrivate;
}  // namespace detail

class IApi;
class IReply;

template <typename DataType, typename ErrorType>
class GenericReply;

/* -------------------------------- IPagingData ----------------------------- */

class EGNITE_REST_API IPagingData {
 public:
  explicit IPagingData();
  virtual ~IPagingData();

  [[nodiscard]] virtual bool valid() const = 0;

  [[nodiscard]] virtual qint64 total() const = 0;

  [[nodiscard]] virtual const Data& items() const = 0;
  [[nodiscard]] virtual const Data& data() const = 0;

  [[nodiscard]] virtual bool hasNext() const = 0;
  [[nodiscard]] virtual bool hasPrev() const = 0;

  [[nodiscard]] virtual QUrl nextUrl() const = 0;
  [[nodiscard]] virtual QUrl prevUrl() const = 0;
};

/* ----------------------------- StandardPagingData ------------------------- */

class EGNITE_REST_API StandardPagingData : public IPagingData {
 public:
  explicit StandardPagingData(const Data& data);
  ~StandardPagingData() override;

  [[nodiscard]] bool valid() const override;

  [[nodiscard]] qint64 total() const override;

  [[nodiscard]] const Data& items() const override;
  [[nodiscard]] const Data& data() const override;

  [[nodiscard]] bool hasNext() const override;
  [[nodiscard]] bool hasPrev() const override;

  [[nodiscard]] QUrl nextUrl() const override;
  [[nodiscard]] QUrl prevUrl() const override;

 private:
  std::unique_ptr<detail::StandardPagingDataPrivate> m_impl;
};

/* ---------------------------- IPagingDataFactory -------------------------- */

class EGNITE_REST_API IPagingDataFactory {
 public:
  explicit IPagingDataFactory();
  virtual ~IPagingDataFactory();

  [[nodiscard]] virtual std::unique_ptr<IPagingData> create(
      const Data& data) = 0;
};

/* ------------------------ StandardPagingDataFactory ----------------------- */

class EGNITE_REST_API StandardPagingDataFactory : public IPagingDataFactory {
 public:
  explicit StandardPagingDataFactory();
  ~StandardPagingDataFactory() override;

  [[nodiscard]] std::unique_ptr<IPagingData> create(const Data& data) override;
};

/* ---------------------------------- Paging -------------------------------- */

class Paging {
 public:
  explicit Paging(IApi* api, std::unique_ptr<IPagingData> paging_data);
  virtual ~Paging();

  [[nodiscard]] qint64 total() const;

  [[nodiscard]] bool hasNext() const;
  [[nodiscard]] bool hasPrev() const;

  [[nodiscard]] QUrl nextUrl() const;
  [[nodiscard]] QUrl prevUrl() const;

  [[nodiscard]] IReply* next() const;
  [[nodiscard]] IReply* prev() const;

  [[nodiscard]] const Data& items() const;

 private:
  std::unique_ptr<detail::PagingPrivate> m_impl;
};

/* ---------------------------------- Paging -------------------------------- */

template <typename DataType>
class GenericPaging : public Paging {
 public:
  explicit GenericPaging(IApi* api, DataSerializer* serializer,
                         std::unique_ptr<IPagingData> paging_data);
  ~GenericPaging() override;

  template <typename ErrorType>
  GenericReply<DataType, ErrorType>* next() const;

  template <typename ErrorType>
  GenericReply<DataType, ErrorType>* prev() const;

  [[nodiscard]] const QList<DataType>& items() const;

 private:
  DataSerializer* m_serializer;
  QList<DataType> m_items;
};

template <typename DataType>
GenericPaging<DataType>::GenericPaging(IApi* api, DataSerializer* serializer,
                                       std::unique_ptr<IPagingData> paging_data)
    : Paging(api, std::move(paging_data)), m_serializer(serializer) {}

template <typename DataType>
GenericPaging<DataType>::~GenericPaging() = default;

template <typename DataType>
const QList<DataType>& GenericPaging<DataType>::items() const {
  if (!m_items.has_value()) {
    m_items = m_serializer->deserialize<QList<DataType>>(Paging::items());
  }

  assert(m_items.has_value());
  return m_items.value();
}

template <typename DataType>
template <typename ErrorType>
GenericReply<DataType, ErrorType>* GenericPaging<DataType>::next() const {
  if (hasNext()) new GenericReply<DataType, ErrorType>(Paging::next());
  return nullptr;
}

template <typename DataType>
template <typename ErrorType>
GenericReply<DataType, ErrorType>* GenericPaging<DataType>::prev() const {
  if (hasPrev()) new GenericReply<DataType, ErrorType>(Paging::next());
  return nullptr;
}

/* ------------------------------- PagingModel ------------------------------ */

class EGNITE_REST_API PagingModel : public QAbstractTableModel {
  Q_OBJECT

 public:
  template <typename DataType>
  explicit PagingModel(QObject* parent = nullptr);

  explicit PagingModel(QObject* parent = nullptr);
  ~PagingModel() override;

  QVariant headerData(int section, Qt::Orientation orientation = Qt::Horizontal,
                      int role = Qt::DisplayRole) const override;

  int rowCount(const QModelIndex& parent = QModelIndex()) const override;
  int columnCount(const QModelIndex& parent = QModelIndex()) const override;

  bool canFetchMore(const QModelIndex& parent) const override;
  void fetchMore(const QModelIndex& parent) override;

  QVariant data(const QModelIndex& index,
                int role = Qt::DisplayRole) const override;

  Qt::ItemFlags flags(const QModelIndex& index) const override;
  QHash<int, QByteArray> roleNames() const override;

 protected:
  explicit PagingModel(detail::PagingModelPrivate& impl,
                       QObject* parent = nullptr);

 private:
  Q_DECLARE_PRIVATE(detail::PagingModel);
};

}  // namespace egnite::rest

#endif  // EGNITE_REST_PAGINATION_H