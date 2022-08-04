/* ----------------------------------- Locals ------------------------------- */
#include "rest/test_serializer.h"
/* -------------------------------------------------------------------------- */

TEST_F(TestJsonSerializer, returnCorrectFormat) {
  ASSERT_EQ(m_serializer->getFormat(), egnite::rest::Serializer::Format::Json);
}

TEST_F(TestJsonSerializer, loginRequestIsSerializable) {
  egnite::rest::LoginRequest request{.username = "username",
                                     .password = "password"};
  auto serialized_data = m_serializer->serialize(request);
  auto deserialized_request =
      m_serializer->deserialize<egnite::rest::LoginRequest>(serialized_data);

  ASSERT_EQ(request, deserialized_request);
}

TEST_F(TestJsonSerializer, loginResponseIsSerializable) {
  egnite::rest::LoginResponse response{.access_token = "access_token",
                                       .refresh_token = "refresh_token"};
  auto serialized_data = m_serializer->serialize(response);
  auto deserialized_response =
      m_serializer->deserialize<egnite::rest::LoginResponse>(serialized_data);

  ASSERT_EQ(response, deserialized_response);
}

TEST_F(TestJsonSerializer, logoutRequestIsSerializable) {
  egnite::rest::LogoutRequest request{.refresh_token = "refresh_token"};
  auto serialized_data = m_serializer->serialize(request);
  auto deserialized_request =
      m_serializer->deserialize<egnite::rest::LogoutRequest>(serialized_data);

  ASSERT_EQ(request, deserialized_request);
}

TEST_F(TestJsonSerializer, renewAccessTokenRequestIsSerializable) {
  egnite::rest::RenewAccessTokenRequest request{.refresh_token =
                                                    "refresh_token"};
  auto serialized_data = m_serializer->serialize(request);
  auto deserialized_request =
      m_serializer->deserialize<egnite::rest::RenewAccessTokenRequest>(
          serialized_data);

  ASSERT_EQ(request, deserialized_request);
}

TEST_F(TestJsonSerializer, renewAccessTokenResponseIsSerializable) {
  egnite::rest::RenewAccessTokenResponse response{.access_token =
                                                      "access_token"};
  auto serialized_data = m_serializer->serialize(response);
  auto deserialized_response =
      m_serializer->deserialize<egnite::rest::RenewAccessTokenResponse>(
          serialized_data);

  ASSERT_EQ(response, deserialized_response);
}
