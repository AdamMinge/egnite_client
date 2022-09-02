#ifndef EGNITE_CMD_PARSER_H
#define EGNITE_CMD_PARSER_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QCoreApplication>
/* --------------------------------- Standard ------------------------------- */
#include <memory>
/* ----------------------------------- Local -------------------------------- */
#include "egnite/export.h"
/* -------------------------------------------------------------------------- */

namespace egnite::cmd {

namespace detail {
class ParserPrivate;
}

class EGNITE_API Parser {
 public:
  explicit Parser();
  virtual ~Parser();

  void process(const QCoreApplication &app);

 protected:
  void registerOption(const QStringList &names, const QString &description,
                      const std::function<void()> &callback);

  template <typename TYPE>
  void registerOption(const QStringList &names, const QString &description,
                      const std::function<void(const TYPE &)> &callback,
                      const QString &valueName);

 private:
  template <typename TYPE>
  [[nodiscard]] TYPE convertValue(const QString &value);

 private:
  std::unique_ptr<detail::ParserPrivate> m_impl;
};

template <typename TYPE>
void Parser::registerOption(const QStringList &names,
                            const QString &description,
                            const std::function<void(const TYPE &)> &callback,
                            const QString &valueName) {
  registerOption(
      names, description,
      [callback, this](const QString &value) {
        callback(convertValue<TYPE>(value));
      },
      valueName);
}

/* -------------------------- Convert specializations  ----------------------
 */

template <typename TYPE>
TYPE Parser::convertValue(const QString &value) {
  return static_cast<TYPE>(value);
}

#define CONVERTER_VALUE_SPEC(TYPE, METHOD)                       \
  template <>                                                    \
  inline TYPE Parser::convertValue<TYPE>(const QString &value) { \
    return (METHOD);                                             \
  }

CONVERTER_VALUE_SPEC(short, value.toShort())
CONVERTER_VALUE_SPEC(ushort, value.toUShort())
CONVERTER_VALUE_SPEC(int, value.toInt())
CONVERTER_VALUE_SPEC(uint, value.toUInt())
CONVERTER_VALUE_SPEC(long, value.toLong())
CONVERTER_VALUE_SPEC(ulong, value.toULong())
CONVERTER_VALUE_SPEC(qlonglong, value.toLongLong())
CONVERTER_VALUE_SPEC(qulonglong, value.toULongLong())
CONVERTER_VALUE_SPEC(float, value.toFloat())
CONVERTER_VALUE_SPEC(double, value.toDouble())
#undef CONVERTER_VALUE_SPEC

}  // namespace egnite::cmd

#endif  // EGNITE_CMD_PARSER_H
