/* ----------------------------------- Local -------------------------------- */
#include "binder_test.h"
/* ----------------------------------- Egnite ------------------------------- */
#include <egnite/utils/binder.h>
/* -------------------------------------------------------------------------- */

/* --------------------------- From Lambda to Lambda ------------------------ */

TEST_F(BinderTest, bindFromLambdaToLambdaWithoutArgs) {
  auto src_lambda = [](const std::string&, int, bool, char) {};
  auto bind = egnite::utils::bindCallback<decltype(src_lambda)>([]() {});
  bind("string", 1, true, 'z');

  ASSERT_TRUE(true);
}

TEST_F(BinderTest, bindFromLambdaToLambdaWithAllArgs) {
  auto src_lambda = [](const std::string&, int, bool, char) {};

  std::tuple<std::string, int, bool, char> values;
  auto bind = egnite::utils::bindCallback<decltype(src_lambda)>(
      [&values](const std::string& str, int nr, bool flag, char c) {
        std::get<0>(values) = str;
        std::get<1>(values) = nr;
        std::get<2>(values) = flag;
        std::get<3>(values) = c;
      });
  bind("string", 1, true, 'z');

  ASSERT_EQ(std::get<0>(values), "string");
  ASSERT_EQ(std::get<1>(values), 1);
  ASSERT_EQ(std::get<2>(values), true);
  ASSERT_EQ(std::get<3>(values), 'z');
}

TEST_F(BinderTest, bindFromLambdaToLambdaWithSomeArgs) {
  auto src_lambda = [](const std::string&, int, bool, char) {};

  std::tuple<std::string, int> values;
  auto bind = egnite::utils::bindCallback<decltype(src_lambda)>(
      [&values](const std::string& str, int nr) {
        std::get<0>(values) = str;
        std::get<1>(values) = nr;
      });
  bind("string", 1, true, 'z');

  ASSERT_EQ(std::get<0>(values), "string");
  ASSERT_EQ(std::get<1>(values), 1);
}

/* ------------------------ From Lambda to Class Method --------------------- */

TEST_F(BinderTest, bindFromLambdaToClassMethodWithoutArgs) {
  auto src_lambda = [](const std::string&, int, bool, char) {};

  struct ClassWithMethod {
    void operator()() const {};
  };

  const auto cvm = ClassWithMethod{};
  auto bind = egnite::utils::bindCallback<decltype(src_lambda)>(cvm);
  bind("string", 1, true, 'z');

  ASSERT_TRUE(true);
}

TEST_F(BinderTest, bindFromLambdaToClassMethodWithAllArgs) {
  auto src_lambda = [](const std::string&, int, bool, char) {};

  struct ClassWithMethod {
    void operator()(const std::string& str, int nr, bool flag, char c) {
      std::get<0>(values) = str;
      std::get<1>(values) = nr;
      std::get<2>(values) = flag;
      std::get<3>(values) = c;
    };

    std::tuple<std::string, int, bool, char> values;
  };

  auto cvm = ClassWithMethod{};
  auto bind = egnite::utils::bindCallback<decltype(src_lambda)>(cvm);
  bind("string", 1, true, 'z');

  ASSERT_EQ(std::get<0>(cvm.values), "string");
  ASSERT_EQ(std::get<1>(cvm.values), 1);
  ASSERT_EQ(std::get<2>(cvm.values), true);
  ASSERT_EQ(std::get<3>(cvm.values), 'z');
}

TEST_F(BinderTest, bindFromLambdaToClassMethodWithSomeArgs) {
  auto src_lambda = [](const std::string&, int, bool, char) {};

  struct ClassWithMethod {
    void operator()(const std::string& str, int nr) {
      std::get<0>(values) = str;
      std::get<1>(values) = nr;
    };

    std::tuple<std::string, int> values;
  };

  auto cvm = ClassWithMethod{};
  auto bind = egnite::utils::bindCallback<decltype(src_lambda)>(cvm);
  bind("string", 1, true, 'z');

  ASSERT_EQ(std::get<0>(cvm.values), "string");
  ASSERT_EQ(std::get<1>(cvm.values), 1);
}

/* ------------------------ From Class Method to Lambda --------------------- */

TEST_F(BinderTest, bindFromClassMethodToLambdaWithoutArgs) {
  struct ClassWithMethod {
    void operator()(const std::string& str, int nr, bool flag, char c){};
  };

  auto bind = egnite::utils::bindCallback<ClassWithMethod>([]() {});
  bind("string", 1, true, 'z');

  ASSERT_TRUE(true);
}

TEST_F(BinderTest, bindFromClassMethodToLambdaWithAllArgs) {
  struct ClassWithMethod {
    void operator()(const std::string& str, int nr, bool flag, char c){};
  };

  std::tuple<std::string, int, bool, char> values;
  auto bind = egnite::utils::bindCallback<ClassWithMethod>(
      [&values](const std::string& str, int nr, bool flag, char c) {
        std::get<0>(values) = str;
        std::get<1>(values) = nr;
        std::get<2>(values) = flag;
        std::get<3>(values) = c;
      });
  bind("string", 1, true, 'z');

  ASSERT_EQ(std::get<0>(values), "string");
  ASSERT_EQ(std::get<1>(values), 1);
  ASSERT_EQ(std::get<2>(values), true);
  ASSERT_EQ(std::get<3>(values), 'z');
}

TEST_F(BinderTest, bindFromClassMethodToLambdaWithSomeArgs) {
  struct ClassWithMethod {
    void operator()(const std::string& str, int nr, bool flag, char c){};
  };

  std::tuple<std::string, int, bool, char> values;
  auto bind = egnite::utils::bindCallback<ClassWithMethod>(
      [&values](const std::string& str, int nr) {
        std::get<0>(values) = str;
        std::get<1>(values) = nr;
      });
  bind("string", 1, true, 'z');

  ASSERT_EQ(std::get<0>(values), "string");
  ASSERT_EQ(std::get<1>(values), 1);
}

/* --------------------- From Class Method to Class Method ------------------ */

TEST_F(BinderTest, bindFromClassMethodToClassMethodWithoutArgs) {
  struct SrcClassWithMethod {
    void operator()(const std::string& str, int nr, bool flag, char c){};
  };

  struct DstClassWithMethod {
    void operator()() const {};
  };

  const auto cvm = DstClassWithMethod{};
  auto bind = egnite::utils::bindCallback<SrcClassWithMethod>(cvm);
  bind("string", 1, true, 'z');

  ASSERT_TRUE(true);
}

TEST_F(BinderTest, bindFromClassMethodToClassMethodWithAllArgs) {
  struct SrcClassWithMethod {
    void operator()(const std::string& str, int nr, bool flag, char c){};
  };

  struct DstClassWithMethod {
    void operator()(const std::string& str, int nr, bool flag, char c) {
      std::get<0>(values) = str;
      std::get<1>(values) = nr;
      std::get<2>(values) = flag;
      std::get<3>(values) = c;
    };

    std::tuple<std::string, int, bool, char> values;
  };

  auto cvm = DstClassWithMethod{};
  auto bind = egnite::utils::bindCallback<SrcClassWithMethod>(cvm);
  bind("string", 1, true, 'z');

  ASSERT_EQ(std::get<0>(cvm.values), "string");
  ASSERT_EQ(std::get<1>(cvm.values), 1);
  ASSERT_EQ(std::get<2>(cvm.values), true);
  ASSERT_EQ(std::get<3>(cvm.values), 'z');
}

TEST_F(BinderTest, bindFromClassMethodToClassMethodWithSomeArgs) {
  struct SrcClassWithMethod {
    void operator()(const std::string& str, int nr, bool flag, char c){};
  };

  struct DstClassWithMethod {
    void operator()(const std::string& str, int nr) {
      std::get<0>(values) = str;
      std::get<1>(values) = nr;
    };

    std::tuple<std::string, int> values;
  };

  auto cvm = DstClassWithMethod{};
  auto bind = egnite::utils::bindCallback<SrcClassWithMethod>(cvm);
  bind("string", 1, true, 'z');

  ASSERT_EQ(std::get<0>(cvm.values), "string");
  ASSERT_EQ(std::get<1>(cvm.values), 1);
}
