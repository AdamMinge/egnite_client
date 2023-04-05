#ifndef EGNITE_REST_PAGINATION_P_H
#define EGNITE_REST_PAGINATION_P_H

/* ------------------------------------ Local ------------------------------- */
#include "egnite/rest/api.h"
#include "egnite/rest/pagination.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

/* ---------------------------------- Paging -------------------------------- */

template <typename DataType>
template <typename ErrorType>
GenericReply<DataType, ErrorType>* Paging<DataType>::next() const {
  if (hasNext())
    return m_api->template get<Paging<DataType>, ErrorType>(nextUrl());
  return nullptr;
}

template <typename DataType>
template <typename ErrorType>
GenericReply<DataType, ErrorType>* Paging<DataType>::prev() const {
  if (hasPrev())
    return m_api->template get<Paging<DataType>, ErrorType>(prevUrl());
  return nullptr;
}

namespace detail {

/* ----------------------- StandardPagingStrategyPrivate -------------------- */

class StandardPagingStrategyPrivate {
 public:
  static QUrl extractUrl(const Data& data);
  static std::unique_ptr<StandardPagingStrategyPrivate> create(
      const Data& data);

 public:
  explicit StandardPagingStrategyPrivate(qint64 count, const Data& items,
                                         const Data& data, const QUrl& next,
                                         const QUrl& prev);

  [[nodiscard]] qint64 count() const;

  [[nodiscard]] Data items() const;
  [[nodiscard]] Data data() const;

  [[nodiscard]] bool hasNext() const;
  [[nodiscard]] bool hasPrev() const;

  [[nodiscard]] QUrl nextUrl() const;
  [[nodiscard]] QUrl prevUrl() const;

 private:
  qint64 m_count;
  Data m_items;
  Data m_data;
  QUrl m_next;
  QUrl m_prev;
};

}  // namespace detail

}  // namespace egnite::rest

#endif  // EGNITE_REST_PAGINATION_P_H