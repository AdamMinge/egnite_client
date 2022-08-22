/* ----------------------------------- Local -------------------------------- */
#include "egnite/serializer/cbor_archive.h"

#include "egnite/serializer/detail/cbor_archive_p.h"
/* -------------------------------------------------------------------------- */

namespace egnite::serializer {

/* -------------------------------- CborOArchive ---------------------------- */

CborOArchive::CborOArchive()
    : m_impl(std::make_unique<detail::CborOArchivePrivate>()) {}

CborOArchive::~CborOArchive() {}

void CborOArchive::save_start(QStringView name) {}

void CborOArchive::save_end(QStringView name) {}

void CborOArchive::array_item(int number) {}

void CborOArchive::array_end() {}

/* ------------------------------- CborIArchive ----------------------------- */

CborIArchive::CborIArchive()
    : m_impl(std::make_unique<detail::CborIArchivePrivate>()) {}

CborIArchive::~CborIArchive() {}

void CborIArchive::load_start(QStringView name) {}

void CborIArchive::load_end(QStringView name) {}

void CborIArchive::array_item(int number) {}

void CborIArchive::array_end() {}

/* ---------------------------- CborOArchivePrivate ------------------------- */

namespace detail {

/* ---------------------------- CborIArchivePrivate ------------------------- */

}  // namespace detail

}  // namespace egnite::serializer
