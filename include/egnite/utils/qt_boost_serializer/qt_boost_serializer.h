#ifndef EGNITE_QT_BOOST_SERIALIZER_H
#define EGNITE_QT_BOOST_SERIALIZER_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QByteArray>
#include <QString>
/* ----------------------------------- Local -------------------------------- */
#include "egnite/utils/qt_boost_serializer/export.h"
/* -------------------------------------------------------------------------- */

namespace boost::serialization {

/* ---------------------------------- QString ----------------------------- */

template <class Archive> void save(Archive& ar, const QString& str, const unsigned int) {
  ar& str.toStdString();
}

template <class Archive> void load(Archive& ar, QString& str, const unsigned int) {
  std::string stdString;
  ar& stdString;
  str = str.fromStdString(stdString);
}

template <class Archive>
void serialize(Archive& ar, QString& str, const unsigned int file_version) {
  split_free(ar, str, file_version);
}

/* --------------------------------- QByteArray --------------------------- */

template <class Archive> void save(Archive& ar, const QByteArray& array, const unsigned int) {
  ar& array.toStdString();
}

template <class Archive> void load(Archive& ar, QByteArray& array, const unsigned int) {
  std::string stdString;
  ar& stdString;
  array = array.fromStdString(stdString);
}

template <class Archive>
void serialize(Archive& ar, QByteArray& array, const unsigned int file_version) {
  split_free(ar, array, file_version);
}

} // namespace boost::serialization

#endif // EGNITE_QT_BOOST_SERIALIZER_H
