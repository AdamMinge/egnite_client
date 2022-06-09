/* ----------------------------------- Local -------------------------------- */
#include "egnite/egnite/quick_egnite.h"
/* ------------------------------------ Qt ---------------------------------- */
#include <QFontDatabase>
/* -------------------------------------------------------------------------- */

QScopedPointer<QuickEgnite> QuickEgnite::m_instance =
  QScopedPointer<QuickEgnite>(nullptr);

QuickEgnite &QuickEgnite::getInstance()
{
  if (m_instance.isNull()) m_instance.reset(new QuickEgnite);

  return *m_instance;
}

void QuickEgnite::deleteInstance() { m_instance.reset(nullptr); }

QuickEgnite::QuickEgnite() = default;

QuickEgnite::~QuickEgnite() = default;

void QuickEgnite::registerTypes()
{
  Q_INIT_RESOURCE(egnite);

  qmlRegisterType(
    QUrl("qrc:/qml/auth/AuthWindow.qml"), "egnite", 1, 0, "AuthWindow");
  qmlRegisterType(
    QUrl("qrc:/qml/auth/LoginPage.qml"), "egnite", 1, 0, "LoginPage");
  qmlRegisterType(
    QUrl("qrc:/qml/auth/SignUpPage.qml"), "egnite", 1, 0, "SignUpPage");

  QFontDatabase::addApplicationFont(":/font/FontAwesome.ttf");
}

void QuickEgnite::initializeEngine(QQmlEngine *engine) {}
