#ifndef EGNITE_QUICK_EGNITE_H
#define EGNITE_QUICK_EGNITE_H

/* ------------------------------------ Qml --------------------------------- */
#include <QQmlEngine>
/* ----------------------------------- Local -------------------------------- */
#include "egnite/export.h"
/* -------------------------------------------------------------------------- */

namespace egnite {

class EGNITE_API QuickEgnite : public QObject {
 public:
  [[nodiscard]] static QuickEgnite &getInstance();
  static void deleteInstance();

 public:
  ~QuickEgnite() override;

  void registerTypes();
  void initializeEngine(QQmlEngine *engine);

 protected:
  explicit QuickEgnite();

  void registerRestModule();

 private:
  static QScopedPointer<QuickEgnite> m_instance;
};

}  // namespace egnite

#endif  // EGNITE_QUICK_EGNITE_H
