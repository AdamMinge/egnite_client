/* ----------------------------------- Local -------------------------------- */
#include "pagination.h"
/* -------------------------------------------------------------------------- */

/* ------------------------------- QmlPagingModel --------------------------- */

QmlPagingModel::QmlPagingModel(QObject* parent)
    : QObject(parent), m_model(nullptr) {}

QmlPagingModel::~QmlPagingModel() = default;

void QmlPagingModel::classBegin() override {}

void QmlPagingModel::componentComplete() override {
  m_revaluate_data.init = true;
  revaluateApi();
}

void QmlPagingModel::setApi(QmlApi* api) {
  if (m_revaluate_data.api == api) return;

  m_revaluate_data.api = api;
  revaluateApi();

  Q_EMIT apiChanged(api);
}

QmlApi* QmlPagingModel::getApi() const { return m_revaluate_data.api; }

QVariant QmlPagingModel::headerData(int section, Qt::Orientation orientation,
                                    int role) const {
  if (!isEvaluated()) return {};
  return m_model->headerData(section, orientation, role);
}

int QmlPagingModel::rowCount(const QModelIndex& parent) const {
  if (!isEvaluated()) return 0;
  return m_model->rowCount(parent);
}

int QmlPagingModel::columnCount(const QModelIndex& parent) const {
  if (!isEvaluated()) return 0;
  return m_model->columnCount(parent);
}

bool QmlPagingModel::canFetchMore(const QModelIndex& parent) const {
  if (!isEvaluated()) return false;
  return m_model->canFetchMore(parent);
}

void QmlPagingModel::fetchMore(const QModelIndex& parent) {
  if (!isEvaluated()) return;
  return m_model->fetchMore(parent);
}

QVariant QmlPagingModel::data(const QModelIndex& index, int role) const {
  if (!isEvaluated()) return {};
  return m_model->data(index, role);
}

Qt::ItemFlags QmlPagingModel::flags(const QModelIndex& index) const {
  if (!isEvaluated()) return {};
  return m_model->flags(index);
}

void QmlPagingModel::revaluateApi() {
  if (!m_revaluate_data.init) return;
  if (m_model) m_api->deleteLater();

  if (!m_revaluate_data.api)
    qmlWarning(this) << "api property must be set";
  else {
  }
}

bool QmlPagingModel::isEvaluated() const {
  if (m_model) return true;

  qmlWarning(this) << "components wasn't evaluate correctly or not at all";
  return false;
}