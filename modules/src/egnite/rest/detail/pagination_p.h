#ifndef EGNITE_REST_PAGINATION_P_H
#define EGNITE_REST_PAGINATION_P_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QtCore/private/qabstractitemmodel_p.h>
/* ------------------------------------ Local ------------------------------- */
#include "egnite/rest/api.h"
#include "egnite/rest/pagination.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

namespace detail {

/* ------------------------- StandardPagingDataPrivate ---------------------- */

class StandardPagingDataPrivate {
 public:
  explicit StandardPagingDataPrivate(const Data& data);

  [[nodiscard]] bool valid() const;

  [[nodiscard]] qint64 total() const;

  [[nodiscard]] const Data& items() const;
  [[nodiscard]] const Data& data() const;

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

/* ------------------------------- PagingPrivate ---------------------------- */

class PagingPrivate {
 public:
  explicit PagingPrivate(IApi*, std::unique_ptr<IPagingData> paging_data);
  ~PagingPrivate();

  IApi* getApi() const;
  IPagingData* getPagingData() const;

 private:
  IApi* m_api;
  std::unique_ptr<IPagingData> m_paging_data;
};

/* ----------------------------- PagingModelPrivate ------------------------- */

class PagingModelPrivate : public QAbstractItemModelPrivate {
 public:
  Q_DECLARE_PUBLIC(PagingModel)

 public:
  explicit PagingModelPrivate();
  ~PagingModelPrivate();
};

}  // namespace detail

}  // namespace egnite::rest

#endif  // EGNITE_REST_PAGINATION_P_H