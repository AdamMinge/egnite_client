/* ----------------------------------- Local -------------------------------- */
#include "egnite/config.h"
/* ------------------------------------ Qt ---------------------------------- */
#include <QApplication>
#include <QQmlApplicationEngine>
/* -------------------------------------------------------------------------- */

void setQtEnvironment()
{
  qputenv("QT_AUTO_SCREEN_SCALE_FACTOR", "1");
  qputenv("QT_ENABLE_HIGHDPI_SCALING", "0");
  qputenv("QT_LOGGING_RULES", "qt.qml.connections=false");
  qputenv("QT_QUICK_CONTROLS_CONF", ":/conf/qtquickcontrols2.conf");
}

int main(int argc, char **argv)
{
  setQtEnvironment();

  QApplication app(argc, argv);
  QApplication::setApplicationName(QStringLiteral("Egnite-Editor"));
  QApplication::setApplicationVersion(QLatin1String(EGNITE_VERSION_STR));
  QApplication::setApplicationDisplayName(QStringLiteral("Egnite-Editor"));
  QApplication::setOrganizationName(QStringLiteral("Egnite"));

  QQmlApplicationEngine engine;
  const auto url = QUrl("qrc:/qml/Main.qml");
  QObject::connect(
    &engine, &QQmlApplicationEngine::objectCreated, &app,
    [url](QObject *obj, const QUrl &objUrl) {
      if (!obj && url == objUrl) QCoreApplication::exit(-1);
    },
    Qt::QueuedConnection);

  engine.load(url);
  if (engine.rootObjects().isEmpty()) { return -1; }

  return QApplication::exec();
}
