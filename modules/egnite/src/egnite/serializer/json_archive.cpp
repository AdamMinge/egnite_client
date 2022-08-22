/* ----------------------------------- Local -------------------------------- */
#include "egnite/serializer/json_archive.h"

#include "egnite/serializer/detail/json_archive_p.h"
#include "egnite/utils/binder.h"
/* ------------------------------------ Qt ---------------------------------- */
#include <QJsonArray>
#include <QJsonObject>
/* -------------------------------------------------------------------------- */

namespace egnite::serializer {

/* -------------------------------- JsonOArchive ---------------------------- */

JsonOArchive::JsonOArchive(QJsonValue& root)
    : m_impl(std::make_unique<detail::JsonOArchivePrivate>(root)) {}

JsonOArchive::~JsonOArchive() {}

void JsonOArchive::write_data(const QJsonValue& input) {
  auto& data = m_impl->getArchive().value();

  if (data.isNull() || data.isUndefined()) {
    data = input;
  } else if (data.isArray()) {
    auto array = data.toArray();
    array.append(input);
    data = array;
  } else {
    QJsonValue temp(QJsonValue::Array);
    temp.swap(data);

    auto array = data.toArray();
    array.append(temp);
    array.append(input);
    data = array;
  }
}

void JsonOArchive::save_start(QLatin1String name) {
  m_impl->getArchive().push(name);
}

void JsonOArchive::save_end(QLatin1String name) { m_impl->getArchive().pop(); }

void JsonOArchive::array_item(int number) { m_impl->getArchive().push(number); }

void JsonOArchive::array_end() { m_impl->getArchive().pop(); }

/* ------------------------------- JsonIArchive ----------------------------- */

JsonIArchive::JsonIArchive(QJsonValue& root)
    : m_impl(std::make_unique<detail::JsonIArchivePrivate>(root)) {}

JsonIArchive::~JsonIArchive() {}

QJsonValue& JsonIArchive::read_data() { return m_impl->getArchive().value(); }

void JsonIArchive::load_start(QLatin1String name) {
  m_impl->getArchive().push(name);
}

void JsonIArchive::load_end(QLatin1String name) { m_impl->getArchive().pop(); }

void JsonIArchive::array_item(int number) { m_impl->getArchive().push(number); }

void JsonIArchive::array_end() { m_impl->getArchive().pop(); }

namespace detail {

/* -------------------------------- JsonArchive ----------------------------- */

JsonArchive::JsonArchive(QJsonValue& root) : m_root(root) {}

void JsonArchive::push(QLatin1String key) {
  m_stack.push(ValueWithKey{.value = value()[key], .key = key});
}

void JsonArchive::push(int number) {
  auto& target = value();
  if (!target.isArray()) target = QJsonArray{};

  auto array = target.toArray();
  while (array.size() < number) array.append(QJsonValue{});
  target = array;

  m_stack.push(ValueWithKey{.value = array.at(number), .key = number});
}

void JsonArchive::pop() {
  auto value_with_key = m_stack.top();
  m_stack.pop();

  std::visit(utils::overloaded{
                 [this, value = value_with_key.value](QLatin1String key) {
                   auto object = this->value().toObject();
                   object.insert(key, value);
                   this->value() = object;
                 },
                 [this, value = value_with_key.value](int key) {
                   auto array = this->value().toArray();
                   array.insert(key, value);
                   this->value() = array;
                 }},
             value_with_key.key);
}

const QJsonValue& JsonArchive::value() const {
  return m_stack.empty() ? m_root : m_stack.top().value;
}

QJsonValue& JsonArchive::value() {
  return m_stack.empty() ? m_root : m_stack.top().value;
}

/* ---------------------------- JsonOArchivePrivate ------------------------- */

JsonOArchivePrivate::JsonOArchivePrivate(QJsonValue& root) : m_archive(root) {}

const JsonArchive& JsonOArchivePrivate::getArchive() const { return m_archive; }

JsonArchive& JsonOArchivePrivate::getArchive() { return m_archive; }

/* ---------------------------- JsonIArchivePrivate ------------------------- */

JsonIArchivePrivate::JsonIArchivePrivate(QJsonValue& root) : m_archive(root) {}

const JsonArchive& JsonIArchivePrivate::getArchive() const { return m_archive; }

JsonArchive& JsonIArchivePrivate::getArchive() { return m_archive; }

}  // namespace detail

}  // namespace egnite::serializer
