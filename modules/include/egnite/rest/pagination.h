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
class StandardPagingPrivate;
class PagingModelPrivate;
}  // namespace detail

class IApi;
class IReply;

template <typename DataType, typename ErrorType>
class GenericReply;

/* ---------------------------------- IPaging ------------------------------- */

class IPaging {
 public:
  explicit IPaging();
  virtual ~IPaging();

  [[nodiscard]] virtual bool valid() const = 0;

  [[nodiscard]] virtual qint64 total() const = 0;

  [[nodiscard]] virtual bool hasNext() const = 0;
  [[nodiscard]] virtual bool hasPrev() const = 0;

  [[nodiscard]] virtual QUrl nextUrl() const = 0;
  [[nodiscard]] virtual QUrl prevUrl() const = 0;

  [[nodiscard]] virtual IReply* rawNext() const = 0;
  [[nodiscard]] virtual IReply* rawPrev() const = 0;

  [[nodiscard]] virtual const Data& rawItems() const = 0;
};

/* ------------------------------ StandardPaging ---------------------------- */

class StandardPaging : public IPaging {
 public:
  explicit StandardPaging(const Data& data, IApi* api);
  ~StandardPaging() override;

  [[nodiscard]] bool valid() const override;

  [[nodiscard]] qint64 total() const override;

  [[nodiscard]] bool hasNext() const override;
  [[nodiscard]] bool hasPrev() const override;

  [[nodiscard]] QUrl nextUrl() const override;
  [[nodiscard]] QUrl prevUrl() const override;

  [[nodiscard]] IReply* rawNext() const override;
  [[nodiscard]] IReply* rawPrev() const override;

  [[nodiscard]] const Data& rawItems() const override;

 private:
  std::unique_ptr<detail::StandardPagingPrivate> m_impl;
};

/* ------------------------------ IPagingFactory ---------------------------- */

class EGNITE_REST_API IPagingFactory {
 public:
  explicit IPagingFactory();
  virtual ~IPagingFactory();

  [[nodiscard]] virtual std::unique_ptr<IPaging> create(const Data& data,
                                                        IApi* api) = 0;
};

/* -------------------------- StandardPagingFactory ------------------------- */

class EGNITE_REST_API StandardPagingFactory : public IPagingFactory {
 public:
  explicit StandardPagingFactory();
  ~StandardPagingFactory() override;

  [[nodiscard]] std::unique_ptr<IPaging> create(const Data& data,
                                                IApi* api) override;
};

/* ------------------------------ GenericPaging ----------------------------- */

template <typename DataType>
class GenericPaging : public IPaging {
 public:
  explicit GenericPaging(std::unique_ptr<IPaging> paging,
                         DataSerializer* serializer);
  ~GenericPaging() override;

  [[nodiscard]] bool valid() const override;

  [[nodiscard]] qint64 total() const override;

  [[nodiscard]] bool hasNext() const override;
  [[nodiscard]] bool hasPrev() const override;

  [[nodiscard]] QUrl nextUrl() const override;
  [[nodiscard]] QUrl prevUrl() const override;

  [[nodiscard]] IReply* rawNext() const override;
  [[nodiscard]] IReply* rawPrev() const override;

  [[nodiscard]] const Data& rawItems() const override;

  template <typename ErrorType>
  GenericReply<DataType, ErrorType>* next() const;

  template <typename ErrorType>
  GenericReply<DataType, ErrorType>* prev() const;

  [[nodiscard]] const QList<DataType>& items() const;

 private:
  std::unique_ptr<IPaging> m_paging;
  DataSerializer* m_serializer;
  QList<DataType> m_items;
};

template <typename DataType>
GenericPaging<DataType>::GenericPaging(std::unique_ptr<IPaging> paging,
                                       DataSerializer* serializer)
    : m_paging(std::move(m_paging)), m_serializer(serializer) {}

template <typename DataType>
GenericPaging<DataType>::~GenericPaging() = default;

template <typename DataType>
bool GenericPaging<DataType>::valid() const {
  return m_paging->valid();
}

template <typename DataType>
qint64 GenericPaging<DataType>::total() const {
  return m_paging->total();
}

template <typename DataType>
bool GenericPaging<DataType>::hasNext() const {
  return m_paging->hasNext();
}

template <typename DataType>
bool GenericPaging<DataType>::hasPrev() const {
  return m_paging->hasPrev();
}

template <typename DataType>
QUrl GenericPaging<DataType>::nextUrl() const {
  return m_paging->nextUrl();
}

template <typename DataType>
QUrl GenericPaging<DataType>::prevUrl() const {
  return m_paging->prevUrl();
}

template <typename DataType>
IReply* GenericPaging<DataType>::rawNext() const {
  return m_paging->rawNext();
}

template <typename DataType>
IReply* GenericPaging<DataType>::rawPrev() const {
  return m_paging->rawPrev();
}

template <typename DataType>
const Data& GenericPaging<DataType>::rawItems() const {
  return m_paging->rawItems();
}

template <typename DataType>
const QList<DataType>& GenericPaging<DataType>::items() const {
  if (!m_items.has_value()) {
    m_items = m_serializer->deserialize<QList<DataType>>(m_paging->rawItems());
  }

  assert(m_items.has_value());
  return m_items.value();
}

template <typename DataType>
template <typename ErrorType>
GenericReply<DataType, ErrorType>* GenericPaging<DataType>::next() const {
  if (hasNext()) new GenericReply<DataType, ErrorType>(m_paging->rawNext());
  return nullptr;
}

template <typename DataType>
template <typename ErrorType>
GenericReply<DataType, ErrorType>* GenericPaging<DataType>::prev() const {
  if (hasPrev()) new GenericReply<DataType, ErrorType>(m_paging->rawPrev());
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