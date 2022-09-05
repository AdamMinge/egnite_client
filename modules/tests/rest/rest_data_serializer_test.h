#ifndef EGNITE_REST_TEST_REST_DATA_SERIALIZER_H
#define EGNITE_REST_TEST_REST_DATA_SERIALIZER_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QObject>
#include <QScopedPointer>
/* -------------------------------- GoogleTest ------------------------------ */
#include <gtest/gtest.h>
/* ----------------------------------- Egnite ------------------------------- */
#include <egnite/rest/data_serializer.h>
/* -------------------------------------------------------------------------- */

namespace mocks {

#define DEFINE_MOCK(name, type)                              \
  struct MockWith##name {                                    \
    Q_GADGET                                                 \
    Q_PROPERTY(type _##type MEMBER _##type);                 \
                                                             \
   public:                                                   \
    MockWith##name(type _##type = {}) : _##type(_##type) {}  \
    bool operator<=>(const MockWith##name&) const = default; \
    type _##type;                                            \
  };

DEFINE_MOCK(Url, QUrl);
DEFINE_MOCK(Uuid, QUuid);
DEFINE_MOCK(RectF, QRectF);
DEFINE_MOCK(Rect, QRect);
DEFINE_MOCK(LineF, QLineF);
DEFINE_MOCK(Line, QLine);
DEFINE_MOCK(PointF, QPointF);
DEFINE_MOCK(Point, QPoint);
DEFINE_MOCK(SizeF, QSizeF);
DEFINE_MOCK(Size, QSize);
DEFINE_MOCK(Time, QTime);
DEFINE_MOCK(Date, QDate);
DEFINE_MOCK(DateTime, QDateTime);
DEFINE_MOCK(StringList, QStringList);
DEFINE_MOCK(String, QString);
DEFINE_MOCK(Float, float);
DEFINE_MOCK(Double, double);
DEFINE_MOCK(Bool, bool);
DEFINE_MOCK(ULongLong, qulonglong);
DEFINE_MOCK(LongLong, qlonglong);
DEFINE_MOCK(UInt, uint);
DEFINE_MOCK(Int, int);

struct MockFriend {
  Q_GADGET

  Q_PROPERTY(uint id MEMBER id)
  Q_PROPERTY(QString username MEMBER username)

 public:
  MockFriend(uint id = 0, QString username = {})
      : id(id), username(std::move(username)) {}
  bool operator<=>(const MockFriend&) const = default;

  uint id;
  QString username;
};

struct MockUserDetail {
  Q_GADGET

  Q_PROPERTY(QString email MEMBER email)
  Q_PROPERTY(uint age MEMBER age)

 public:
  MockUserDetail(QString email = {}, uint age = 0) : email(email), age(age) {}
  bool operator<=>(const MockUserDetail&) const = default;

  QString email;
  uint age;
};

struct MockUser {
  Q_GADGET

  Q_PROPERTY(uint id MEMBER id)
  Q_PROPERTY(QString username MEMBER username)
  Q_PROPERTY(QList<MockFriend> friends MEMBER friends)
  Q_PROPERTY(QList<QDateTime> login_history MEMBER login_history)
  Q_PROPERTY(MockUserDetail detail MEMBER detail)

 public:
  MockUser(uint id = 0, QString username = {})
      : id(id), username(std::move(username)) {}
  bool operator<=>(const MockUser&) const = default;

  uint id;
  QString username;
  QList<MockFriend> friends;
  QList<QDateTime> login_history;
  MockUserDetail detail;
};

}  // namespace mocks

class DataSerializerTest : public testing::Test {
 public:
  DataSerializerTest() = default;
  ~DataSerializerTest() override = default;

  void SetUp() override {
    m_serializer.reset(new egnite::rest::DataSerializer);
  }
  void TearDown() override { m_serializer.reset(); }

 protected:
  QScopedPointer<egnite::rest::DataSerializer> m_serializer;
};

class DataJsonSerializerTest : public DataSerializerTest {
 public:
  DataJsonSerializerTest() = default;
  ~DataJsonSerializerTest() override = default;
};

class DataCborSerializerTest : public DataSerializerTest {
 public:
  DataCborSerializerTest() = default;
  ~DataCborSerializerTest() override = default;
};

#endif  // EGNITE_REST_TEST_REST_DATA_SERIALIZER_H