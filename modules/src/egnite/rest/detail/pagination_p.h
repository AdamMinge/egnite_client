#ifndef EGNITE_REST_PAGINATION_P_H
#define EGNITE_REST_PAGINATION_P_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QtCore/private/qabstractitemmodel_p.h>
/* ------------------------------------ Local ------------------------------- */
#include "egnite/rest/api.h"
#include "egnite/rest/pagination.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest::detail {

/* --------------------------- StandardPagingPrivate ------------------------ */

class StandardPagingPrivate {
 public:
  explicit StandardPagingPrivate(const Data& data, IApi* api);

  [[nodiscard]] bool valid() const;

  [[nodiscard]] qint64 total() const;

  [[nodiscard]] bool hasNext() const;
  [[nodiscard]] bool hasPrev() const;

  [[nodiscard]] QUrl nextUrl() const;
  [[nodiscard]] QUrl prevUrl() const;

  [[nodiscard]] IReply* rawNext() const;
  [[nodiscard]] IReply* rawPrev() const;

  [[nodiscard]] const Data& rawItems() const;

 private:
  IApi* m_api;
  bool m_valid;
  qint64 m_total;
  Data m_items;
  Data m_data;
  QUrl m_next;
  QUrl m_prev;
};

/* ----------------------------- PagingModelPrivate ------------------------- */

class PagingModelPrivate : public QAbstractItemModelPrivate {
 public:
  Q_DECLARE_PUBLIC(PagingModel)

 public:
  explicit PagingModelPrivate();
  ~PagingModelPrivate();
};

}  // namespace egnite::rest::detail

#endif  // EGNITE_REST_PAGINATION_P_H