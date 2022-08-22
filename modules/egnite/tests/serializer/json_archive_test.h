#ifndef EGNITE_SERIALIZER_TEST_JSON_ARCHIVE_H
#define EGNITE_SERIALIZER_TEST_JSON_ARCHIVE_H

/* -------------------------------- GoogleTest ------------------------------ */
#include <gtest/gtest.h>
/* -------------------------------------------------------------------------- */

class JsonOArchiveTest : public testing::Test {
 protected:
  JsonOArchiveTest() = default;
  ~JsonOArchiveTest() override = default;

  void SetUp() override {}
  void TearDown() override {}
};

class JsonIArchiveTest : public testing::Test {
 protected:
  JsonIArchiveTest() = default;
  ~JsonIArchiveTest() override = default;

  void SetUp() override {}
  void TearDown() override {}
};

#endif  // EGNITE_SERIALIZER_TEST_JSON_ARCHIVE_H