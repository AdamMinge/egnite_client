#ifndef TEST_WEB_SERIALIZER_H
#define TEST_WEB_SERIALIZER_H

/* ----------------------------------- GTest -------------------------------- */
#include <gtest/gtest.h>
/* ---------------------------------- Standard ------------------------------ */
#include <memory>
/* ----------------------------------- Egnite ------------------------------- */
#include <egnite/egnite/web/messages.h>
#include <egnite/egnite/web/serializer.h>
/* -------------------------------------------------------------------------- */

class TestJsonSerializer : public testing::Test {
protected:
  TestJsonSerializer() = default;
  ~TestJsonSerializer() override = default;

  void SetUp() override { m_serializer = std::make_unique<egnite::web::JsonSerializer>(); }
  void TearDown() override { m_serializer.reset(); }

protected:
  std::unique_ptr<egnite::web::Serializer> m_serializer;
};

#endif // TEST_WEB_SERIALIZER_H
