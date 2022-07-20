#ifndef EGNITE_JSON_ARCHIVE_H
#define EGNITE_JSON_ARCHIVE_H

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
#include <stack>
/* ----------------------------------- Local -------------------------------- */
#include "egnite/utils/json_archive/export.h"
/* -------------------------------------------------------------------------- */

namespace boost::serialization {

namespace detail {

class JSON_ARCHIVE_API JsonArchive {
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
  std::stack<nlohmann::json*> m_stack;
};

template <typename TYPE>
concept ConvertableToJson = requires(TYPE value) {
  {nlohmann::json{value}};
};

template <typename TYPE>
concept ConvertableFromJson = requires() {
  {TYPE{} = nlohmann::json{}};
};

} // namespace detail

class JSON_ARCHIVE_API JsonOArchive : public boost::archive::detail::common_oarchive<JsonOArchive> {
  friend class boost::archive::save_access;
  friend class boost::archive::detail::interface_oarchive<JsonOArchive>;

  struct DataItem {
    std::string key;
    bool pop_flag;
  };
  using DataVector = std::vector<DataItem>;
  using BaseClass = boost::archive::detail::common_oarchive<JsonOArchive>;

public:
  JsonOArchive(nlohmann::json& root);
  ~JsonOArchive() override;

private:
  template <typename TYPE> void save(const TYPE& v) {}
  template <detail::ConvertableToJson TYPE> void save(const TYPE& v) {
    write_data(m_archive.value(), nlohmann::json(v));
  }

  void write_data(nlohmann::json& data, const nlohmann::json& input);

  template <typename TYPE> void save_override(TYPE& object) { BaseClass::save_override(object); }
  template <typename TYPE> void save_override(const boost::serialization::nvp<TYPE>& nvp) {
    nvp_start(nvp.name());
    BaseClass::save_override(nvp.const_value());
    nvp_end(nvp.name());
  }
  template <typename TYPE>
  void save_override(const boost::serialization::array_wrapper<TYPE>& array) {
    for (auto i = 0; i < array.count(); ++i) {
      array_item_start(array.count());
      BaseClass::save_override(array.address()[i]);
      array_item_end(array.name());
    }
  }

  void nvp_start(const std::string& name);
  void nvp_end(const std::string& name);

  void array_item_start(int number);
  void array_item_end();

private:
  detail::JsonArchive m_archive;
  DataVector m_array_stack;
  unsigned m_array_level;
};

class JSON_ARCHIVE_API JsonIArchive : public boost::archive::detail::common_iarchive<JsonIArchive> {
  friend class boost::archive::load_access;
  friend class boost::archive::detail::interface_iarchive<JsonIArchive>;

  struct DataItem {
    std::string key;
    int index;
    bool pop_flag;
  };
  using DataVector = std::vector<DataItem>;
  using BaseClass = boost::archive::detail::common_iarchive<JsonIArchive>;

public:
  JsonIArchive(nlohmann::json& root);
  ~JsonIArchive() override;

private:
  template <typename TYPE> void load(TYPE& v) {}
  template <detail::ConvertableFromJson TYPE> void load(TYPE& v) { v = read_data(); }

  nlohmann::json& read_data();

  template <typename TYPE> void load_override(TYPE& object) { BaseClass::load_override(object); }
  template <typename TYPE> void load_override(const boost::serialization::nvp<TYPE>& nvp) {
    nvp_start(nvp.name());
    BaseClass::load_override(nvp.value());
    nvp_end(nvp.name());
  }
  template <typename TYPE>
  void load_override(const boost::serialization::array_wrapper<TYPE>& array) {
    for (auto i = 0; i < array.count(); ++i) {
      array_start(i);
      BaseClass::load_override(array.address()[i]);
      array_end();
    }
  }

  void nvp_start(const std::string& name);
  void nvp_end(const std::string& name);

  void array_start(int number);
  void array_end();

private:
  detail::JsonArchive m_archive;
  DataVector m_array_stack;
  unsigned m_array_level;
};

} // namespace boost::serialization

BOOST_SERIALIZATION_REGISTER_ARCHIVE(boost::serialization::JsonOArchive)
BOOST_SERIALIZATION_REGISTER_ARCHIVE(boost::serialization::JsonIArchive)

#endif // EGNITE_JSON_ARCHIVE_H
