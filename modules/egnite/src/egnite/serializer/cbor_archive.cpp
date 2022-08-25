/* ----------------------------------- Local -------------------------------- */
#include "egnite/serializer/cbor_archive.h"

#include "egnite/serializer/detail/cbor_archive_p.h"
/* -------------------------------------------------------------------------- */

namespace egnite::serializer {

/* -------------------------------- CborOArchive ---------------------------- */

CborOArchive::CborOArchive()
    : m_impl(std::make_unique<detail::CborOArchivePrivate>()) {}

CborOArchive::~CborOArchive() {}

void CborOArchive::save_start(QLatin1String name) {}

void CborOArchive::save_end(QLatin1String name) {}

void CborOArchive::array_item(int number) {}

void CborOArchive::array_end() {}

void CborOArchive::save_override(const boost::archive::class_name_type& t) {}

void CborOArchive::save_override(const boost::archive::version_type& t) {}

void CborOArchive::save_override(const boost::archive::object_id_type& t) {}

void CborOArchive::save_override(
    const boost::archive::object_reference_type& t) {}

void CborOArchive::save_override(const boost::archive::class_id_type& t) {}

void CborOArchive::save_override(
    const boost::archive::class_id_optional_type& t) {}

void CborOArchive::save_override(
    const boost::archive::class_id_reference_type& t) {}

void CborOArchive::save_override(const boost::archive::tracking_type& t) {}

/* ------------------------------- CborIArchive ----------------------------- */

CborIArchive::CborIArchive()
    : m_impl(std::make_unique<detail::CborIArchivePrivate>()) {}

CborIArchive::~CborIArchive() {}

void CborIArchive::load_start(QLatin1String name) {}

void CborIArchive::load_end(QLatin1String name) {}

void CborIArchive::array_item(int number) {}

void CborIArchive::array_end() {}

void CborIArchive::load_override(boost::archive::class_name_type& t) {}

void CborIArchive::load_override(boost::archive::version_type& t) {}

void CborIArchive::load_override(boost::archive::object_id_type& t) {}

void CborIArchive::load_override(boost::archive::object_reference_type& t) {}

void CborIArchive::load_override(boost::archive::class_id_type& t) {}

void CborIArchive::load_override(boost::archive::class_id_optional_type& t) {}

void CborIArchive::load_override(boost::archive::class_id_reference_type& t) {}

void CborIArchive::load_override(boost::archive::tracking_type& t) {}

/* ---------------------------- CborOArchivePrivate ------------------------- */

namespace detail {

/* ---------------------------- CborIArchivePrivate ------------------------- */

}  // namespace detail

}  // namespace egnite::serializer
