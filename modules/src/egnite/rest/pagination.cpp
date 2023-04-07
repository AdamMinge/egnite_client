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

bool StandardPaging::valid() const { return m_impl->valid(); }

qint64 StandardPaging::total() const { return m_impl->total(); }

bool StandardPaging::hasNext() const { return m_impl->hasNext(); }

bool StandardPaging::hasPrev() const { return m_impl->hasPrev(); }

QUrl StandardPaging::nextUrl() const { return m_impl->nextUrl(); }

QUrl StandardPaging::prevUrl() const { return m_impl->prevUrl(); }

IReply* StandardPaging::rawNext() const { return m_impl->rawNext(); }

IReply* StandardPaging::rawPrev() const { return m_impl->rawPrev(); }

const Data& StandardPaging::rawItems() const { return m_impl->rawItems(); }

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

PagingModel::PagingModel(QObject* parent)
    : PagingModel(*new detail::PagingModelPrivate()) {}

PagingModel::PagingModel(detail::PagingModelPrivate& impl, QObject* parent)
    : QAbstractTableModel(impl, parent) {}

PagingModel::~PagingModel() = default;

QVariant PagingModel::headerData(int section, Qt::Orientation orientation,
                                 int role) const {}

int PagingModel::rowCount(const QModelIndex& parent) const {}

int PagingModel::columnCount(const QModelIndex& parent) const {}

bool PagingModel::canFetchMore(const QModelIndex& parent) const {}

void PagingModel::fetchMore(const QModelIndex& parent) {}

QVariant PagingModel::data(const QModelIndex& index, int role) const {}

Qt::ItemFlags PagingModel::flags(const QModelIndex& index) const {}

QHash<int, QByteArray> PagingModel::roleNames() const {}

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

bool StandardPagingPrivate::valid() const { return m_valid; }

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

/* ----------------------------- PagingModelPrivate ------------------------- */

PagingModelPrivate::PagingModelPrivate() = default;

PagingModelPrivate::~PagingModelPrivate() = default;

}  // namespace detail

}  // namespace egnite::rest