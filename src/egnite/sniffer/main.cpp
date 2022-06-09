/* ----------------------------------- Local -------------------------------- */
#include "egnite/config.h"
/* ------------------------------------ Qt ---------------------------------- */
#include <QApplication>
#include <QFontDatabase>
#include <QQmlApplicationEngine>
/* -------------------------------------------------------------------------- */

int main(int argc, char **argv)
{
  QApplication app(argc, argv);
  QApplication::setApplicationName(QStringLiteral("Egnite-Sniffer"));
  QApplication::setApplicationVersion(QLatin1String(EGNITE_VERSION_STR));
  QApplication::setApplicationDisplayName(QStringLiteral("Egnite-Sniffer"));
  QApplication::setOrganizationName(QStringLiteral("Egnite"));

  QQmlApplicationEngine engine;
  const auto url = QUrl("qrc:/qml/Main.qml");
  QObject::connect(
    &engine, &QQmlApplicationEngine::objectCreated, &app,
    [url](QObject *obj, const QUrl &objUrl) {
      if (!obj && url == objUrl) QCoreApplication::exit(-1);
    },
    Qt::QueuedConnection);

  engine.addImportPath("qrc:/");
  engine.load(url);
  if (engine.rootObjects().isEmpty()) { QCoreApplication::exit(-1); }

  return QApplication::exec();
}
