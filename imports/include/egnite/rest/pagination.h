#ifndef EGNITE_QML_REST_PAGINATION_H
#define EGNITE_QML_REST_PAGINATION_H

/* ------------------------------------- Qt --------------------------------- */
#include <QJSValue>
#include <QtQml>
/* ----------------------------------- Egnite ------------------------------- */
#include <egnite/rest/pagination.h>
/* ----------------------------------- Local -------------------------------- */
#include "api.h"
/* -------------------------------------------------------------------------- */

/* ------------------------------- QmlPagingModel --------------------------- */

class QmlPagingModel : public QObject,
                       public QQmlParserStatus,
                       public QAbstractTableModel {
  Q_OBJECT
  QML_ELEMENT
  Q_INTERFACES(QQmlParserStatus)

  Q_PROPERTY(QmlApi* api READ getApi WRITE setApi NOTIFY apiChanged)

 public:
  explicit QmlPagingModel(QObject* parent = nullptr);
  ~QmlPagingModel() override;

  void classBegin() override;
  void componentComplete() override;

  void setApi(QmlApi* api);
  [[nodiscard]] QmlApi* getApi() const;

  [[nodiscard]] QVariant headerData(
      int section, Qt::Orientation orientation = Qt::Horizontal,
      int role = Qt::DisplayRole) const override;

  [[nodiscard]] int rowCount(
      const QModelIndex& parent = QModelIndex()) const override;
  [[nodiscard]] int columnCount(
      const QModelIndex& parent = QModelIndex()) const override;

  [[nodiscard]] bool canFetchMore(const QModelIndex& parent) const override;
  [[nodiscard]] void fetchMore(const QModelIndex& parent) override;

  [[nodiscard]] QVariant data(const QModelIndex& index,
                              int role = Qt::DisplayRole) const override;

  [[nodiscard]] Qt::ItemFlags flags(const QModelIndex& index) const override;

 Q_SIGNALS:
  void apiChanged(QmlApi* api);

 protected:
  void revaluateApi();
  [[nodiscard]] bool isEvaluated() const;

 protected:
  struct RevaluateData {
    bool init = false;
    QmlApi* api = nullptr;
  };

 protected:
  RevaluateData m_revaluate_data;
  egnite::rest::PagingModel* m_model;
};

#endif  // EGNITE_QML_REST_PAGINATION_H