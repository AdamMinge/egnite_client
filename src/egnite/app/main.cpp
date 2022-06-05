/* ----------------------------------- Local -------------------------------- */
#include "egnite/config.h"
/* ------------------------------------ Qt ---------------------------------- */
#include <QApplication>
#include <QQmlApplicationEngine>
/* -------------------------------------------------------------------------- */


int main(int argc, char **argv)
{
  QApplication app(argc, argv);
  QApplication::setApplicationName(QStringLiteral("Egnite-Editor"));
  QApplication::setApplicationVersion(QLatin1String(EGNITE_VERSION_STR));
  QApplication::setApplicationDisplayName(QStringLiteral("Egnite-Editor"));
  QApplication::setOrganizationName(QStringLiteral("Egnite"));

  QQmlApplicationEngine engine;
  engine.load(QUrl("qrc:/qml/main.qml"));

  return QApplication::exec();
}
