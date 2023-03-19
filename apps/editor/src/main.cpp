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
  qputenv("QT_QUICK_CONTROLS_CONF", ":/editor/qtquickcontrols2.conf");
}

/* ----------------------------- CommandLineParser -------------------------- */

class EditorCommandLineParser : public egnite::cmd::Parser {
 public:
  explicit EditorCommandLineParser();
  ~EditorCommandLineParser() override;
};

EditorCommandLineParser::EditorCommandLineParser() = default;

EditorCommandLineParser::~EditorCommandLineParser() = default;

/* ----------------------------- parseCommandLine --------------------------- */

static void parseCommandLine(QGuiApplication &app) {
  EditorCommandLineParser parser;
  parser.process(app);
}

/* ------------------------------------ main -------------------------------- */

int main(int argc, char **argv) {
  initEnvironmentVariables();

  QGuiApplication app(argc, argv);
  QGuiApplication::setApplicationName(QStringLiteral("Egnite-Editor"));
  QGuiApplication::setApplicationVersion(QLatin1String(EGNITE_VERSION_STR));
  QGuiApplication::setApplicationDisplayName(QStringLiteral("Egnite-Editor"));
  QGuiApplication::setOrganizationName(QStringLiteral("Egnite"));

  parseCommandLine(app);

  QQmlApplicationEngine engine;
  const auto url = QUrl("qrc:/editor/MainWindow.qml");
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
