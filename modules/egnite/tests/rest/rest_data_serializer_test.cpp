/* ----------------------------------- Local -------------------------------- */
#include "rest/rest_data_serializer_test.h"
/* -------------------------------------------------------------------------- */

TEST_F(RestDataJsonSerializerTest, urlIsSerializable) {
  auto mock = mocks::MockWithUrl{QUrl("http://localhost:8000/api/v1/swagger")};

  auto rest_data = m_serializer->serialize(
      mock, egnite::rest::RestDataSerializer::Format::Json);
  auto mock_deserialized =
      m_serializer->deserialize<mocks::MockWithUrl>(rest_data);

  ASSERT_EQ(mock, mock_deserialized);
}

TEST_F(RestDataJsonSerializerTest, uuidIsSerializable) {
  auto mock = mocks::MockWithUuid{QUuid(0x67c8770b, 0x44f1, 0x410a, 0xab, 0x9a,
                                        0xf9, 0xb5, 0x44, 0x6f, 0x13, 0xee)};

  auto rest_data = m_serializer->serialize(
      mock, egnite::rest::RestDataSerializer::Format::Json);
  auto mock_deserialized =
      m_serializer->deserialize<mocks::MockWithUuid>(rest_data);

  ASSERT_EQ(mock, mock_deserialized);
}

TEST_F(RestDataJsonSerializerTest, RectFIsSerializable) {
  auto mock = mocks::MockWithRectF{QRectF(2.5, 2.5, 10.5, 10.5)};

  auto rest_data = m_serializer->serialize(
      mock, egnite::rest::RestDataSerializer::Format::Json);
  auto mock_deserialized =
      m_serializer->deserialize<mocks::MockWithRectF>(rest_data);

  ASSERT_EQ(mock, mock_deserialized);
}

TEST_F(RestDataJsonSerializerTest, RectIsSerializable) {
  auto mock = mocks::MockWithRect{QRect(2, 2, 10, 10)};

  auto rest_data = m_serializer->serialize(
      mock, egnite::rest::RestDataSerializer::Format::Json);
  auto mock_deserialized =
      m_serializer->deserialize<mocks::MockWithRect>(rest_data);

  ASSERT_EQ(mock, mock_deserialized);
}

TEST_F(RestDataJsonSerializerTest, LineFIsSerializable) {
  auto mock = mocks::MockWithLineF{QLineF{{2.5, 2.5}, {10.5, 10.5}}};

  auto rest_data = m_serializer->serialize(
      mock, egnite::rest::RestDataSerializer::Format::Json);
  auto mock_deserialized =
      m_serializer->deserialize<mocks::MockWithLineF>(rest_data);

  ASSERT_EQ(mock, mock_deserialized);
}

TEST_F(RestDataJsonSerializerTest, LineIsSerializable) {
  auto mock = mocks::MockWithLine{QLine({{2, 2}, {10, 10}})};

  auto rest_data = m_serializer->serialize(
      mock, egnite::rest::RestDataSerializer::Format::Json);
  auto mock_deserialized =
      m_serializer->deserialize<mocks::MockWithLine>(rest_data);

  ASSERT_EQ(mock, mock_deserialized);
}

TEST_F(RestDataJsonSerializerTest, PointFIsSerializable) {
  auto mock = mocks::MockWithPointF{QPointF(2.5, 10.5)};

  auto rest_data = m_serializer->serialize(
      mock, egnite::rest::RestDataSerializer::Format::Json);
  auto mock_deserialized =
      m_serializer->deserialize<mocks::MockWithPointF>(rest_data);

  ASSERT_EQ(mock, mock_deserialized);
}

TEST_F(RestDataJsonSerializerTest, PointIsSerializable) {
  auto mock = mocks::MockWithPoint{QPoint(2, 10)};

  auto rest_data = m_serializer->serialize(
      mock, egnite::rest::RestDataSerializer::Format::Json);
  auto mock_deserialized =
      m_serializer->deserialize<mocks::MockWithPoint>(rest_data);

  ASSERT_EQ(mock, mock_deserialized);
}

TEST_F(RestDataJsonSerializerTest, SizeFIsSerializable) {
  auto mock = mocks::MockWithSizeF{QSizeF(2.5, 10.5)};

  auto rest_data = m_serializer->serialize(
      mock, egnite::rest::RestDataSerializer::Format::Json);
  auto mock_deserialized =
      m_serializer->deserialize<mocks::MockWithSizeF>(rest_data);

  ASSERT_EQ(mock, mock_deserialized);
}

TEST_F(RestDataJsonSerializerTest, SizeIsSerializable) {
  auto mock = mocks::MockWithSize{QSize(2, 10)};

  auto rest_data = m_serializer->serialize(
      mock, egnite::rest::RestDataSerializer::Format::Json);
  auto mock_deserialized =
      m_serializer->deserialize<mocks::MockWithSize>(rest_data);

  ASSERT_EQ(mock, mock_deserialized);
}

TEST_F(RestDataJsonSerializerTest, TimeIsSerializable) {
  auto mock = mocks::MockWithTime{QTime(12, 20, 30, 40)};

  auto rest_data = m_serializer->serialize(
      mock, egnite::rest::RestDataSerializer::Format::Json);
  auto mock_deserialized =
      m_serializer->deserialize<mocks::MockWithTime>(rest_data);

  ASSERT_EQ(mock, mock_deserialized);
}

TEST_F(RestDataJsonSerializerTest, DateIsSerializable) {
  auto mock = mocks::MockWithDate{QDate(1996, 10, 21)};

  auto rest_data = m_serializer->serialize(
      mock, egnite::rest::RestDataSerializer::Format::Json);
  auto mock_deserialized =
      m_serializer->deserialize<mocks::MockWithDate>(rest_data);

  ASSERT_EQ(mock, mock_deserialized);
}

TEST_F(RestDataJsonSerializerTest, DateTimeIsSerializable) {
  auto mock = mocks::MockWithDateTime{
      QDateTime(QDate(1996, 10, 21), QTime(12, 20, 30, 40))};

  auto rest_data = m_serializer->serialize(
      mock, egnite::rest::RestDataSerializer::Format::Json);
  auto mock_deserialized =
      m_serializer->deserialize<mocks::MockWithDateTime>(rest_data);

  ASSERT_EQ(mock, mock_deserialized);
}

TEST_F(RestDataJsonSerializerTest, StringListIsSerializable) {
  auto mock = mocks::MockWithStringList{QStringList{} << "str1"
                                                      << "str2"
                                                      << "str3"};
  auto rest_data = m_serializer->serialize(
      mock, egnite::rest::RestDataSerializer::Format::Json);
  auto mock_deserialized =
      m_serializer->deserialize<mocks::MockWithStringList>(rest_data);

  ASSERT_EQ(mock, mock_deserialized);
}

TEST_F(RestDataJsonSerializerTest, StringIsSerializable) {
  auto mock = mocks::MockWithString{QString("str")};

  auto rest_data = m_serializer->serialize(
      mock, egnite::rest::RestDataSerializer::Format::Json);
  auto mock_deserialized =
      m_serializer->deserialize<mocks::MockWithString>(rest_data);

  ASSERT_EQ(mock, mock_deserialized);
}

TEST_F(RestDataJsonSerializerTest, FloatIsSerializable) {
  auto mock = mocks::MockWithFloat{10.5f};

  auto rest_data = m_serializer->serialize(
      mock, egnite::rest::RestDataSerializer::Format::Json);
  auto mock_deserialized =
      m_serializer->deserialize<mocks::MockWithFloat>(rest_data);

  ASSERT_EQ(mock, mock_deserialized);
}

TEST_F(RestDataJsonSerializerTest, DoubleIsSerializable) {
  auto mock = mocks::MockWithDouble{10.5};

  auto rest_data = m_serializer->serialize(
      mock, egnite::rest::RestDataSerializer::Format::Json);
  auto mock_deserialized =
      m_serializer->deserialize<mocks::MockWithDouble>(rest_data);

  ASSERT_EQ(mock, mock_deserialized);
}

TEST_F(RestDataJsonSerializerTest, BoolIsSerializable) {
  auto mock = mocks::MockWithBool{true};

  auto rest_data = m_serializer->serialize(
      mock, egnite::rest::RestDataSerializer::Format::Json);
  auto mock_deserialized =
      m_serializer->deserialize<mocks::MockWithBool>(rest_data);

  ASSERT_EQ(mock, mock_deserialized);
}

TEST_F(RestDataJsonSerializerTest, ULongLongIsSerializable) {
  auto mock = mocks::MockWithULongLong{qulonglong(1024)};

  auto rest_data = m_serializer->serialize(
      mock, egnite::rest::RestDataSerializer::Format::Json);
  auto mock_deserialized =
      m_serializer->deserialize<mocks::MockWithULongLong>(rest_data);

  ASSERT_EQ(mock, mock_deserialized);
}

TEST_F(RestDataJsonSerializerTest, LongLongIsSerializable) {
  auto mock = mocks::MockWithLongLong{qlonglong(-1024)};

  auto rest_data = m_serializer->serialize(
      mock, egnite::rest::RestDataSerializer::Format::Json);
  auto mock_deserialized =
      m_serializer->deserialize<mocks::MockWithLongLong>(rest_data);

  ASSERT_EQ(mock, mock_deserialized);
}

TEST_F(RestDataJsonSerializerTest, UIntIsSerializable) {
  auto mock = mocks::MockWithUInt{1024u};

  auto rest_data = m_serializer->serialize(
      mock, egnite::rest::RestDataSerializer::Format::Json);
  auto mock_deserialized =
      m_serializer->deserialize<mocks::MockWithUInt>(rest_data);

  ASSERT_EQ(mock, mock_deserialized);
}

TEST_F(RestDataJsonSerializerTest, IntIsSerializable) {
  auto mock = mocks::MockWithInt{-1024};

  auto rest_data = m_serializer->serialize(
      mock, egnite::rest::RestDataSerializer::Format::Json);
  auto mock_deserialized =
      m_serializer->deserialize<mocks::MockWithInt>(rest_data);

  ASSERT_EQ(mock, mock_deserialized);
}
