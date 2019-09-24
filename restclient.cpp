#include "restclient.h"

#include "racer.h"

#include "QtNetwork/QNetworkRequest"
#include "QtNetwork/QNetworkReply"
#include "QtNetwork/QNetworkAccessManager"
#include <QJsonDocument>
#include <QJsonArray>
#include <QEventLoop>

const QString AUTH_USERNAME = "user";
const QString AUTH_PASSWORD = "pass";
const QString SERVER_URL = "server_url";

RestClient::RestClient() : m_networkManager(QSharedPointer<QNetworkAccessManager>(new QNetworkAccessManager()))
{
}

QByteArray RestClient::get(QUrl &url)
{
    url.setUserName(AUTH_USERNAME);
    url.setPassword(AUTH_PASSWORD);

    auto reply = m_networkManager->get(QNetworkRequest(url));
    QEventLoop eventLoop;
    QObject::connect(reply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
    eventLoop.exec();

    if (reply->error())
    {
        qDebug() << reply->errorString();
    }
    return reply->readAll();
}

QByteArray RestClient::post(QUrl &url, QByteArray &data)
{
    url.setUserName(AUTH_USERNAME);
    url.setPassword(AUTH_PASSWORD);

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,
                      QStringLiteral("Content-Type: application/json; charset=utf-8"));
    auto reply = m_networkManager->post(request, data);
    QEventLoop eventLoop;
    QObject::connect(reply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
    eventLoop.exec();

    if (reply->error())
    {
        qDebug() << reply->errorString();
        qDebug() << reply->readAll();
        return QByteArray();
    }
    return reply->readAll();
}

void RestClient::races(QJsonDocument &racers)
{
    QUrl url(QString("%1/races").arg(SERVER_URL));
    racers = QJsonDocument::fromJson(get(url));
}

void RestClient::registrations(uint8_t raceID, QJsonDocument &registrations)
{
    QString apiUrl = QString("%1/races/%2/registrations").arg(SERVER_URL).arg(raceID);
    QUrl url(apiUrl);
    registrations = QJsonDocument::fromJson(get(url));
}

bool RestClient::addResults(uint8_t raceId, QList<Racer> racers)
{
    QJsonArray results;
    QJsonObject root;

    for (const auto racer : racers)
    {
        QJsonObject racerJson;
        racerJson.insert("registrationId", racer.id());
        racerJson.insert("startTime", racer.startTime().toString("yyyy-MM-dd hh:mm:ss.zzz"));
        racerJson.insert("finishTime", racer.finishTime().toString("yyyy-MM-dd hh:mm:ss.zzz"));

        results.push_back(racerJson);
    }

    root.insert("results", results);
    QJsonDocument allResults(root);

    QString apiUrl = QString("%1/races/%2/results").arg(SERVER_URL).arg(raceId);
    QUrl url(apiUrl);
    QByteArray resultJson = allResults.toJson(QJsonDocument::Compact);
    qDebug() << resultJson;
    auto result = post(url, resultJson);
    int x;
}
