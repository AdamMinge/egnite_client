#ifndef EGNITE_SERIALIZER_CBOR_ARCHIVE_P_H
#define EGNITE_SERIALIZER_CBOR_ARCHIVE_P_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QString>
/* ---------------------------------- Standard ------------------------------ */
#include <stack>
#include <variant>
/* ----------------------------------- Local -------------------------------- */
#include "egnite/serializer/cbor_archive.h"
#include "egnite/serializer/export.h"
/* -------------------------------------------------------------------------- */

namespace egnite::serializer::detail {

class CborArchive {
  using ValueWithKey = struct {
    QCborValue value;
    std::variant<int, QLatin1String> key;
  };

 public:
  explicit CborArchive(QCborValue& root);

  void push(QLatin1String key);
  void push(int number);

  void pop();

  [[nodiscard]] const QCborValue& value() const;
  [[nodiscard]] QCborValue& value();

 private:
  QCborValue& m_root;
  std::stack<ValueWithKey> m_stack;
};

class CborOArchivePrivate {
 public:
  explicit CborOArchivePrivate(QCborValue& root);

  const CborArchive& getArchive() const;
  CborArchive& getArchive();

 private:
  CborArchive m_archive;
};

class CborIArchivePrivate {
 public:
  explicit CborIArchivePrivate(QCborValue& root);

  const CborArchive& getArchive() const;
  CborArchive& getArchive();

 private:
  CborArchive m_archive;
};

}  // namespace egnite::serializer::detail

#endif  // EGNITE_SERIALIZER_CBOR_ARCHIVE_P_H