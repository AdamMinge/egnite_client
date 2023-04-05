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

/* ------------------------------ IPagingStrategy --------------------------- */

IPagingStrategy::IPagingStrategy() = default;

IPagingStrategy::~IPagingStrategy() = default;

/* --------------------------- StandardPagingStrategy ----------------------- */

StandardPagingStrategy::StandardPagingStrategy(const Data& data)
    : m_impl(detail::StandardPagingStrategyPrivate::create(data)) {}

StandardPagingStrategy::~StandardPagingStrategy() = default;

bool StandardPagingStrategy::valid() const { return static_cast<bool>(m_impl); }

qint64 StandardPagingStrategy::count() const {
  assert(m_impl);
  return m_impl->count();
}

Data StandardPagingStrategy::items() const {
  assert(m_impl);
  return m_impl->items();
}

Data StandardPagingStrategy::data() const {
  assert(m_impl);
  return m_impl->data();
}

bool StandardPagingStrategy::hasNext() const {
  assert(m_impl);
  return m_impl->hasNext();
}

bool StandardPagingStrategy::hasPrev() const {
  assert(m_impl);
  return m_impl->hasPrev();
}

QUrl StandardPagingStrategy::nextUrl() const {
  assert(m_impl);
  return m_impl->nextUrl();
}

QUrl StandardPagingStrategy::prevUrl() const {
  assert(m_impl);
  return m_impl->prevUrl();
}

/* --------------------------- IPagingStrategyFactory ----------------------- */

IPagingStrategyFactory::IPagingStrategyFactory() = default;

IPagingStrategyFactory::~IPagingStrategyFactory() = default;

/* ----------------------- StandardPagingStrategyFactory -------------------- */

StandardPagingStrategyFactory::StandardPagingStrategyFactory() {}

StandardPagingStrategyFactory::~StandardPagingStrategyFactory() {}

std::unique_ptr<IPagingStrategy>
StandardPagingStrategyFactory::createPagingStrategy(const Data& data) {
  return std::make_unique<StandardPagingStrategy>(data);
}

namespace detail {

/* ----------------------- StandardPagingStrategyPrivate -------------------- */

QUrl StandardPagingStrategyPrivate::extractUrl(const Data& data) {
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

std::unique_ptr<StandardPagingStrategyPrivate>
StandardPagingStrategyPrivate::create(const Data& data) {
  return std::visit(
      core::utils::overloaded{
          [&](std::nullopt_t)
              -> std::unique_ptr<StandardPagingStrategyPrivate> {
            return nullptr;
          },
          [&](const QJsonValue& data)
              -> std::unique_ptr<StandardPagingStrategyPrivate> {
            const auto obj = data.toObject();
            const auto count = obj[QStringLiteral("count")].toInteger();
            const auto next = extractUrl(obj[QStringLiteral("next")]);
            const auto prev = extractUrl(obj[QStringLiteral("previous")]);
            const auto items = obj[QStringLiteral("results")].toArray();

            return std::make_unique<StandardPagingStrategyPrivate>(
                count, items, data, next, prev);
          },
          [&](const QCborValue& data)
              -> std::unique_ptr<StandardPagingStrategyPrivate> {
            const auto map = data.toMap();
            const auto count = map[QStringLiteral("count")].toInteger();
            const auto next = extractUrl(map[QStringLiteral("next")]);
            const auto prev = extractUrl(map[QStringLiteral("previous")]);
            const auto items = map[QStringLiteral("results")].toArray();

            return std::make_unique<StandardPagingStrategyPrivate>(
                count, items, data, next, prev);
          }},
      data);
}

StandardPagingStrategyPrivate::StandardPagingStrategyPrivate(qint64 count,
                                                             const Data& items,
                                                             const Data& data,
                                                             const QUrl& next,
                                                             const QUrl& prev)
    : m_count(count),
      m_items(items),
      m_data(data),
      m_next(next),
      m_prev(prev) {}

qint64 StandardPagingStrategyPrivate::count() const { return m_count; }

Data StandardPagingStrategyPrivate::items() const { return m_items; }

Data StandardPagingStrategyPrivate::data() const { return m_data; }

bool StandardPagingStrategyPrivate::hasNext() const {
  return !m_next.isEmpty();
}
bool StandardPagingStrategyPrivate::hasPrev() const {
  return !m_prev.isEmpty();
}

QUrl StandardPagingStrategyPrivate::nextUrl() const { return m_next; }

QUrl StandardPagingStrategyPrivate::prevUrl() const { return m_prev; }

}  // namespace detail

}  // namespace egnite::rest