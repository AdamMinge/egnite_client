/* ----------------------------------- Local -------------------------------- */
#include "egnite/rest/rest_reply.h"

#include "egnite/rest/private/rest_reply_p.h"
/* ------------------------------------ Qt ---------------------------------- */
#include <QCborStreamReader>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTimer>
/* -------------------------------------------------------------------------- */

namespace egnite::rest {

/* -------------------------------- RestReply ------------------------------- */

RestReply::RestReply(QNetworkReply* network_reply, QObject* parent)
    : RestReply(
          *new RestReplyPrivate(network_reply, std::chrono::milliseconds(0)),
          parent) {}

RestReply::RestReply(RestReplyPrivate& impl, QObject* parent)
    : QObject(impl, parent) {}

RestReply::~RestReply() = default;

void RestReply::abort() {
  Q_D(RestReply);
  d->abort();
}

void RestReply::retry() {
  Q_D(RestReply);
  d->retry(std::chrono::milliseconds(0));
}

void RestReply::retry(std::chrono::milliseconds msecs) {
  Q_D(RestReply);
  d->retry(msecs);
}

/* ---------------------------- RestReplyPrivate ---------------------------- */

const QByteArray RestReplyPrivate::PropertyBody =
    QByteArray{"__Egnite_Rest_RestReplyPrivate_PropertyBody"};
const QByteArray RestReplyPrivate::ContentTypeJson =
    QByteArray{"application/json"};
const QByteArray RestReplyPrivate::ContentTypeCbor =
    QByteArray{"application/cbor"};

RestReplyPrivate::RestReplyPrivate(QNetworkReply* network_reply,
                                   const std::chrono::milliseconds& retry_delay)
    : m_network_reply(network_reply), m_retry_delay(retry_delay) {
  connectReply();
}

RestReplyPrivate::~RestReplyPrivate() {
  if (m_network_reply) m_network_reply->deleteLater();
}

void RestReplyPrivate::abort() { m_network_reply->abort(); }

void RestReplyPrivate::retry(std::chrono::milliseconds msecs) {
  m_retry_delay = msecs;
}

void RestReplyPrivate::connectReply() {
  connect(m_network_reply, &QNetworkReply::finished, this,
          &RestReplyPrivate::replyFinished);

  Q_Q(RestReply);
  QObject::connect(m_network_reply, &QNetworkReply::downloadProgress, q,
                   &RestReply::downloadProgress);
  QObject::connect(m_network_reply, &QNetworkReply::uploadProgress, q,
                   &RestReply::uploadProgress);
}

void RestReplyPrivate::retryReply() {
  auto manager = m_network_reply->manager();
  auto request = m_network_reply->request();
  auto verb = m_network_reply->attribute(QNetworkRequest::CustomVerbAttribute)
                  .toByteArray();
  auto body = m_network_reply->property(PropertyBody).toByteArray();

  m_network_reply->deleteLater();
  send(manager, request, verb, body);
}

void RestReplyPrivate::replyFinished() {
  Q_Q(RestReply);

  if (!m_network_reply) return;

  const auto status =
      m_network_reply->attribute(QNetworkRequest::HttpStatusCodeAttribute)
          .toInt();
  auto content_type =
      m_network_reply->header(QNetworkRequest::ContentTypeHeader)
          .toByteArray()
          .trimmed();

  auto data = RestReply::DataType{std::nullopt};
  auto parse_error = std::optional<std::pair<RestReply::Error, QString>>{};

  if (const auto content_types = content_type.split(';');
      content_types.size() > 1) {
    content_type = content_types.first().trimmed();
    for (auto i = 1; i < content_types.size(); ++i) {
      const auto args = content_types[i].trimmed().split('=');
      if (args.size() == 2 && args[0] == "charset") {
        if (args[1].toLower() != "utf-8")
          parse_error = std::make_pair(RestReply::Error::Parser,
                                       QStringLiteral("Unsupported charset: %1")
                                           .arg(QString::fromUtf8(args[1])));

      } else
        parse_error = std::make_pair(
            RestReply::Error::Parser,
            QStringLiteral("Unknown content type directive: %1").arg(args[0]));
    }
  }

  if (!parse_error && content_type == ContentTypeJson) {
    const auto read_data = m_network_reply->readAll();
    QJsonParseError error;
    auto json_doc = QJsonDocument::fromJson(read_data, &error);
    if (error.error != QJsonParseError::NoError) {
      parse_error =
          std::make_pair(RestReply::Error::Parser, error.errorString());

      if (error.error == QJsonParseError::IllegalValue) {
        error = QJsonParseError{};
        json_doc = QJsonDocument::fromJson("[" + read_data + "]", &error);
        if (error.error == QJsonParseError::NoError) {
          parse_error.reset();
          data = json_doc.array().first();
        }
      }
    } else if (json_doc.isObject())
      data = QJsonValue(json_doc.object());
    else if (json_doc.isArray())
      data = QJsonValue(json_doc.array());
    else
      data = QJsonValue(QJsonValue::Null);

  } else if (!parse_error && content_type == ContentTypeCbor) {
    QCborStreamReader reader{m_network_reply};
    data = QCborValue::fromCbor(reader);
    if (const auto error = reader.lastError(); error.c != QCborError::NoError)
      parse_error = std::make_pair(RestReply::Error::Parser, error.toString());
  } else if (!parse_error) {
    parse_error = std::make_pair(RestReply::Error::Parser,
                                 QStringLiteral("Unsupported content type: %1")
                                     .arg(QString::fromUtf8(content_type)));
  }

  if (!parse_error && status >= 300 &&
      !std::holds_alternative<std::nullopt_t>(data)) {
    Q_EMIT q->failed(status, data, {});
  } else if (m_network_reply->error() != QNetworkReply::NoError)
    Q_EMIT q->error(m_network_reply->errorString(), RestReply::Error::Network,
                    {});
  else if (parse_error)
    Q_EMIT q->error(parse_error->second, parse_error->first, {});
  else if (status >= 300 && std::holds_alternative<std::nullopt_t>(data)) {
    Q_EMIT q->failed(status, data, {});
  } else {
    Q_EMIT q->succeeded(status, data, {});
  }

  if (m_retry_delay == std::chrono::milliseconds(0)) {
    retryReply();
  } else if (m_retry_delay > std::chrono::milliseconds(0)) {
    auto timer = new QTimer();
    timer->setSingleShot(true);
    timer->setTimerType(Qt::PreciseTimer);
    timer->setInterval(m_retry_delay);

    connect(timer, &QTimer::timeout, this, &RestReplyPrivate::retryReply);
    QObject::connect(timer, &QTimer::timeout, timer, &QTimer::deleteLater);

    timer->start();
  }

  m_retry_delay = std::chrono::milliseconds(-1);
}

QNetworkReply* RestReplyPrivate::send(QNetworkAccessManager* manager,
                                      const QNetworkRequest& request,
                                      const QByteArray& verb,
                                      const QByteArray& body) {
  QNetworkReply* reply = nullptr;
  if (body.isEmpty())
    reply = manager->sendCustomRequest(request, verb);
  else {
    reply = manager->sendCustomRequest(request, verb, body);
    if (reply) reply->setProperty(PropertyBody, body);
  }
  return reply;
}

}  // namespace egnite::rest

#include "egnite/rest/moc_rest_reply.cpp"