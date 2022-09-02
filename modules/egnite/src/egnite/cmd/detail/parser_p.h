#ifndef EGNITE_CMD_PARSER_P_H
#define EGNITE_CMD_PARSER_P_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QCommandLineOption>
/* ---------------------------------- Standard ------------------------------ */
#include <list>
/* ------------------------------------ Local ------------------------------- */
#include "egnite/cmd/parser.h"
/* -------------------------------------------------------------------------- */

namespace egnite::cmd {

namespace detail {

class ParserPrivate {
 private:
  struct Option {
    explicit Option(const QCommandLineOption &cmd_option,
                    std::function<void(const QString &)> callback = {});

    QCommandLineOption cmd_option;
    std::function<void(const QString &)> callback;
  };

 public:
  explicit ParserPrivate();
  ~ParserPrivate();

  void process(const QCoreApplication &app);
  void registerOption(const QStringList &names, const QString &description,
                      const std::function<void(const QString &)> &callback,
                      const QString &valueName = QString{});

 private:
  std::list<Option> m_options;
};

}  // namespace detail

}  // namespace egnite::cmd

#endif  // EGNITE_CMD_PARSER_P_H