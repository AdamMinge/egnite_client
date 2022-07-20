/* ----------------------------------- Local -------------------------------- */
#include "egnite/utils/json_archive/json_archive.h"
/* -------------------------------------------------------------------------- */

namespace boost::serialization {

/* ------------------------------ JsonArchive ----------------------------- */

namespace detail {

JsonArchive::JsonArchive(nlohmann::json& root) : m_root(root) { m_stack.push(&root); }

JsonArchive::~JsonArchive() = default;

void JsonArchive::push(const std::string& key) { m_stack.push(&value()[key]); }

void JsonArchive::push(int number) {
  auto& target = value();
  if (!target.is_array())
    target = nlohmann::json();

  m_stack.push(&target[number]);
}

void JsonArchive::pop() { m_stack.pop(); }

const nlohmann::json& JsonArchive::value() const { return *m_stack.top(); }

nlohmann::json& JsonArchive::value() { return *m_stack.top(); }

} // namespace detail

/* ------------------------------ JsonOArchive ---------------------------- */

JsonOArchive::JsonOArchive(nlohmann::json& root) : m_archive(root), m_array_level(0) {}

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

void JsonOArchive::nvp_start(const std::string& name) {
  if (name.empty())
    return;

  m_archive.push(name);
  auto& data = m_archive.value();
  const auto number = m_array_stack.size();

  if (number < ++m_array_level) {
    m_array_stack.emplace_back(DataItem{.key = name, .pop_flag = false});
    data = nlohmann::json();
  } else {
    auto& item = m_array_stack.back();
    if (item.key != name) {
      item.key = name;
      item.pop_flag = false;
      data = nlohmann::json();
    } else {
      if (!data.is_array()) {
        item.pop_flag = true;

        nlohmann::json temp;
        temp.swap(data);
        data.push_back(temp);
      }

      m_archive.push(data.size());
    }
  }
}

void JsonOArchive::nvp_end(const std::string& name) {
  if (name.empty())
    return;

  m_archive.pop();
  if (m_array_stack[--m_array_level].pop_flag)
    m_archive.pop();

  m_array_stack.resize(m_array_level + 1);
}

void JsonOArchive::array_item_start(int number) {
  m_archive.push(number);
  m_array_stack.resize(m_array_level);
}

void JsonOArchive::array_item_end() {}

/* ------------------------------ JsonIArchive ---------------------------- */

JsonIArchive::JsonIArchive(nlohmann::json& root) : m_archive(root), m_array_level(0) {}

JsonIArchive::~JsonIArchive() = default;

nlohmann::json& JsonIArchive::read_data() {
  auto& data = m_archive.value();
  const auto idx = m_array_stack.back().index;
  const auto is_array = data.is_array();

  if (idx == 0) {
    if (is_array) {
      return data[0];
    }
  } else {
    if (is_array) {
      return data[idx];
    }
  }
  return data;
}

void JsonIArchive::nvp_start(const std::string& name) {
  if (name.empty())
    return;

  m_archive.push(name);

  const auto number = m_array_stack.size();
  if (number < ++m_array_level) {
    m_array_stack.emplace_back(DataItem{name, 0, false});
  } else {
    const auto is_array = m_archive.value().is_array();
    auto& item = m_array_stack.back();
    if (item.key == name) {
      ++item.index;
      item.pop_flag = true;
      m_archive.push(item.index);
    } else {
      item.index = 0;
      item.key = name;
      item.pop_flag = false;

      if (is_array) {
        item.pop_flag = true;
        m_archive.push(item.index);
      }
    }
  }
}

void JsonIArchive::nvp_end(const std::string& name) {
  if (name.empty())
    return;

  m_archive.pop();
  if (m_array_stack[--m_array_level].pop_flag)
    m_archive.pop();

  m_array_stack.resize(m_array_level + 1);
}

void JsonIArchive::array_start(int number) {
  m_archive.push(number);
  m_array_stack.resize(m_array_level);
}

void JsonIArchive::array_end() {}

} // namespace boost::serialization
