#ifndef EGNITE_REST_PAGINATION_H
#define EGNITE_REST_PAGINATION_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QObject>
/* ---------------------------------- Standard ------------------------------ */
#include <memory>
/* ----------------------------------- Local -------------------------------- */
#include "egnite/rest/export.h"
#include "egnite/rest/global.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

namespace detail {
class StandardPagingStrategyPrivate;
}  // namespace detail

template <typename DataType, typename ErrorType>
class GenericReply;

class IApi;

/* ------------------------------ IPagingStrategy --------------------------- */

class IPagingStrategy {
 public:
  explicit IPagingStrategy();
  virtual ~IPagingStrategy();

  [[nodiscard]] virtual bool valid() const = 0;

  [[nodiscard]] virtual qint64 count() const = 0;

  [[nodiscard]] virtual Data items() const = 0;
  [[nodiscard]] virtual Data data() const = 0;

  [[nodiscard]] virtual bool hasNext() const = 0;
  [[nodiscard]] virtual bool hasPrev() const = 0;

  [[nodiscard]] virtual QUrl nextUrl() const = 0;
  [[nodiscard]] virtual QUrl prevUrl() const = 0;
};

/* --------------------------- StandardPagingStrategy ----------------------- */

class StandardPagingStrategy : public IPagingStrategy {
 public:
  explicit StandardPagingStrategy(const Data& data);
  ~StandardPagingStrategy() override;

  [[nodiscard]] bool valid() const override;

  [[nodiscard]] qint64 count() const override;

  [[nodiscard]] Data items() const override;
  [[nodiscard]] Data data() const override;

  [[nodiscard]] bool hasNext() const override;
  [[nodiscard]] bool hasPrev() const override;

  [[nodiscard]] QUrl nextUrl() const override;
  [[nodiscard]] QUrl prevUrl() const override;

 private:
  std::unique_ptr<detail::StandardPagingStrategyPrivate> m_impl;
};

/* --------------------------- IPagingStrategyFactory ----------------------- */

class IPagingStrategyFactory {
 public:
  explicit IPagingStrategyFactory();
  virtual ~IPagingStrategyFactory();

  [[nodiscard]] virtual std::unique_ptr<IPagingStrategy> createPagingStrategy(
      const Data& data) = 0;
};

/* ----------------------- StandardPagingStrategyFactory -------------------- */

class StandardPagingStrategyFactory : public IPagingStrategyFactory {
 public:
  explicit StandardPagingStrategyFactory();
  ~StandardPagingStrategyFactory() override;

  [[nodiscard]] std::unique_ptr<IPagingStrategy> createPagingStrategy(
      const Data& data) override;
};

/* ---------------------------------- Paging -------------------------------- */

template <typename DataType>
class Paging {
 public:
  explicit Paging(IApi* api, std::unique_ptr<IPagingStrategy> strategy);

  [[nodiscard]] qint64 count() const;

  [[nodiscard]] bool hasNext() const;
  [[nodiscard]] bool hasPrev() const;

  [[nodiscard]] QUrl nextUrl() const;
  [[nodiscard]] QUrl prevUrl() const;

  template <typename ErrorType>
  GenericReply<DataType, ErrorType>* next() const;

  template <typename ErrorType>
  GenericReply<DataType, ErrorType>* prev() const;

 private:
  IApi* m_api;
  std::unique_ptr<IPagingStrategy> m_strategy;
};

template <typename DataType>
Paging<DataType>::Paging(IApi* api, std::unique_ptr<IPagingStrategy> strategy)
    : m_api(api), m_strategy(std::move(strategy)) {}

template <typename DataType>
qint64 Paging<DataType>::count() const {
  return m_strategy->count();
}

template <typename DataType>
bool Paging<DataType>::hasNext() const {
  return m_strategy->hasNext();
}

template <typename DataType>
bool Paging<DataType>::hasPrev() const {
  return m_strategy->hasPrev();
}

template <typename DataType>
QUrl Paging<DataType>::nextUrl() const {
  return m_strategy->nextUrl();
}

template <typename DataType>
QUrl Paging<DataType>::prevUrl() const {
  return m_strategy->prevUrl();
}

}  // namespace egnite::rest

#endif  // EGNITE_REST_PAGINATION_H