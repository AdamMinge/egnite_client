/* ----------------------------------- Local -------------------------------- */
#include "egnite/config.h"
/* ---------------------------------- Egnite -------------------------------- */
#include <egnite/egnite/quick_egnite.h>
/* ---------------------------------- Utils --------------------------------- */
#include <egnite/utils/cmd/parser.h>
/* ------------------------------------ Qt ---------------------------------- */
#include <QApplication>
#include <QFontDatabase>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
/* -------------------------------------------------------------------------- */

/* ------------------------- initEnvironmentVariables ----------------------- */

void initEnvironmentVariables() { qputenv("QT_QUICK_CONTROLS_CONF", ":/conf/qtquickcontrols2.conf"); }

/* ----------------------------- CommandLineParser -------------------------- */

class ViewerCommandLineParser : public cmd::Parser {
public:
  explicit ViewerCommandLineParser();
  ~ViewerCommandLineParser() override;
};

ViewerCommandLineParser::ViewerCommandLineParser() = default;

ViewerCommandLineParser::~ViewerCommandLineParser() = default;

/* ----------------------------- parseCommandLine --------------------------- */

static void parseCommandLine(QApplication &app) {
  ViewerCommandLineParser parser;
  parser.process(app);
}

/* ------------------------------------ main -------------------------------- */

int main(int argc, char **argv) {
  initEnvironmentVariables();

  QApplication app(argc, argv);
  QApplication::setApplicationName(QStringLiteral("Egnite-Viewer"));
  QApplication::setApplicationVersion(QLatin1String(EGNITE_VERSION_STR));
  QApplication::setApplicationDisplayName(QStringLiteral("Egnite-Viewer"));
  QApplication::setOrganizationName(QStringLiteral("Egnite"));

  parseCommandLine(app);

  QQmlApplicationEngine engine;
  egnite::QuickEgnite::getInstance().registerTypes();
  egnite::QuickEgnite::getInstance().initializeEngine(&engine);

  const auto url = QUrl("qrc:/qml/Main.qml");
  QObject::connect(
      &engine, &QQmlApplicationEngine::objectCreated, &app,
      [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
          QCoreApplication::exit(-1);
      },
      Qt::QueuedConnection);

  engine.addImportPath("./plugins");
  engine.load(url);
  if (engine.rootObjects().isEmpty()) {
    QCoreApplication::exit(-1);
  }

  return QApplication::exec();
}
