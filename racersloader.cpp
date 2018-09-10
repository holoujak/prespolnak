#include "racersloader.h"

#include <QFile>
#include <QStringList>
#include <QTextStream>

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
