/* ---------------------------------- Egnite -------------------------------- */
#include <egnite/cmd/parser.h>
#include <egnite/config.h>
/* ------------------------------------ Qt ---------------------------------- */
#include <QFontDatabase>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
/* -------------------------------- QuickQanava ----------------------------- */
#include <QuickQanava.h>
/* -------------------------------------------------------------------------- */

/* ------------------------- initEnvironmentVariables ----------------------- */

void initEnvironmentVariables() {
  qputenv("QT_QUICK_CONTROLS_CONF", ":/flow/qtquickcontrols2.conf");
}

/* ----------------------------- CommandLineParser -------------------------- */

class FlowCommandLineParser : public egnite::cmd::Parser {
 public:
  explicit FlowCommandLineParser();
  ~FlowCommandLineParser() override;
};

FlowCommandLineParser::FlowCommandLineParser() = default;

FlowCommandLineParser::~FlowCommandLineParser() = default;

/* ----------------------------- parseCommandLine --------------------------- */

static void parseCommandLine(QGuiApplication &app) {
  FlowCommandLineParser parser;
  parser.process(app);
}

/* ------------------------------------ main -------------------------------- */

int main(int argc, char **argv) {
  initEnvironmentVariables();

  QGuiApplication app(argc, argv);
  QGuiApplication::setApplicationName(QStringLiteral("Egnite-Flow"));
  QGuiApplication::setApplicationVersion(QLatin1String(EGNITE_VERSION_STR));
  QGuiApplication::setApplicationDisplayName(QStringLiteral("Egnite-Flow"));
  QGuiApplication::setOrganizationName(QStringLiteral("Egnite"));

  parseCommandLine(app);

  QQmlApplicationEngine engine;
  const auto url = QUrl("qrc:/flow/MainWindow.qml");
  QObject::connect(
      &engine, &QQmlApplicationEngine::objectCreated, &app,
      [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl) QCoreApplication::exit(-1);
      },
      Qt::QueuedConnection);

  engine.addImportPath(app.applicationDirPath() + "/plugins");
  QuickQanava::initialize(&engine);
  engine.load(url);
  if (engine.rootObjects().isEmpty()) {
    QCoreApplication::exit(-1);
  }

  return QGuiApplication::exec();
}
