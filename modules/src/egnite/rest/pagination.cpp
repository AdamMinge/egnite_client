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

/* -------------------------------- IPagingData ----------------------------- */

IPagingData::IPagingData() = default;

IPagingData::~IPagingData() = default;

/* ----------------------------- StandardPagingData ------------------------- */

StandardPagingData::StandardPagingData(const Data& data)
    : m_impl(std::make_unique<detail::StandardPagingDataPrivate>(data)) {}

StandardPagingData::~StandardPagingData() = default;

bool StandardPagingData::valid() const { return m_impl->valid(); }

qint64 StandardPagingData::total() const { return m_impl->total(); }

Data StandardPagingData::items() const { return m_impl->items(); }

Data StandardPagingData::data() const { return m_impl->data(); }

bool StandardPagingData::hasNext() const { return m_impl->hasNext(); }

bool StandardPagingData::hasPrev() const { return m_impl->hasPrev(); }

QUrl StandardPagingData::nextUrl() const { return m_impl->nextUrl(); }

QUrl StandardPagingData::prevUrl() const { return m_impl->prevUrl(); }

/* ---------------------------- IPagingDataFactory -------------------------- */

IPagingDataFactory::IPagingDataFactory() = default;

IPagingDataFactory::~IPagingDataFactory() = default;

/* ------------------------ StandardPagingDataFactory ----------------------- */

StandardPagingDataFactory::StandardPagingDataFactory() = default;

StandardPagingDataFactory::~StandardPagingDataFactory() = default;

std::unique_ptr<IPagingData> StandardPagingDataFactory::create(
    const Data& data) {
  return std::make_unique<StandardPagingData>(data);
}

/* ------------------------- StandardPagingDataPrivate ---------------------- */

namespace detail {

StandardPagingDataPrivate::StandardPagingDataPrivate(const Data& data)
    : m_valid(false),
      m_total(0),
      m_items(std::nullopt),
      m_data(data),
      m_next(QUrl{}),
      m_prev(QUrl{}) {
  auto contains_required_fields = [](const auto& map) {
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

bool StandardPagingDataPrivate::valid() const { return m_valid; }

qint64 StandardPagingDataPrivate::total() const { return m_total; }

Data StandardPagingDataPrivate::items() const { return m_items; }

Data StandardPagingDataPrivate::data() const { return m_data; }

bool StandardPagingDataPrivate::hasNext() const { return !m_next.isEmpty(); }

bool StandardPagingDataPrivate::hasPrev() const { return !m_prev.isEmpty(); }

QUrl StandardPagingDataPrivate::nextUrl() const { return m_next; }

QUrl StandardPagingDataPrivate::prevUrl() const { return m_prev; }

QUrl StandardPagingDataPrivate::extractUrl(const Data& data) {
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

}  // namespace detail

}  // namespace egnite::rest