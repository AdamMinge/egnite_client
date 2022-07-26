#ifndef EGNITE_BOOST_ARCHIVE_JSON_ARCHIVE_H
#define EGNITE_BOOST_ARCHIVE_JSON_ARCHIVE_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QString>
/* ----------------------------------- Boost -------------------------------- */
#include <boost/archive/detail/common_iarchive.hpp>
#include <boost/archive/detail/common_oarchive.hpp>
#include <boost/archive/detail/register_archive.hpp>
/* ------------------------------- nlohmann_json ---------------------------- */
#include <nlohmann/json.hpp>
/* ---------------------------------- Standard ------------------------------ */
#include <memory>
#include <vector>
/* ----------------------------------- Utils -------------------------------- */
#include <egnite/utils/boost_serializer/wrappers.h>
/* ----------------------------------- Local -------------------------------- */
#include "egnite/utils/boost_archive/export.h"
/* -------------------------------------------------------------------------- */

namespace boost::archive {

namespace detail {

class BOOST_ARCHIVE_API JsonArchive {
public:
  JsonArchive(nlohmann::json& root);
  virtual ~JsonArchive();

  void push(const std::string& key);
  void push(int number);
  void pop();

  const nlohmann::json& value() const;
  nlohmann::json& value();

private:
  nlohmann::json& m_root;
  std::vector<nlohmann::json*> m_stack;
};

template <typename TYPE>
concept ConvertableToJson = requires(const TYPE& value) {
  {nlohmann::json{value}};
};

template <typename TYPE>
concept ConvertableFromJson = requires(TYPE& value) {
  {value = nlohmann::json{}};
};

} // namespace detail

class BOOST_ARCHIVE_API JsonOArchive : public detail::common_oarchive<JsonOArchive> {
  friend class save_access;
  friend class detail::interface_oarchive<JsonOArchive>;
  using BaseClass = detail::common_oarchive<JsonOArchive>;

public:
  JsonOArchive(nlohmann::json& root);
  ~JsonOArchive() override;

private:
  template <typename TYPE> void save(const TYPE& v) {}
  template <detail::ConvertableToJson TYPE> void save(const TYPE& v) {
    write_data(m_archive.value(), nlohmann::json(v));
  }

  void write_data(nlohmann::json& data, const nlohmann::json& input);

  template <typename TYPE> void save_override(const TYPE& object) {
    BaseClass::save_override(object);
  }
  template <typename TYPE>
  void save_override(const serialization::dynamic_array_wrapper<TYPE>& array) {
    for (auto i = 0; i < array.size(); ++i) {
      array_item(i);
      save_override(array.value(i));
      array_end();
    }
  }
  template <typename TYPE> void save_override(const serialization::nvp<TYPE>& nvp) {
    save_start(nvp.name());
    save_override(nvp.const_value());
    save_end(nvp.name());
  }
  template <typename TYPE> void load_override(const serialization::array_wrapper<TYPE>& array) {
    for (auto i = 0; i < array.count(); ++i) {
      array_item(i);
      load_override(array.address()[i]);
      array_end();
    }
  }

  void save_start(const std::string& name);
  void save_end(const std::string& name);

  void array_item(int number);
  void array_end();

  void save_override(const class_name_type& t) {}
  void save_override(const version_type& t) {}
  void save_override(const object_id_type& t) {}
  void save_override(const object_reference_type& t) {}
  void save_override(const class_id_type& t) {}
  void save_override(const class_id_optional_type& t) {}
  void save_override(const class_id_reference_type& t) {}
  void save_override(const tracking_type& t) {}

private:
  detail::JsonArchive m_archive;
};

class BOOST_ARCHIVE_API JsonIArchive : public detail::common_iarchive<JsonIArchive> {
  friend class load_access;
  friend class detail::interface_iarchive<JsonIArchive>;
  using BaseClass = detail::common_iarchive<JsonIArchive>;

public:
  JsonIArchive(nlohmann::json& root);
  ~JsonIArchive() override;

private:
  template <typename TYPE> void load(TYPE& v) {}
  template <detail::ConvertableFromJson TYPE> void load(TYPE& v) {
    v = read_data();
  }

  nlohmann::json& read_data();

  template <typename TYPE> void load_override(TYPE& object) { BaseClass::load_override(object); }
  template <typename TYPE>
  void load_override(const serialization::dynamic_array_wrapper<TYPE>& array) {
    for (auto i = 0; i < array.size(); ++i) {
      array_item(i);
      load_override(array.value(i));
      array_end();
    }
  }
  template <typename TYPE> void load_override(const serialization::nvp<TYPE>& nvp) {
    load_start(nvp.name());
    if (!m_archive.value().is_null())
      load_override(nvp.value());
    load_end(nvp.name());
  }
  template <typename TYPE> void load_override(const serialization::array_wrapper<TYPE>& array) {
    for (auto i = 0; i < array.count(); ++i) {
      array_item(i);
      load_override(array.address()[i]);
      array_end();
    }
  }

  void load_start(const std::string& name);
  void load_end(const std::string& name);

  void array_item(int number);
  void array_end();

  void load_override(class_name_type& t) {}
  void load_override(version_type& t) {}
  void load_override(object_id_type& t) {}
  void load_override(object_reference_type& t) {}
  void load_override(class_id_type& t) {}
  void load_override(class_id_optional_type& t) {}
  void load_override(class_id_reference_type& t) {}
  void load_override(tracking_type& t) {}

private:
  detail::JsonArchive m_archive;
};

} // namespace boost::archive

BOOST_SERIALIZATION_REGISTER_ARCHIVE(boost::archive::JsonOArchive)
BOOST_SERIALIZATION_REGISTER_ARCHIVE(boost::archive::JsonIArchive)

#endif // EGNITE_BOOST_ARCHIVE_JSON_ARCHIVE_H
