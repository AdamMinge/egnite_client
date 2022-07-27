/* ----------------------------------- Locals ------------------------------- */
#include "egnite/web/test_serializer.h"
/* -------------------------------------------------------------------------- */

TEST_F(TestJsonSerializer, returnCorrectFormat) {
  ASSERT_EQ(m_serializer->getFormat(), egnite::web::Serializer::Format::Json);
}

TEST_F(TestJsonSerializer, loginRequestIsSerializable) {
  egnite::web::LoginRequest request{.username = "username", .password = "password"};
  auto serialized_data = m_serializer->serialize(request);
  auto deserialized_request = m_serializer->deserialize<egnite::web::LoginRequest>(serialized_data);

  ASSERT_EQ(request, deserialized_request);
}

TEST_F(TestJsonSerializer, loginResponseIsSerializable) {
  egnite::web::LoginResponse response{.access_token = "access_token",
                                      .refresh_token = "refresh_token"};
  auto serialized_data = m_serializer->serialize(response);
  auto deserialized_response
      = m_serializer->deserialize<egnite::web::LoginResponse>(serialized_data);

  ASSERT_EQ(response, deserialized_response);
}

TEST_F(TestJsonSerializer, logoutRequestIsSerializable) {
  egnite::web::LogoutRequest request{.refresh_token = "refresh_token"};
  auto serialized_data = m_serializer->serialize(request);
  auto deserialized_request
      = m_serializer->deserialize<egnite::web::LogoutRequest>(serialized_data);

  ASSERT_EQ(request, deserialized_request);
}

TEST_F(TestJsonSerializer, renewAccessTokenRequestIsSerializable) {
  egnite::web::RenewAccessTokenRequest request{.refresh_token = "refresh_token"};
  auto serialized_data = m_serializer->serialize(request);
  auto deserialized_request
      = m_serializer->deserialize<egnite::web::RenewAccessTokenRequest>(serialized_data);

  ASSERT_EQ(request, deserialized_request);
}

TEST_F(TestJsonSerializer, renewAccessTokenResponseIsSerializable) {
  egnite::web::RenewAccessTokenResponse response{.access_token = "access_token"};
  auto serialized_data = m_serializer->serialize(response);
  auto deserialized_response
      = m_serializer->deserialize<egnite::web::RenewAccessTokenResponse>(serialized_data);

  ASSERT_EQ(response, deserialized_response);
}
