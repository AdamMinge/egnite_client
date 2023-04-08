/* ----------------------------------- Local -------------------------------- */
#include "egnite/rest/pagination.h"

#include "egnite/rest/detail/pagination_p.h"
/* ----------------------------------- Egnite ------------------------------- */
#include <egnite/core/utils/binder.h>
/* ------------------------------------- Qt --------------------------------- */
#include <QCborArray>
#include <QCborMap>
#include <QJsonArray>
#include <QJsonObject>
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

namespace {

QUrl extractUrl(const Data& data) {
  return std::visit(
      core::utils::overloaded{[&](std::nullopt_t) -> QUrl { return QUrl{}; },
                              [&](const QJsonValue& data) -> QUrl {
                                if (data.isString()) {
                                  auto url = QUrl(data.toString());
                                  if (url.isValid()) return url;
                                }

                                return QUrl{};
                              },
                              [&](const QCborValue& data) -> QUrl {
                                return data.isUrl() ? data.toUrl() : QUrl{};
                              }},
      data);
}

}  // namespace

/* --------------------------------- IPaging -------------------------------- */

IPaging::IPaging() = default;

IPaging::~IPaging() = default;

/* ------------------------------ StandardPaging ---------------------------- */

StandardPaging::StandardPaging(const Data& data, IApi* api)
    : m_impl(std::make_unique<detail::StandardPagingPrivate>(data, api)) {}

StandardPaging::~StandardPaging() = default;

bool StandardPaging::isValid() const { return m_impl->isValid(); }

qint64 StandardPaging::total() const { return m_impl->total(); }

bool StandardPaging::hasNext() const { return m_impl->hasNext(); }

bool StandardPaging::hasPrev() const { return m_impl->hasPrev(); }

QUrl StandardPaging::nextUrl() const { return m_impl->nextUrl(); }

QUrl StandardPaging::prevUrl() const { return m_impl->prevUrl(); }

IReply* StandardPaging::rawNext() const { return m_impl->rawNext(); }

IReply* StandardPaging::rawPrev() const { return m_impl->rawPrev(); }

const Data& StandardPaging::rawItems() const { return m_impl->rawItems(); }

IApi* StandardPaging::getApi() const { return m_impl->getApi(); }

DataSerializer* StandardPaging::getDataSerializer() const {
  return m_impl->getDataSerializer();
}

/* ------------------------------ IPagingFactory ---------------------------- */

IPagingFactory::IPagingFactory() = default;

IPagingFactory::~IPagingFactory() = default;

/* -------------------------- StandardPagingFactory ------------------------- */

StandardPagingFactory::StandardPagingFactory() = default;

StandardPagingFactory::~StandardPagingFactory() = default;

std::unique_ptr<IPaging> StandardPagingFactory::create(const Data& data,
                                                       IApi* api) {
  return std::make_unique<StandardPaging>(data, api);
}

/* ------------------------------- PagingModel ------------------------------ */

PagingModel::PagingModel(IPaging* paging, int type_id, QObject* parent)
    : PagingModel(*new detail::PagingModelPrivate(paging, type_id)) {}

PagingModel::PagingModel(IReply* reply, int type_id, QObject* parent)
    : PagingModel(*new detail::PagingModelPrivate(reply, type_id)) {}

PagingModel::PagingModel(detail::PagingModelPrivate& impl, QObject* parent)
    : QAbstractTableModel(impl, parent) {}

PagingModel::~PagingModel() = default;

QVariant PagingModel::headerData(int section, Qt::Orientation orientation,
                                 int role) const {
  Q_D(const detail::PagingModel);
  if (orientation != Qt::Horizontal || role != Qt::DisplayRole) return {};

  if (const auto& columns = d->getColumns(); !columns.isEmpty())
    return columns.value(section);

  return {};
}

int PagingModel::rowCount(const QModelIndex& parent) const {
  Q_D(const detail::PagingModel);
  return d->getItems().size();
}

int PagingModel::columnCount(const QModelIndex& parent) const {
  Q_D(const detail::PagingModel);
  return d->getColumns().size();
}

bool PagingModel::canFetchMore(const QModelIndex& parent) const {
  Q_D(const detail::PagingModel);
  if (!parent.isValid()) return false;
  return d->canFetchMore();
}

void PagingModel::fetchMore(const QModelIndex& parent) {
  Q_D(detail::PagingModel);
  if (!canFetchMore(parent)) return;
  d->fetchMore();
}

QVariant PagingModel::data(const QModelIndex& index, int role) const {
  Q_D(const detail::PagingModel);
  const auto& items = d->getItems();
  const auto type_id = d->getTypeId();
  const auto& value = items.at(index.row());

  if (role == ModelDataRole) return value;

  if (role == Qt::DisplayRole && type_id != QMetaType::UnknownType) {
    const auto meta_object = QMetaType(type_id).metaObject();
    if (!meta_object) return {};

    const auto property = meta_object->property(index.column());
    if (meta_object->inherits(&QObject::staticMetaObject))
      return property.read(value.value<QObject*>());
    else
      return property.readOnGadget(value.data());
  }

  return {};
}

Qt::ItemFlags PagingModel::flags(const QModelIndex& index) const {
  if (!index.isValid()) return Qt::NoItemFlags;
  return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemNeverHasChildren;
}

namespace detail {

/* --------------------------- StandardPagingPrivate ------------------------ */

StandardPagingPrivate::StandardPagingPrivate(const Data& data, IApi* api)
    : m_api(api),
      m_valid(false),
      m_total(0),
      m_items(std::nullopt),
      m_data(data),
      m_next(QUrl{}),
      m_prev(QUrl{}) {
  static auto contains_required_fields = [](const auto& map) {
    return map.contains(QStringLiteral("count")) &&
           map.contains(QStringLiteral("next")) &&
           map.contains(QStringLiteral("previous")) &&
           map.contains(QStringLiteral("results"));
  };

  m_valid =
      std::visit(core::utils::overloaded{
                     [&](std::nullopt_t) -> bool { return false; },
                     [&](const QJsonValue& data) -> bool {
                       const auto obj = data.toObject();

                       if (!contains_required_fields(obj)) return false;

                       m_total = obj[QStringLiteral("count")].toInteger();
                       m_next = extractUrl(obj[QStringLiteral("next")]);
                       m_prev = extractUrl(obj[QStringLiteral("previous")]);
                       m_items = obj[QStringLiteral("results")].toArray();
                       return true;
                     },
                     [&](const QCborValue& data) -> bool {
                       const auto map = data.toMap();

                       if (!contains_required_fields(map)) return false;

                       m_total = map[QStringLiteral("count")].toInteger();
                       m_next = extractUrl(map[QStringLiteral("next")]);
                       m_prev = extractUrl(map[QStringLiteral("previous")]);
                       m_items = map[QStringLiteral("results")].toArray();
                       return true;
                     }},
                 data);
}

bool StandardPagingPrivate::isValid() const { return m_valid; }

qint64 StandardPagingPrivate::total() const { return m_total; }

bool StandardPagingPrivate::hasNext() const { return !m_next.isEmpty(); }

bool StandardPagingPrivate::hasPrev() const { return !m_prev.isEmpty(); }

QUrl StandardPagingPrivate::nextUrl() const { return m_next; }

QUrl StandardPagingPrivate::prevUrl() const { return m_prev; }

IReply* StandardPagingPrivate::rawNext() const {
  if (hasNext()) return m_api->callRaw(IApi::GetVerb, nextUrl());
  return nullptr;
}

IReply* StandardPagingPrivate::rawPrev() const {
  if (hasNext()) return m_api->callRaw(IApi::GetVerb, nextUrl());
  return nullptr;
}

const Data& StandardPagingPrivate::rawItems() const { return m_items; }

IApi* StandardPagingPrivate::getApi() const { return m_api; }

DataSerializer* StandardPagingPrivate::getDataSerializer() const {
  return m_api->getDataSerializer();
}

/* ----------------------------- PagingModelPrivate ------------------------- */

PagingModelPrivate::PagingModelPrivate(IPaging* paging,
                                       int type_id = QMetaType::UnknownType)
    : m_api(paging->getApi()),
      m_next_url(paging->nextUrl()),
      m_type_id(type_id) {
  initColumns();
  processPaging(*paging);
}

PagingModelPrivate::PagingModelPrivate(IReply* reply, int type_id)
    : m_api(reply->getApi()), m_next_url(QUrl{}), m_type_id(type_id) {
  initColumns();
  reply->onSucceeded([this](int, const Data& data) { processReply(data); });
}

PagingModelPrivate::~PagingModelPrivate() = default;

int PagingModelPrivate::getTypeId() const { return m_type_id; }

const QVariantList& PagingModelPrivate::getItems() const { return m_items; }

const QStringList& PagingModelPrivate::getColumns() const { return m_columns; }

void PagingModelPrivate::fetchMore() {
  Q_Q(PagingModel);
  Q_ASSERT(canFetchMore());
  if (auto reply = m_api->callRaw(IApi::GetVerb, m_next_url); reply) {
    m_next_url = QUrl{};
    reply->onSucceeded([this](int, const Data& data) { processReply(data); },
                       q);
  }
}

bool PagingModelPrivate::canFetchMore() const { return !m_next_url.isEmpty(); }

void PagingModelPrivate::initColumns() {
  if (const auto meta_object = QMetaType(m_type_id).metaObject(); meta_object) {
    for (auto index = meta_object->propertyOffset();
         index < meta_object->propertyCount(); ++index) {
      const auto property_name = meta_object->property(index).name();

      m_columns << property_name;
    }
  }
}

void PagingModelPrivate::processReply(const Data& data) {
  const auto paging = std::visit(
      core::utils::overloaded{
          [](std::nullopt_t) -> std::unique_ptr<IPaging> { return nullptr; },
          [this](const auto& data) -> std::unique_ptr<IPaging> {
            return m_api->getPagingFactory()->create(data, m_api);
          }},
      data);

  if (paging) processPaging(*paging);
}

void PagingModelPrivate::processPaging(const IPaging& paging) {
  Q_Q(PagingModel);
  std::visit(core::utils::overloaded{
                 [](std::nullopt_t) {},
                 [&]<typename Type>(const Type& data) {
                   const auto array = data.toArray();
                   q->beginInsertRows(
                       {}, m_items.size(),
                       static_cast<int>(m_items.size() + array.size() - 1));

                   for (const auto& value : array) {
                     auto new_item = paging.getDataSerializer()->deserialize(
                         value, m_type_id);

                     m_items.append(std::move(new_item));
                   }

                   q->endInsertRows();
                 }},
             paging.rawItems());
}

}  // namespace detail

}  // namespace egnite::rest