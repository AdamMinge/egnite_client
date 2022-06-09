#ifndef EGNITE_QUICK_EGNITE_H
#define EGNITE_QUICK_EGNITE_H

/* ------------------------------------ Qml --------------------------------- */
#include <QQmlEngine>
/* ----------------------------------- Local -------------------------------- */
#include "egnite/egnite/export.h"
/* -------------------------------------------------------------------------- */

class EGNITE_API QuickEgnite : public QObject
{
public:
  [[nodiscard]] static QuickEgnite &getInstance();
  static void deleteInstance();

public:
  ~QuickEgnite() override;

  void registerTypes();
  void initializeEngine(QQmlEngine *engine);

protected:
  explicit QuickEgnite();

private:
  static QScopedPointer<QuickEgnite> m_instance;
};

#endif// EGNITE_QUICK_EGNITE_H
