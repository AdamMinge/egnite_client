#ifndef TEST_WEB_SERIALIZER_H
#define TEST_WEB_SERIALIZER_H

/* ----------------------------------- GTest -------------------------------- */
#include <gtest/gtest.h>
/* ---------------------------------- Standard ------------------------------ */
#include <memory>
/* ----------------------------------- Egnite ------------------------------- */
#include <egnite/egnite/web/headers.h>
/* -------------------------------------------------------------------------- */

class TestHeaders : public testing::Test {
protected:
  using KnownHeaders = egnite::web::Headers::KnownHeaders;

protected:
  TestHeaders() = default;
  ~TestHeaders() override = default;

  void SetUp() override {}
  void TearDown() override {}
};

#endif // TEST_WEB_SERIALIZER_H
