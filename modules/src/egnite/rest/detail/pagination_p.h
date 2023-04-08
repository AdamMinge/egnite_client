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

  [[nodiscard]] bool isValid() const;

  [[nodiscard]] qint64 total() const;

  [[nodiscard]] bool hasNext() const;
  [[nodiscard]] bool hasPrev() const;

  [[nodiscard]] QUrl nextUrl() const;
  [[nodiscard]] QUrl prevUrl() const;

  [[nodiscard]] IReply* rawNext() const;
  [[nodiscard]] IReply* rawPrev() const;

  [[nodiscard]] const Data& rawItems() const;

  [[nodiscard]] IApi* getApi() const;
  [[nodiscard]] DataSerializer* getDataSerializer() const;

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
  explicit PagingModelPrivate(const IPaging& paging, int type_id);
  ~PagingModelPrivate();

  [[nodiscard]] int getTypeId() const;

  [[nodiscard]] const QVariantList& getItems() const;
  [[nodiscard]] const QStringList& getColumns() const;

  void fetchMore();
  [[nodiscard]] bool canFetchMore() const;

 private:
  void initColumns();
  void processReply(const Data& data);
  void processPaging(const IPaging& paging);

 private:
  IApi* m_api;
  QUrl m_next_url;

  int m_type_id;
  QVariantList m_items;
  QStringList m_columns;
};

}  // namespace egnite::rest::detail

#endif  // EGNITE_REST_PAGINATION_P_H