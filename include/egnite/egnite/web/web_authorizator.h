#ifndef EGNITE_WEB_AUTHORIZATOR_H
#define EGNITE_WEB_AUTHORIZATOR_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QNetworkRequest>
#include <QObject>
/* ----------------------------------- Local -------------------------------- */
#include "egnite/egnite/export.h"
/* -------------------------------------------------------------------------- */

namespace egnite::web
{

  class EGNITE_API WebAuthorizator : public QObject
  {
    Q_OBJECT

  public:
    explicit WebAuthorizator(QObject *parent = nullptr);
    ~WebAuthorizator() override;

    virtual void setRequestHeaders(QNetworkRequest &request) = 0;
  };

  class EGNITE_API ApiKeyAuthorizator : public WebAuthorizator
  {
    Q_OBJECT

  public:
    Q_PROPERTY(
      QString apiKey READ getApiKey WRITE setApiKey NOTIFY apiKeyChanged)

  public:
    explicit ApiKeyAuthorizator(QObject *parent = nullptr);
    ~ApiKeyAuthorizator() override;

    void setRequestHeaders(QNetworkRequest &request) override;

    [[nodiscard]] QString getApiKey() const;
    void setApiKey(const QString &api_key);

  Q_SIGNALS:
    void apiKeyChanged(const QString &api_key);

  private:
    QString m_api_key;
  };

}// namespace egnite::web

#endif// EGNITE_WEB_AUTHORIZATOR_H
