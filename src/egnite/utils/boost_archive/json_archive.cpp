/* ----------------------------------- Local -------------------------------- */
#include "egnite/utils/boost_archive/json_archive.h"
/* -------------------------------------------------------------------------- */

namespace boost::archive {

/* ------------------------------ JsonArchive ----------------------------- */

namespace detail {

JsonArchive::JsonArchive(nlohmann::json& root) : m_root(root) { m_stack.push_back(&root); }

JsonArchive::~JsonArchive() = default;

void JsonArchive::push(const std::string& key) { m_stack.push_back(&value()[key]); }

void JsonArchive::push(int number) {
  auto& target = value();
  if (!target.is_array())
    target = nlohmann::json();

  m_stack.push_back(&target[number]);
}

void JsonArchive::pop() { m_stack.pop_back(); }

const nlohmann::json& JsonArchive::value() const { return *m_stack.back(); }

nlohmann::json& JsonArchive::value() { return *m_stack.back(); }

} // namespace detail

/* ------------------------------ JsonOArchive ---------------------------- */

JsonOArchive::JsonOArchive(nlohmann::json& root) : m_archive(root) {}

JsonOArchive::~JsonOArchive() = default;

void JsonOArchive::write_data(nlohmann::json& data, const nlohmann::json& input) {
  if (data.is_null()) {
    data = input;
  } else if (data.is_array()) {
    data.push_back(input);
  } else {
    nlohmann::json temp;
    temp.swap(data);
    data.push_back(temp);
    data.push_back(input);
  }
}

void JsonOArchive::save_start(const std::string& name) {
  if (name.empty())
    return;

  m_archive.push(name);
}

void JsonOArchive::save_end(const std::string& name) {
  if (name.empty())
    return;

  m_archive.pop();
}

void JsonOArchive::array_item(int number) { m_archive.push(number); }

void JsonOArchive::array_end() { m_archive.pop(); }

/* ------------------------------ JsonIArchive ---------------------------- */

JsonIArchive::JsonIArchive(nlohmann::json& root) : m_archive(root) {}

JsonIArchive::~JsonIArchive() = default;

nlohmann::json& JsonIArchive::read_data() { return m_archive.value(); }

void JsonIArchive::load_start(const std::string& name) {
  if (name.empty())
    return;

  m_archive.push(name);
}

void JsonIArchive::load_end(const std::string& name) {
  if (name.empty())
    return;

  m_archive.pop();
}

void JsonIArchive::array_item(int number) { m_archive.push(number); }

void JsonIArchive::array_end() { m_archive.pop(); }

} // namespace boost::archive
