/* ----------------------------------- Local -------------------------------- */
#include "egnite/rest/data_serializer.h"
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

/* ---------------------------- DataSerializer ------------------------------ */

DataSerializer::DataSerializer(QObject* parent) : QObject(parent) {}

DataSerializer::~DataSerializer() = default;

QVariant DataSerializer::deserialize(const Data& data, int type_id) const {
  return std::visit(
      core::utils::overloaded{[&](std::nullopt_t) -> QVariant { return {}; },
                              [&](QJsonValue& data) -> QVariant {
                                auto object = QVariant(QMetaType(type_id));
                                auto archive = serializer::JsonIArchive(data);
                                archive& object;
                                return object;
                              },
                              [&](QCborValue& data) -> QVariant {
                                auto object = QVariant(QMetaType(type_id));
                                auto archive = serializer::CborIArchive(data);
                                archive& object;
                                return object;
                              }},
      const_cast<Data&>(data));
}

}  // namespace egnite::rest