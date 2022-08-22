#ifndef EGNITE_SERIALIZER_JSON_ARCHIVE_P_H
#define EGNITE_SERIALIZER_JSON_ARCHIVE_P_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QString>
/* ---------------------------------- Standard ------------------------------ */
#include <stack>
#include <variant>
/* ----------------------------------- Local -------------------------------- */
#include "egnite/export.h"
#include "egnite/serializer/json_archive.h"
/* -------------------------------------------------------------------------- */

namespace egnite::serializer::detail {

class JsonArchive {
  using ValueWithKey = struct {
    QJsonValue value;
    std::variant<int, QLatin1String> key;
  };

 public:
  JsonArchive(QJsonValue& root);

  void push(QLatin1String key);
  void push(int number);

  void pop();

  [[nodiscard]] const QJsonValue& value() const;
  [[nodiscard]] QJsonValue& value();

 private:
  QJsonValue& m_root;
  std::stack<ValueWithKey> m_stack;
};

class JsonOArchivePrivate {
 public:
  JsonOArchivePrivate(QJsonValue& root);

  const JsonArchive& getArchive() const;
  JsonArchive& getArchive();

 private:
  JsonArchive m_archive;
};

class JsonIArchivePrivate {
 public:
  JsonIArchivePrivate(QJsonValue& root);

  const JsonArchive& getArchive() const;
  JsonArchive& getArchive();

 private:
  JsonArchive m_archive;
};

}  // namespace egnite::serializer::detail

#endif  // EGNITE_SERIALIZER_JSON_ARCHIVE_P_H