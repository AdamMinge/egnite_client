#ifndef EGNITE_WEB_CLIENT_H
#define EGNITE_WEB_CLIENT_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QNetworkAccessManager>
#include <QObject>
/* -------------------------------------------------------------------------- */

namespace egnite::web
{

  class WebClient : public QObject
  {
    Q_OBJECT

  public:
    Q_PROPERTY(
      QUrl base_url READ getBaseUrl WRITE setBaseUrl NOTIFY baseUrlChanged)

  public:
    explicit WebClient();
    ~WebClient() override;

    [[nodiscard]] QUrl getBaseUrl() const;
    void setBaseUrl(const QUrl &base_url);

  Q_SIGNALS:
    void baseUrlChanged(const QUrl &base_url);

  private:
    QUrl m_base_url;
  };

}// namespace egnite::web

#endif// EGNITE_WEB_CLIENT_H
