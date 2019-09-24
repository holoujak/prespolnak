#include "racer.h"

#include <QVariantMap>

QString RACER_SEPARATOR = ";";

Racer::Racer(short id, short startNumber, short year, QString name, QString surname, QString track, QString category, QString city)
{
    m_id = id;
    m_startNumber = startNumber;
    m_year = year;
    m_name = name.trimmed();
    m_surname = surname.trimmed();
    m_track = track.trimmed();
    m_category = category.trimmed();
    m_city = city.trimmed();
    m_startTime = QDateTime();
    m_finishTime = QDateTime();
}

short Racer::id() const
{
    return m_id;
}

short Racer::startNumber() const
{
    return m_startNumber;
}

QString Racer::name() const
{
    return m_name;
}

QString Racer::track() const
{
    return m_track;
}

QString Racer::category() const
{
    return m_category;
}

QString Racer::city() const
{
    return m_city;
}

QTime Racer::time() const
{
    return m_time;
}

void Racer::setTime(const QTime &time)
{
    if (m_timeByWinner != QTime(0, 0, 0) && m_time != QTime(0, 0, 0)) {
        m_timeByWinner = m_timeByWinner.addSecs(m_time.secsTo(time));
    }
    m_time = time;
}

void Racer::setStartTime(const QDateTime &time)
{
    m_startTime = time;
}

void Racer::setFinishTime(const QDateTime &time)
{
    m_finishTime = time;
}

void Racer::setId(short id)
{
    m_id = id;
}

void Racer::setStartNumber(short startNumber)
{
    m_startNumber = startNumber;
}

void Racer::setName(const QString &name)
{
    m_name = name.trimmed();
}

void Racer::setTrack(const QString &track)
{
    m_track = track.trimmed();
}

void Racer::setCategory(const QString &category)
{
    m_category = category.trimmed();
}

void Racer::setCity(const QString &city)
{
    m_city = city.trimmed();
}

QString Racer::toString()
{
    QString racerString = QString::number(m_id) + RACER_SEPARATOR + QString::number(m_startNumber) + RACER_SEPARATOR +
                          m_name + RACER_SEPARATOR + m_surname + RACER_SEPARATOR + m_track + RACER_SEPARATOR +
                          m_city + RACER_SEPARATOR + m_category + RACER_SEPARATOR + m_year + RACER_SEPARATOR +
                          m_time.toString().trimmed() + RACER_SEPARATOR + m_timeByWinner.toString().trimmed() +
                          RACER_SEPARATOR + QString::number(m_categoryRank) + RACER_SEPARATOR + QString::number(m_trackRank);
    return racerString;
}

Racer Racer::fromString(QString racerString)
{
    Racer racer(-1, -1, -1, "", "", "", "", "");
    QStringList parts = racerString.trimmed().replace("\"", "").split(RACER_SEPARATOR);

    for (int i = 0; i < parts.length(); i++) {
        QString trimmed = parts[i].trimmed();
        switch (i) {
            case 0: racer.setId(trimmed.toShort()); break;
            case 1: racer.setStartNumber(trimmed.toShort()); break;
            case 2: racer.setName(trimmed); break;
            case 3: racer.setSurname(trimmed); break;
            case 4: racer.setTrack(trimmed); break;
            case 5: racer.setCity(trimmed); break;
            case 6: racer.setCategory(trimmed); break;
            case 7: racer.setYear(trimmed); break;
            case 8: racer.setTime(QTime::fromString(trimmed)); break;
            case 9: racer.setTimeByWinner(QTime::fromString(trimmed)); break;
            case 10: racer.setCategoryRank(trimmed.toShort()); break;
            case 11: racer.setTrackRank(trimmed.toShort()); break;
        }
    }

    return racer;
}

Racer Racer::fromJson(QJsonObject racerJson)
{
    Racer racer(-1, -1, -1, "", "", "", "", "");
    QVariantMap mainMap = racerJson.toVariantMap();
    int racerStartNumber = 1 + ( std::rand() % ( 100 - 1 + 1 ) );
    racer.setId(mainMap["id"].toInt());
    racer.setStartNumber(mainMap["startNumber"].toInt());
    racer.setStartNumber(racerStartNumber);
    racer.setName(mainMap["firstName"].toString());
    racer.setSurname(mainMap["lastName"].toString());
    racer.setCategory(mainMap["categories"].toList()[0].toMap()["name"].toString());
    return racer;
}

QString Racer::surname() const
{
    return m_surname;
}

void Racer::setSurname(const QString &surname)
{
    m_surname = surname;
}

short Racer::trackRank() const
{
    return m_trackRank;
}

void Racer::setTrackRank(short trackRank)
{
    m_trackRank = trackRank;
}

short Racer::categoryRank() const
{
    return m_categoryRank;
}

void Racer::setCategoryRank(short categoryRank)
{
    m_categoryRank = categoryRank;
}

QTime Racer::timeByWiner() const
{
    return m_timeByWinner;
}

QDateTime Racer::startTime() const
{
    return m_startTime;
}

QDateTime Racer::finishTime() const
{
    return m_finishTime;
}

void Racer::setTimeByWinner(const QTime &timeByWiner)
{
    m_timeByWinner = timeByWiner;
}

QString Racer::year() const
{
    return m_year;
}

void Racer::setYear(const QString &year)
{
    m_year = year;
}
