#ifndef EGNITE_UTILS_TEST_BINDER_H
#define EGNITE_UTILS_TEST_BINDER_H

/* -------------------------------- GoogleTest ------------------------------ */
#include <gtest/gtest.h>
/* -------------------------------------------------------------------------- */

class BinderTest : public testing::Test {
 protected:
  BinderTest() = default;
  ~BinderTest() override = default;

  void SetUp() override {}
  void TearDown() override {}
};

#endif  // EGNITE_UTILS_TEST_BINDER_H