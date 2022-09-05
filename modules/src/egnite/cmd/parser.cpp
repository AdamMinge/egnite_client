/* ----------------------------------- Local -------------------------------- */
#include "egnite/cmd/parser.h"

#include "egnite/cmd/detail/parser_p.h"
/* ------------------------------------ Qt ---------------------------------- */
#include <QCommandLineParser>
/* -------------------------------------------------------------------------- */

namespace egnite::cmd {

/* --------------------------------- Parser --------------------------------- */

Parser::Parser() : m_impl(std::make_unique<detail::ParserPrivate>()) {}

Parser::~Parser() = default;

void Parser::process(const QCoreApplication &app) { m_impl->process(app); }

void Parser::registerOption(const QStringList &names,
                            const QString &description,
                            const std::function<void()> &callback) {
  m_impl->registerOption(names, description, [callback](const QString &value) {
    Q_UNUSED(value);
    callback();
  });
}

template <>
void Parser::registerOption(
    const QStringList &names, const QString &description,
    const std::function<void(const QString &)> &callback,
    const QString &valueName) {
  m_impl->registerOption(names, description, callback, valueName);
}

/* ----------------------------- ParserPrivate ------------------------------ */

namespace detail {

ParserPrivate::Option::Option(const QCommandLineOption &cmd_option,
                              std::function<void(const QString &)> callback)
    : cmd_option(cmd_option), callback(std::move(callback)) {}

ParserPrivate::ParserPrivate() = default;

ParserPrivate::~ParserPrivate() = default;

void ParserPrivate::process(const QCoreApplication &app) {
  QCommandLineParser parser;
  parser.setApplicationDescription(app.applicationName());
  parser.addHelpOption();
  parser.addVersionOption();

  for (auto &option : m_options) parser.addOption(option.cmd_option);

  parser.process(app);

  for (auto &option : m_options) {
    if (parser.isSet(option.cmd_option)) {
      auto value = QString{};
      if (!option.cmd_option.valueName().isEmpty())
        value = parser.value(option.cmd_option);

      option.callback(value);
    }
  }
}

void ParserPrivate::registerOption(
    const QStringList &names, const QString &description,
    const std::function<void(const QString &)> &callback,
    const QString &valueName) {
  Q_ASSERT(!names.isEmpty());
  Q_ASSERT(!description.isEmpty());

  auto cmd_option = QCommandLineOption(names, description, valueName);
  m_options.emplace_back(Option(cmd_option, callback));
}

}  // namespace detail

}  // namespace egnite::cmd
