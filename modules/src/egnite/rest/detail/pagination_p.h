#ifndef EGNITE_REST_PAGINATION_P_H
#define EGNITE_REST_PAGINATION_P_H

/* ------------------------------------ Local ------------------------------- */
#include "egnite/rest/api.h"
#include "egnite/rest/pagination.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

/* ---------------------------------- Paging -------------------------------- */

template <typename DataType>
const QList<DataType>& Paging<DataType>::items() const {
  if (!m_items.has_value()) {
    m_items =
        m_serializer->deserialize<QList<DataType>>(m_paging_data->items());
  }

  assert(m_items.has_value());
  return m_items.value();
}

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

/* ------------------------- StandardPagingDataPrivate ---------------------- */

class StandardPagingDataPrivate {
 public:
  explicit StandardPagingDataPrivate(const Data& data);

  [[nodiscard]] bool valid() const;

  [[nodiscard]] qint64 total() const;

  [[nodiscard]] Data items() const;
  [[nodiscard]] Data data() const;

  [[nodiscard]] bool hasNext() const;
  [[nodiscard]] bool hasPrev() const;

  [[nodiscard]] QUrl nextUrl() const;
  [[nodiscard]] QUrl prevUrl() const;

 private:
  static QUrl extractUrl(const Data& data);

 private:
  bool m_valid;
  qint64 m_total;
  Data m_items;
  Data m_data;
  QUrl m_next;
  QUrl m_prev;
};

}  // namespace detail

}  // namespace egnite::rest

#endif  // EGNITE_REST_PAGINATION_P_H