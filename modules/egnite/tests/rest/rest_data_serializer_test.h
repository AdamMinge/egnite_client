#ifndef EGNITE_REST_TEST_REST_DATA_SERIALIZER_H
#define EGNITE_REST_TEST_REST_DATA_SERIALIZER_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QObject>
#include <QScopedPointer>
/* -------------------------------- GoogleTest ------------------------------ */
#include <gtest/gtest.h>
/* ----------------------------------- Egnite ------------------------------- */
#include <egnite/rest/rest_data_serializer.h>
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
DEFINE_MOCK(BitArray, QBitArray);
DEFINE_MOCK(Float, float);
DEFINE_MOCK(Double, double);
DEFINE_MOCK(Bool, bool);
DEFINE_MOCK(ULongLong, qulonglong);
DEFINE_MOCK(LongLong, qlonglong);
DEFINE_MOCK(UInt, uint);
DEFINE_MOCK(Int, int);

}  // namespace mocks

class RestDataSerializerTest : public testing::Test {
 public:
  RestDataSerializerTest() = default;
  ~RestDataSerializerTest() override = default;

  void SetUp() override {
    m_serializer.reset(new egnite::rest::RestDataSerializer);
  }
  void TearDown() override { m_serializer.reset(); }

 protected:
  QScopedPointer<egnite::rest::RestDataSerializer> m_serializer;
};

class RestDataJsonSerializerTest : public RestDataSerializerTest {
 public:
  RestDataJsonSerializerTest() = default;
  ~RestDataJsonSerializerTest() override = default;
};

class RestDataCborSerializerTest : RestDataSerializerTest {
 public:
  RestDataCborSerializerTest() = default;
  ~RestDataCborSerializerTest() override = default;
};

#endif  // EGNITE_REST_TEST_REST_DATA_SERIALIZER_H