#ifndef EGNITE_SERIALIZER_JSON_ARCHIVE_H
#define EGNITE_SERIALIZER_JSON_ARCHIVE_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QJsonValue>
#include <QLatin1String>
/* ----------------------------------- Boost -------------------------------- */
#include <boost/archive/detail/common_iarchive.hpp>
#include <boost/archive/detail/common_oarchive.hpp>
#include <boost/archive/detail/register_archive.hpp>
#include <boost/serialization/collection_size_type.hpp>
/* ---------------------------------- Standard ------------------------------ */
#include <memory>
/* ----------------------------------- Local -------------------------------- */
#include "egnite/serializer/export.h"
/* -------------------------------------------------------------------------- */

namespace egnite::serializer {

namespace detail {
class JsonOArchivePrivate;
class JsonIArchivePrivate;
}  // namespace detail

class EGNITE_SERIALIZER_API JsonOArchive
    : public boost::archive::detail::common_oarchive<JsonOArchive> {
  friend class boost::archive::save_access;
  friend class boost::archive::detail::interface_oarchive<JsonOArchive>;
  using BaseClass = boost::archive::detail::common_oarchive<JsonOArchive>;

 public:
  explicit JsonOArchive(QJsonValue& root);
  ~JsonOArchive() override;

 private:
  template <typename TYPE>
  void save(const TYPE& v) {
    write_data(QJsonValue::fromVariant(v));
  }

  void write_data(const QJsonValue& input);

  template <typename TYPE>
  void save_override(const TYPE& object) {
    BaseClass::save_override(object);
  }

  template <typename TYPE>
  void save_override(const boost::serialization::nvp<TYPE>& nvp) {
    save_start(QLatin1String(nvp.name()));
    save_override(nvp.const_value());
    save_end(QLatin1String(nvp.name()));
  }

  template <typename TYPE>
  void save_override(const boost::serialization::array_wrapper<TYPE>& array) {
    for (auto i = 0; i < array.count(); ++i) {
      array_item(i);
      save_override(array.address()[i]);
      array_end();
    }
  }

  void save_start(QLatin1String name);
  void save_end(QLatin1String name);

  void array_item(int number);
  void array_end();

  void save_override(const boost::serialization::nvp<
                     boost::serialization::collection_size_type>& nvp);

  void save_override(const boost::archive::class_name_type& t);
  void save_override(const boost::archive::version_type& t);
  void save_override(const boost::archive::object_id_type& t);
  void save_override(const boost::archive::object_reference_type& t);
  void save_override(const boost::archive::class_id_type& t);
  void save_override(const boost::archive::class_id_optional_type& t);
  void save_override(const boost::archive::class_id_reference_type& t);
  void save_override(const boost::archive::tracking_type& t);

 private:
  std::unique_ptr<detail::JsonOArchivePrivate> m_impl;
};

class EGNITE_SERIALIZER_API JsonIArchive
    : public boost::archive::detail::common_iarchive<JsonIArchive> {
  friend class boost::archive::load_access;
  friend class boost::archive::detail::interface_iarchive<JsonIArchive>;
  using BaseClass = boost::archive::detail::common_iarchive<JsonIArchive>;

 public:
  JsonIArchive(QJsonValue& root);
  ~JsonIArchive() override;

 private:
  template <typename TYPE>
  void load(TYPE& v) {
    v = read_data().toVariant().value<TYPE>();
  }

  QJsonValue& read_data();

  template <typename TYPE>
  void load_override(TYPE& object) {
    BaseClass::load_override(object);
  }

  template <typename TYPE>
  void load_override(const boost::serialization::nvp<TYPE>& nvp) {
    load_start(QLatin1String(nvp.name()));
    load_override(nvp.value());
    load_end(QLatin1String(nvp.name()));
  }

  template <typename TYPE>
  void load_override(const boost::serialization::array_wrapper<TYPE>& array) {
    for (auto i = 0; i < array.count(); ++i) {
      array_item(i);
      load_override(array.address()[i]);
      array_end();
    }
  }

  void load_start(QLatin1String name);
  void load_end(QLatin1String name);

  void array_item(int number);
  void array_end();

  void load_override(const boost::serialization::nvp<
                     boost::serialization::collection_size_type>& nvp);

  void load_override(boost::archive::class_name_type& t);
  void load_override(boost::archive::version_type& t);
  void load_override(boost::archive::object_id_type& t);
  void load_override(boost::archive::object_reference_type& t);
  void load_override(boost::archive::class_id_type& t);
  void load_override(boost::archive::class_id_optional_type& t);
  void load_override(boost::archive::class_id_reference_type& t);
  void load_override(boost::archive::tracking_type& t);

 private:
  std::unique_ptr<detail::JsonIArchivePrivate> m_impl;
};

}  // namespace egnite::serializer

#endif  // EGNITE_SERIALIZER_JSON_ARCHIVE_H