/* ---------------------------------- Egnite -------------------------------- */
#include <egnite/cmd/parser.h>
#include <egnite/config.h>
/* ------------------------------------ Qt ---------------------------------- */
#include <QFontDatabase>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
/* -------------------------------------------------------------------------- */

/* ------------------------- initEnvironmentVariables ----------------------- */

void initEnvironmentVariables() {
  qputenv("QT_QUICK_CONTROLS_CONF", ":/viewer/qtquickcontrols2.conf");
}

/* ----------------------------- CommandLineParser -------------------------- */

class ViewerCommandLineParser : public egnite::cmd::Parser {
 public:
  explicit ViewerCommandLineParser();
  ~ViewerCommandLineParser() override;
};

ViewerCommandLineParser::ViewerCommandLineParser() = default;

ViewerCommandLineParser::~ViewerCommandLineParser() = default;

/* ----------------------------- parseCommandLine --------------------------- */

static void parseCommandLine(QGuiApplication &app) {
  ViewerCommandLineParser parser;
  parser.process(app);
}

/* ------------------------------------ main -------------------------------- */

int main(int argc, char **argv) {
  initEnvironmentVariables();

  QGuiApplication app(argc, argv);
  QGuiApplication::setApplicationName(QStringLiteral("Egnite-Viewer"));
  QGuiApplication::setApplicationVersion(QLatin1String(EGNITE_VERSION_STR));
  QGuiApplication::setApplicationDisplayName(QStringLiteral("Egnite-Viewer"));
  QGuiApplication::setOrganizationName(QStringLiteral("Egnite"));

  parseCommandLine(app);

  QQmlApplicationEngine engine;
  const auto url = QUrl("qrc:/viewer/Main.qml");
  QObject::connect(
      &engine, &QQmlApplicationEngine::objectCreated, &app,
      [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl) QCoreApplication::exit(-1);
      },
      Qt::QueuedConnection);

  engine.addImportPath("./plugins");
  engine.load(url);
  if (engine.rootObjects().isEmpty()) {
    QCoreApplication::exit(-1);
  }

  return QGuiApplication::exec();
}
