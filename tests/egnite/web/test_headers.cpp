/* ----------------------------------- Locals ------------------------------- */
#include "egnite/web/test_headers.h"
/* -------------------------------------------------------------------------- */

TEST_F(TestHeaders, getHeadersEmptyWhenNoSetAnyHeader) {
  egnite::web::Headers headers{};
  ASSERT_TRUE(headers.getHeaderSet().empty());
}

TEST_F(TestHeaders, getHeadersReturnOnlySetHeaders) {
  egnite::web::Headers headers{};

  headers.setHeader(KnownHeaders::Accept, "application/json");
  headers.setHeader(KnownHeaders::ApiKey, "yeAuAi6RRypRGLtWKnUJHAB0qeHkQ3z2");

  auto header_set = headers.getHeaderSet();

  ASSERT_EQ(header_set.count(), 2);
  ASSERT_TRUE(header_set.contains(KnownHeaders::Accept));
  ASSERT_TRUE(header_set.contains(KnownHeaders::ApiKey));
}

TEST_F(TestHeaders, getHeaderReturnEmptyVariantForNoSetHeader) {
  egnite::web::Headers headers{};
  ASSERT_EQ(headers.getHeader(KnownHeaders::Accept), QVariant{});
}

TEST_F(TestHeaders, getHeaderReturnVariantForHeader) {
  egnite::web::Headers headers{};
  headers.setHeader(KnownHeaders::Accept, "application/json");
  ASSERT_EQ(headers.getHeader(KnownHeaders::Accept), "application/json");
}

TEST_F(TestHeaders, headersAreMultipleTimeChangeable) {
  egnite::web::Headers headers{};

  headers.setHeader(KnownHeaders::Accept, "application/json");
  ASSERT_EQ(headers.getHeader(KnownHeaders::Accept), "application/json");

  headers.setHeader(KnownHeaders::Accept, "application/txt");
  ASSERT_EQ(headers.getHeader(KnownHeaders::Accept), "application/txt");
}

TEST_F(TestHeaders, hasHeaderCheckIfHeaderWasSet) {
  egnite::web::Headers headers{};

  ASSERT_FALSE(headers.hasHeader(KnownHeaders::Accept));
  ASSERT_FALSE(headers.hasHeader(KnownHeaders::ApiKey));

  headers.setHeader(KnownHeaders::ApiKey, "yeAuAi6RRypRGLtWKnUJHAB0qeHkQ3z2");

  ASSERT_FALSE(headers.hasHeader(KnownHeaders::Accept));
  ASSERT_TRUE(headers.hasHeader(KnownHeaders::ApiKey));
}
