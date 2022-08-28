/* ----------------------------------- Local -------------------------------- */
#include "rest/rest_data_serializer_test.h"
/* -------------------------------------------------------------------------- */

/* ------------------------- RestDataJsonSerializerTest --------------------- */

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
  auto mock = mocks::MockWithTime{QTime(12, 20, 30)};

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
      QDateTime(QDate(1996, 10, 21), QTime(12, 20, 30))};

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

TEST_F(RestDataJsonSerializerTest, MockUserIsSerializable) {
  auto mock = mocks::MockUser{1, "user"};
  mock.friends = QList<mocks::MockFriend>{
      mocks::MockFriend{2, "friend2"}, mocks::MockFriend{3, "friend3"},
      mocks::MockFriend{4, "friend4"}, mocks::MockFriend{5, "friend5"}};
  mock.login_history =
      QList<QDateTime>{QDateTime(QDate(2020, 10, 20), QTime(12, 30, 0)),
                       QDateTime(QDate(2020, 10, 20), QTime(12, 30, 0)),
                       QDateTime(QDate(2020, 10, 20), QTime(12, 30, 0)),
                       QDateTime(QDate(2020, 10, 20), QTime(12, 30, 0)),
                       QDateTime(QDate(2020, 10, 20), QTime(12, 30, 0))};
  mock.detail = mocks::MockUserDetail("user@gmail.com", 18);

  auto rest_data = m_serializer->serialize(
      mock, egnite::rest::RestDataSerializer::Format::Json);
  auto mock_deserialized =
      m_serializer->deserialize<mocks::MockUser>(rest_data);

  ASSERT_EQ(mock, mock_deserialized);
}

/* ------------------------- RestDataCborSerializerTest --------------------- */

TEST_F(RestDataCborSerializerTest, urlIsSerializable) {
  auto mock = mocks::MockWithUrl{QUrl("http://localhost:8000/api/v1/swagger")};

  auto rest_data = m_serializer->serialize(
      mock, egnite::rest::RestDataSerializer::Format::Cbor);
  auto mock_deserialized =
      m_serializer->deserialize<mocks::MockWithUrl>(rest_data);

  ASSERT_EQ(mock, mock_deserialized);
}

TEST_F(RestDataCborSerializerTest, uuidIsSerializable) {
  auto mock = mocks::MockWithUuid{QUuid(0x67c8770b, 0x44f1, 0x410a, 0xab, 0x9a,
                                        0xf9, 0xb5, 0x44, 0x6f, 0x13, 0xee)};

  auto rest_data = m_serializer->serialize(
      mock, egnite::rest::RestDataSerializer::Format::Cbor);
  auto mock_deserialized =
      m_serializer->deserialize<mocks::MockWithUuid>(rest_data);

  ASSERT_EQ(mock, mock_deserialized);
}

TEST_F(RestDataCborSerializerTest, RectFIsSerializable) {
  auto mock = mocks::MockWithRectF{QRectF(2.5, 2.5, 10.5, 10.5)};

  auto rest_data = m_serializer->serialize(
      mock, egnite::rest::RestDataSerializer::Format::Cbor);
  auto mock_deserialized =
      m_serializer->deserialize<mocks::MockWithRectF>(rest_data);

  ASSERT_EQ(mock, mock_deserialized);
}

TEST_F(RestDataCborSerializerTest, RectIsSerializable) {
  auto mock = mocks::MockWithRect{QRect(2, 2, 10, 10)};

  auto rest_data = m_serializer->serialize(
      mock, egnite::rest::RestDataSerializer::Format::Cbor);
  auto mock_deserialized =
      m_serializer->deserialize<mocks::MockWithRect>(rest_data);

  ASSERT_EQ(mock, mock_deserialized);
}

TEST_F(RestDataCborSerializerTest, LineFIsSerializable) {
  auto mock = mocks::MockWithLineF{QLineF{{2.5, 2.5}, {10.5, 10.5}}};

  auto rest_data = m_serializer->serialize(
      mock, egnite::rest::RestDataSerializer::Format::Cbor);
  auto mock_deserialized =
      m_serializer->deserialize<mocks::MockWithLineF>(rest_data);

  ASSERT_EQ(mock, mock_deserialized);
}

TEST_F(RestDataCborSerializerTest, LineIsSerializable) {
  auto mock = mocks::MockWithLine{QLine({{2, 2}, {10, 10}})};

  auto rest_data = m_serializer->serialize(
      mock, egnite::rest::RestDataSerializer::Format::Cbor);
  auto mock_deserialized =
      m_serializer->deserialize<mocks::MockWithLine>(rest_data);

  ASSERT_EQ(mock, mock_deserialized);
}

TEST_F(RestDataCborSerializerTest, PointFIsSerializable) {
  auto mock = mocks::MockWithPointF{QPointF(2.5, 10.5)};

  auto rest_data = m_serializer->serialize(
      mock, egnite::rest::RestDataSerializer::Format::Cbor);
  auto mock_deserialized =
      m_serializer->deserialize<mocks::MockWithPointF>(rest_data);

  ASSERT_EQ(mock, mock_deserialized);
}

TEST_F(RestDataCborSerializerTest, PointIsSerializable) {
  auto mock = mocks::MockWithPoint{QPoint(2, 10)};

  auto rest_data = m_serializer->serialize(
      mock, egnite::rest::RestDataSerializer::Format::Cbor);
  auto mock_deserialized =
      m_serializer->deserialize<mocks::MockWithPoint>(rest_data);

  ASSERT_EQ(mock, mock_deserialized);
}

TEST_F(RestDataCborSerializerTest, SizeFIsSerializable) {
  auto mock = mocks::MockWithSizeF{QSizeF(2.5, 10.5)};

  auto rest_data = m_serializer->serialize(
      mock, egnite::rest::RestDataSerializer::Format::Cbor);
  auto mock_deserialized =
      m_serializer->deserialize<mocks::MockWithSizeF>(rest_data);

  ASSERT_EQ(mock, mock_deserialized);
}

TEST_F(RestDataCborSerializerTest, SizeIsSerializable) {
  auto mock = mocks::MockWithSize{QSize(2, 10)};

  auto rest_data = m_serializer->serialize(
      mock, egnite::rest::RestDataSerializer::Format::Cbor);
  auto mock_deserialized =
      m_serializer->deserialize<mocks::MockWithSize>(rest_data);

  ASSERT_EQ(mock, mock_deserialized);
}

TEST_F(RestDataCborSerializerTest, TimeIsSerializable) {
  auto mock = mocks::MockWithTime{QTime(12, 20, 30)};

  auto rest_data = m_serializer->serialize(
      mock, egnite::rest::RestDataSerializer::Format::Cbor);
  auto mock_deserialized =
      m_serializer->deserialize<mocks::MockWithTime>(rest_data);

  ASSERT_EQ(mock, mock_deserialized);
}

TEST_F(RestDataCborSerializerTest, DateIsSerializable) {
  auto mock = mocks::MockWithDate{QDate(1996, 10, 21)};

  auto rest_data = m_serializer->serialize(
      mock, egnite::rest::RestDataSerializer::Format::Cbor);
  auto mock_deserialized =
      m_serializer->deserialize<mocks::MockWithDate>(rest_data);

  ASSERT_EQ(mock, mock_deserialized);
}

TEST_F(RestDataCborSerializerTest, DateTimeIsSerializable) {
  auto mock = mocks::MockWithDateTime{
      QDateTime(QDate(1996, 10, 21), QTime(12, 20, 30))};

  auto rest_data = m_serializer->serialize(
      mock, egnite::rest::RestDataSerializer::Format::Cbor);
  auto mock_deserialized =
      m_serializer->deserialize<mocks::MockWithDateTime>(rest_data);

  ASSERT_EQ(mock, mock_deserialized);
}

TEST_F(RestDataCborSerializerTest, StringListIsSerializable) {
  auto mock = mocks::MockWithStringList{QStringList{} << "str1"
                                                      << "str2"
                                                      << "str3"};
  auto rest_data = m_serializer->serialize(
      mock, egnite::rest::RestDataSerializer::Format::Cbor);
  auto mock_deserialized =
      m_serializer->deserialize<mocks::MockWithStringList>(rest_data);

  ASSERT_EQ(mock, mock_deserialized);
}

TEST_F(RestDataCborSerializerTest, StringIsSerializable) {
  auto mock = mocks::MockWithString{QString("str")};

  auto rest_data = m_serializer->serialize(
      mock, egnite::rest::RestDataSerializer::Format::Cbor);
  auto mock_deserialized =
      m_serializer->deserialize<mocks::MockWithString>(rest_data);

  ASSERT_EQ(mock, mock_deserialized);
}

TEST_F(RestDataCborSerializerTest, FloatIsSerializable) {
  auto mock = mocks::MockWithFloat{10.5f};

  auto rest_data = m_serializer->serialize(
      mock, egnite::rest::RestDataSerializer::Format::Cbor);
  auto mock_deserialized =
      m_serializer->deserialize<mocks::MockWithFloat>(rest_data);

  ASSERT_EQ(mock, mock_deserialized);
}

TEST_F(RestDataCborSerializerTest, DoubleIsSerializable) {
  auto mock = mocks::MockWithDouble{10.5};

  auto rest_data = m_serializer->serialize(
      mock, egnite::rest::RestDataSerializer::Format::Cbor);
  auto mock_deserialized =
      m_serializer->deserialize<mocks::MockWithDouble>(rest_data);

  ASSERT_EQ(mock, mock_deserialized);
}

TEST_F(RestDataCborSerializerTest, BoolIsSerializable) {
  auto mock = mocks::MockWithBool{true};

  auto rest_data = m_serializer->serialize(
      mock, egnite::rest::RestDataSerializer::Format::Cbor);
  auto mock_deserialized =
      m_serializer->deserialize<mocks::MockWithBool>(rest_data);

  ASSERT_EQ(mock, mock_deserialized);
}

TEST_F(RestDataCborSerializerTest, ULongLongIsSerializable) {
  auto mock = mocks::MockWithULongLong{qulonglong(1024)};

  auto rest_data = m_serializer->serialize(
      mock, egnite::rest::RestDataSerializer::Format::Cbor);
  auto mock_deserialized =
      m_serializer->deserialize<mocks::MockWithULongLong>(rest_data);

  ASSERT_EQ(mock, mock_deserialized);
}

TEST_F(RestDataCborSerializerTest, LongLongIsSerializable) {
  auto mock = mocks::MockWithLongLong{qlonglong(-1024)};

  auto rest_data = m_serializer->serialize(
      mock, egnite::rest::RestDataSerializer::Format::Cbor);
  auto mock_deserialized =
      m_serializer->deserialize<mocks::MockWithLongLong>(rest_data);

  ASSERT_EQ(mock, mock_deserialized);
}

TEST_F(RestDataCborSerializerTest, UIntIsSerializable) {
  auto mock = mocks::MockWithUInt{1024u};

  auto rest_data = m_serializer->serialize(
      mock, egnite::rest::RestDataSerializer::Format::Cbor);
  auto mock_deserialized =
      m_serializer->deserialize<mocks::MockWithUInt>(rest_data);

  ASSERT_EQ(mock, mock_deserialized);
}

TEST_F(RestDataCborSerializerTest, IntIsSerializable) {
  auto mock = mocks::MockWithInt{-1024};

  auto rest_data = m_serializer->serialize(
      mock, egnite::rest::RestDataSerializer::Format::Cbor);
  auto mock_deserialized =
      m_serializer->deserialize<mocks::MockWithInt>(rest_data);

  ASSERT_EQ(mock, mock_deserialized);
}

TEST_F(RestDataCborSerializerTest, MockUserIsSerializable) {
  auto mock = mocks::MockUser{1, "user"};
  mock.friends = QList<mocks::MockFriend>{
      mocks::MockFriend{2, "friend2"}, mocks::MockFriend{3, "friend3"},
      mocks::MockFriend{4, "friend4"}, mocks::MockFriend{5, "friend5"}};
  mock.login_history =
      QList<QDateTime>{QDateTime(QDate(2020, 10, 20), QTime(12, 30, 0)),
                       QDateTime(QDate(2020, 10, 20), QTime(12, 30, 0)),
                       QDateTime(QDate(2020, 10, 20), QTime(12, 30, 0)),
                       QDateTime(QDate(2020, 10, 20), QTime(12, 30, 0)),
                       QDateTime(QDate(2020, 10, 20), QTime(12, 30, 0))};
  mock.detail = mocks::MockUserDetail("user@gmail.com", 18);

  auto rest_data = m_serializer->serialize(
      mock, egnite::rest::RestDataSerializer::Format::Cbor);
  auto mock_deserialized =
      m_serializer->deserialize<mocks::MockUser>(rest_data);

  ASSERT_EQ(mock, mock_deserialized);
}
