#include "racersloader.h"
#include "restclient.h"

#include <QFile>
#include <QStringList>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonArray>

RacersLoader::RacersLoader()
{

}

QList<Racer> RacersLoader::loadRacers(const QString &path)
{
    QFile file(path);
    QList<Racer> racers;
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return racers;

    while(!file.atEnd()) {
        QString line = file.readLine();
        racers.append(Racer::fromString(line));
    }

    file.close();

    return racers;
}

QList<Racer> RacersLoader::loadRacersFromWeb(const uint8_t raceId)
{
    RestClient client;
    QList<Racer> racers;
    QJsonDocument racersJson;

    client.registrations(raceId, racersJson);
    QJsonArray racersArray = racersJson.array();
    for (int j = 0; j < racersArray.count(); j++)
    {
        racers.append(Racer::fromJson(racersArray.at(j).toObject()));
    }

    return racers;
}

void RacersLoader::storeRacers(const QString &path, const QList<Racer> &racers)
{
    QFile data(path);
     if (data.open(QFile::WriteOnly | QFile::Truncate)) {
         QTextStream out(&data);
         foreach (Racer racer, racers) {
            out << racer.toString() << endl;
         }
     }

     data.close();
}
