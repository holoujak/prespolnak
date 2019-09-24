#ifndef RACERMODEL_H
#define RACERMODEL_H

#include <QObject>
#include <QMap>
#include <QAbstractTableModel>

#include "racer.h"

const QMap<QString, int> DEFAULT_RACERS_COLUMNS = {
    {"Cislo", 0},
    {"Jmeno", 1},
    {"Prijmeni", 2},
    {"Trat", 3},
    {"Mesto", 4},
    {"Kategorie", 5},
    {"Rocnik", 6},
};
class RacerModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    RacerModel(const QMap<QString, int> &columns, QObject *parent = 0);
    void addRacer(Racer &racer);
    void addRacers(QList<Racer> &racers);

    QList<Racer> &racers();

private:
    void initializeModel();

private:
    QList<Racer> m_racers;
    QMap<QString, int> m_columns;
    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
    Qt::ItemFlags flags(const QModelIndex & index) const ;
    QMap<QString, int> &columns();

    Racer findRacer(short startNumber);

    // QAbstractItemModel interface
};

#endif // RACERMODEL_H
