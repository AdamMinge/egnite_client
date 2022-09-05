/* ----------------------------------- Local -------------------------------- */
#include "egnite/serializer/cbor_archive.h"

#include "egnite/serializer/detail/cbor_archive_p.h"
/* ----------------------------------- Egnite ------------------------------- */
#include <egnite/core/utils/binder.h>
/* ------------------------------------ Qt ---------------------------------- */
#include <QCborArray>
#include <QCborMap>
/* -------------------------------------------------------------------------- */

namespace egnite::serializer {

/* -------------------------------- CborOArchive ---------------------------- */

CborOArchive::CborOArchive(QCborValue& root)
    : m_impl(std::make_unique<detail::CborOArchivePrivate>(root)) {}

CborOArchive::~CborOArchive() {}

void CborOArchive::write_data(const QCborValue& input) {
  auto& data = m_impl->getArchive().value();

  if (data.isNull() || data.isUndefined()) {
    data = input;
  } else if (data.isArray()) {
    auto array = data.toArray();
    array.append(input);
    data = array;
  } else {
    QCborValue temp(QCborValue::Array);
    temp.swap(data);

    auto array = data.toArray();
    array.append(temp);
    array.append(input);
    data = array;
  }
}

void CborOArchive::save_start(QLatin1String name) {
  m_impl->getArchive().push(name);
}

void CborOArchive::save_end(QLatin1String name) { m_impl->getArchive().pop(); }

void CborOArchive::array_item(int number) { m_impl->getArchive().push(number); }

void CborOArchive::array_end() { m_impl->getArchive().pop(); }

void CborOArchive::save_override(
    const boost::serialization::nvp<boost::serialization::collection_size_type>&
        nvp) {}

void CborOArchive::save_override(const boost::archive::class_name_type& t) {}

void CborOArchive::save_override(const boost::archive::version_type& t) {}

void CborOArchive::save_override(const boost::archive::object_id_type& t) {}

void CborOArchive::save_override(
    const boost::archive::object_reference_type& t) {}

void CborOArchive::save_override(const boost::archive::class_id_type& t) {}

void CborOArchive::save_override(
    const boost::archive::class_id_optional_type& t) {}

void CborOArchive::save_override(
    const boost::archive::class_id_reference_type& t) {}

void CborOArchive::save_override(const boost::archive::tracking_type& t) {}

/* ------------------------------- CborIArchive ----------------------------- */

CborIArchive::CborIArchive(QCborValue& root)
    : m_impl(std::make_unique<detail::CborIArchivePrivate>(root)) {}

CborIArchive::~CborIArchive() {}

QCborValue& CborIArchive::read_data() { return m_impl->getArchive().value(); }

void CborIArchive::load_start(QLatin1String name) {
  m_impl->getArchive().push(name);
}

void CborIArchive::load_end(QLatin1String name) { m_impl->getArchive().pop(); }

void CborIArchive::array_item(int number) { m_impl->getArchive().push(number); }

void CborIArchive::array_end() { m_impl->getArchive().pop(); }

void CborIArchive::load_override(
    const boost::serialization::nvp<boost::serialization::collection_size_type>&
        nvp) {
  auto data = read_data();
  nvp.value() = data.isArray() ? data.toArray().size() : 0;
}

void CborIArchive::load_override(boost::archive::class_name_type& t) {}

void CborIArchive::load_override(boost::archive::version_type& t) {}

void CborIArchive::load_override(boost::archive::object_id_type& t) {}

void CborIArchive::load_override(boost::archive::object_reference_type& t) {}

void CborIArchive::load_override(boost::archive::class_id_type& t) {}

void CborIArchive::load_override(boost::archive::class_id_optional_type& t) {}

void CborIArchive::load_override(boost::archive::class_id_reference_type& t) {}

void CborIArchive::load_override(boost::archive::tracking_type& t) {}

namespace detail {

/* -------------------------------- CborArchive ----------------------------- */

CborArchive::CborArchive(QCborValue& root) : m_root(root) {}

void CborArchive::push(QLatin1String key) {
  m_stack.push(ValueWithKey{.value = value()[key], .key = key});
}

void CborArchive::push(int number) {
  auto& target = value();
  if (!target.isArray()) target = QCborArray{};

  auto array = target.toArray();
  while (array.size() <= number) array.append(QCborValue{});
  target = array;

  m_stack.push(ValueWithKey{.value = array.at(number), .key = number});
}

void CborArchive::pop() {
  auto value_with_key = m_stack.top();
  m_stack.pop();

  std::visit(core::utils::overloaded{
                 [this, value = value_with_key.value](QLatin1String key) {
                   auto object = this->value().toMap();
                   object.insert(key, value);
                   this->value() = object;
                 },
                 [this, value = value_with_key.value](int key) {
                   auto array = this->value().toArray();
                   array.removeAt(key);
                   array.insert(key, value);
                   this->value() = array;
                 }},
             value_with_key.key);
}

const QCborValue& CborArchive::value() const {
  return m_stack.empty() ? m_root : m_stack.top().value;
}

QCborValue& CborArchive::value() {
  return m_stack.empty() ? m_root : m_stack.top().value;
}

/* ---------------------------- CborOArchivePrivate ------------------------- */

CborOArchivePrivate::CborOArchivePrivate(QCborValue& root) : m_archive(root) {}

const CborArchive& CborOArchivePrivate::getArchive() const { return m_archive; }

CborArchive& CborOArchivePrivate::getArchive() { return m_archive; }

/* ---------------------------- CborIArchivePrivate ------------------------- */

CborIArchivePrivate::CborIArchivePrivate(QCborValue& root) : m_archive(root) {}

const CborArchive& CborIArchivePrivate::getArchive() const { return m_archive; }

CborArchive& CborIArchivePrivate::getArchive() { return m_archive; }

}  // namespace detail

}  // namespace egnite::serializer
