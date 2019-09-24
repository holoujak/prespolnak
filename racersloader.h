#ifndef RACERSLOADER_H
#define RACERSLOADER_H

#include <QObject>
#include <QList>

#include "racer.h"
class RacersLoader
{
public:
    RacersLoader();

    QList<Racer> loadRacers(const QString &path);
    QList<Racer> loadRacersFromWeb(const uint8_t raceId);
    void storeRacers(const QString &path, const QList<Racer> &racers);
};

#endif // RACERSLOADER_H
