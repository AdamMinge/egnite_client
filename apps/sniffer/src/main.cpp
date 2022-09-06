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
  qputenv("QT_QUICK_CONTROLS_CONF", ":/sniffer/qtquickcontrols2.conf");
}

/* ----------------------------- CommandLineParser -------------------------- */

class SnifferCommandLineParser : public egnite::cmd::Parser {
 public:
  explicit SnifferCommandLineParser();
  ~SnifferCommandLineParser() override;
};

SnifferCommandLineParser::SnifferCommandLineParser() = default;

SnifferCommandLineParser::~SnifferCommandLineParser() = default;

/* ----------------------------- parseCommandLine --------------------------- */

static void parseCommandLine(QGuiApplication &app) {
  SnifferCommandLineParser parser;
  parser.process(app);
}

/* ------------------------------------ main -------------------------------- */

int main(int argc, char **argv) {
  initEnvironmentVariables();

  QGuiApplication app(argc, argv);
  QGuiApplication::setApplicationName(QStringLiteral("Egnite-Sniffer"));
  QGuiApplication::setApplicationVersion(QLatin1String(EGNITE_VERSION_STR));
  QGuiApplication::setApplicationDisplayName(QStringLiteral("Egnite-Sniffer"));
  QGuiApplication::setOrganizationName(QStringLiteral("Egnite"));

  parseCommandLine(app);

  QQmlApplicationEngine engine;
  const auto url = QUrl("qrc:/sniffer/Main.qml");
  QObject::connect(
      &engine, &QQmlApplicationEngine::objectCreated, &app,
      [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl) QCoreApplication::exit(-1);
      },
      Qt::QueuedConnection);

  engine.addImportPath(app.applicationDirPath() + "/plugins");
  engine.load(url);
  if (engine.rootObjects().isEmpty()) {
    QCoreApplication::exit(-1);
  }

  return QGuiApplication::exec();
}
