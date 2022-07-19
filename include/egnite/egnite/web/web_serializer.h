#ifndef EGNITE_WEB_SERIALIZER_H
#define EGNITE_WEB_SERIALIZER_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QByteArray>
#include <QObject>
/* ---------------------------------- Utils --------------------------------- */
#include <egnite/utils/json_archive/json_archive.h>
/* ----------------------------------- Local -------------------------------- */
#include "egnite/egnite/export.h"
/* -------------------------------------------------------------------------- */

namespace egnite::web {

class EGNITE_API WebSerializer : public QObject {
  Q_OBJECT

public:
  enum class Format { Json };

public:
  explicit WebSerializer(Format format);
  virtual ~WebSerializer();

  Format getFormat() const;

  template <typename TYPE> QByteArray serialize(const TYPE& value);
  template <typename TYPE> TYPE deserialize(const QByteArray& data);

private:
  Format m_format;
};

class EGNITE_API JsonSerializer : public WebSerializer {
  Q_OBJECT

public:
  explicit JsonSerializer();
  ~JsonSerializer() override;

  template <typename TYPE> QByteArray serialize(const TYPE& value) {
    nlohmann::json json;
    boost::serialization::JsonOArchive ar(json);
    ar << value;
    return QString::fromStdString(json.dump()).toLocal8Bit();
  }
  template <typename TYPE> TYPE deserialize(const QByteArray& data) {
    nlohmann::json json = nlohmann::json::parse(data);
    boost::serialization::JsonIArchive ar(json);
    auto value = TYPE{};
    ar >> value;
    return value;
  }
};

template <typename TYPE> QByteArray WebSerializer::serialize(const TYPE& value) {
  switch (m_format) {
  case Format::Json:
    return dynamic_cast<JsonSerializer*>(this)->serialize(value);
  default:
    Q_ASSERT_X(false, "WebSerializer::serialize", "unknown format");
  } 
}
template <typename TYPE> TYPE WebSerializer::deserialize(const QByteArray& data) {
  switch (m_format) {
  case Format::Json:
    return dynamic_cast<JsonSerializer*>(this)->deserialize<TYPE>(data);
  default:
    Q_ASSERT_X(false, "WebSerializer::deserialize", "unknown format");
  }
}

} // namespace egnite::web

#endif // EGNITE_WEB_SERIALIZER_H
