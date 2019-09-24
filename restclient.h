#ifndef RESTCLIENT_H
#define RESTCLIENT_H

#include "QSharedPointer"

class QNetworkAccessManager;
class QJsonDocument;
class Racer;

class RestClient
{
public:
    explicit RestClient();

    void races(QJsonDocument &);
    void registrations(uint8_t, QJsonDocument &);
    bool addResults(uint8_t, QList<Racer>);

private:
    QSharedPointer<QNetworkAccessManager> m_networkManager;

    QByteArray get(QUrl &url);
    QByteArray post(QUrl &url, QByteArray &data);

    void setResults();
};

#endif // RESTCLIENT_H
