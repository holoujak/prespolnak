#include "racermodel.h"

#include <QTime>


RacerModel::RacerModel(const QMap<QString, int> &columns, QObject *parent)
    : QAbstractTableModel(parent)
{
    m_columns = columns;
}

void RacerModel::addRacer(Racer &racer)
{
    beginInsertRows({}, m_racers.count(), m_racers.count());
    m_racers.append(racer);
    endInsertRows();
}

void RacerModel::addRacers(QList<Racer> &racers)
{
    beginInsertRows({}, m_racers.count(), m_racers.count() + racers.count() - 1);
    m_racers.append(racers);
    endInsertRows();
}

QList<Racer> &RacerModel::racers()
{
    return m_racers;
}

int RacerModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_racers.count();
}

int RacerModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_columns.count();
}

QVariant RacerModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        switch (index.column()) {
            case 0: return m_racers[index.row()].startNumber();
            case 1: return m_racers[index.row()].name();
            case 2: return m_racers[index.row()].surname();
            case 3: return m_racers[index.row()].track();
            case 4: return m_racers[index.row()].city();
            case 5: return m_racers[index.row()].category();
            case 6: return m_racers[index.row()].year();
            case 7: return m_racers[index.row()].time().toString("HH:mm:ss");
            case 8: return m_racers[index.row()].timeByWiner().toString("HH:mm:ss");
            case 9: return m_racers[index.row()].categoryRank();
            case 10: return m_racers[index.row()].trackRank();
        }
    }
    return QVariant();
}

Racer RacerModel::findRacer(short startNumber)
{
    foreach (Racer racer, m_racers) {
        if (racer.startNumber() == startNumber) {
            return racer;
        }
    }

    return Racer(-1, -1, -1, "", "", "", "", "");
}

QVariant RacerModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal) {
            return m_columns.key(section);
        }
    }
    return QVariant();
}

bool RacerModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role == Qt::EditRole)
    {
        //save value from editor to member m_gridData
        Racer &racer = m_racers[index.row()];
        switch (index.column()) {
            case 0: racer.setStartNumber(value.toInt()); break;
            case 1: racer.setName(value.toString()); break;
            case 2: racer.setSurname(value.toString()); break;
            case 3: racer.setTrack(value.toString()); break;
            case 4: racer.setCity(value.toString()); break;
            case 5: racer.setCategory(value.toString()); break;
            case 6: racer.setYear(value.toString()); break;
            case 7: racer.setTime(QTime::fromString(value.toString(), "HH:mm:ss")); break;
            case 8: racer.setTimeByWinner(QTime::fromString(value.toString(), "HH:mm:ss")); break;
            case 9: racer.setCategoryRank(value.toInt()); break;
            case 10: racer.setTrackRank(value.toInt()); break;
        }
    }
    return true;
}

Qt::ItemFlags RacerModel::flags(const QModelIndex &index) const
{
    Q_UNUSED(index)
    return Qt::ItemIsSelectable |  Qt::ItemIsEditable | Qt::ItemIsEnabled;
}
