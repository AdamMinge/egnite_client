#ifndef EGNITE_WEB_SERIALIZER_H
#define EGNITE_WEB_SERIALIZER_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QByteArray>
#include <QObject>
/* ---------------------------------- Utils --------------------------------- */
#include <egnite/utils/boost_archive/json_archive.h>
/* ----------------------------------- Local -------------------------------- */
#include "egnite/egnite/export.h"
/* -------------------------------------------------------------------------- */

namespace egnite::web {

class EGNITE_API Serializer : public QObject {
  Q_OBJECT

public:
  enum class Format { Json };

public:
  explicit Serializer(Format format);
  virtual ~Serializer();

  Format getFormat() const;

  template <typename TYPE> QByteArray serialize(const TYPE& value);
  template <typename TYPE> TYPE deserialize(const QByteArray& data);

private:
  Format m_format;
};

class EGNITE_API JsonSerializer : public Serializer {
  Q_OBJECT

public:
  explicit JsonSerializer();
  ~JsonSerializer() override;

  template <typename TYPE> QByteArray serialize(const TYPE& value) {
    nlohmann::json json;
    boost::archive::JsonOArchive ar(json);
    ar << value;
    return QString::fromStdString(json.dump()).toLocal8Bit();
  }
  template <typename TYPE> TYPE deserialize(const QByteArray& data) {
    nlohmann::json json = nlohmann::json::parse(data);
    boost::archive::JsonIArchive ar(json);
    auto value = TYPE{};
    ar >> value;
    return value;
  }
};

template <typename TYPE> QByteArray Serializer::serialize(const TYPE& value) {
  switch (m_format) {
  case Format::Json:
    return dynamic_cast<JsonSerializer*>(this)->serialize(value);
  default:
    Q_ASSERT_X(false, "Serializer::serialize", "unknown format");
  }
}
template <typename TYPE> TYPE Serializer::deserialize(const QByteArray& data) {
  switch (m_format) {
  case Format::Json:
    return dynamic_cast<JsonSerializer*>(this)->deserialize<TYPE>(data);
  default:
    Q_ASSERT_X(false, "Serializer::deserialize", "unknown format");
  }
}

} // namespace egnite::web

#endif // EGNITE_WEB_SERIALIZER_H
