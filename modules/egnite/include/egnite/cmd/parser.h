#ifndef EGNITE_CMD_PARSER_H
#define EGNITE_CMD_PARSER_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QApplication>
#include <QCommandLineOption>
#include <QCommandLineParser>
/* ---------------------------------- Standard ------------------------------ */
#include <list>
/* ----------------------------------- Local -------------------------------- */
#include "egnite/export.h"
/* -------------------------------------------------------------------------- */

namespace egnite::cmd {
class EGNITE_API Parser {
 private:
  struct Option {
    explicit Option(const QCommandLineOption &cmd_option,
                    std::function<void(const QString &)> callback = {});

    QCommandLineOption cmd_option;
    std::function<void(const QString &)> callback;
  };

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

  template <typename TYPE>
  [[nodiscard]] TYPE convertValue(const QString &value);

 private:
  void registerOptionImpl(const QStringList &names, const QString &description,
                          const std::function<void(const QString &)> &callback,
                          const QString &valueName = QString{});

 private:
  std::list<Option> m_options;
};

template <typename TYPE>
void Parser::registerOption(const QStringList &names,
                            const QString &description,
                            const std::function<void(const TYPE &)> &callback,
                            const QString &valueName) {
  registerOptionImpl(
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

CONVERTER_VALUE_SPEC(QString, value)
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
