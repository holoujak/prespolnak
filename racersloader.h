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
    void storeRacers(const QString &path, const QList<Racer> &racers);
};

#endif // RACERSLOADER_H
