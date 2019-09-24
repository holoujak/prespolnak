#ifndef RACER_H
#define RACER_H

#include <QString>
#include <QTime>
#include <QJsonObject>

class Racer
{
public:
    Racer(short id, short startNumber, short year, QString name, QString surname, QString track, QString category, QString city);

    short id() const;
    short startNumber() const;
    QString name() const;
    QString track() const;
    QString category() const;
    QString city() const;
    QTime time() const;
    QDateTime startTime() const;
    QDateTime finishTime() const;

    void setTime(const QTime &time);
    void setStartTime(const QDateTime &time);
    void setFinishTime(const QDateTime &time);

    void setId(short id);

    void setStartNumber(short startNumber);

    void setName(const QString &name);

    void setTrack(const QString &track);

    void setCategory(const QString &category);

    void setCity(const QString &city);

    QString toString();

    static Racer fromString(QString racerString);
    static Racer fromJson(QJsonObject racerJson);

    QString surname() const;
    void setSurname(const QString &surname);

    short trackRank() const;
    void setTrackRank(short trackRank);

    short categoryRank() const;
    void setCategoryRank(short categoryRank);

    QTime timeByWiner() const;
    void setTimeByWinner(const QTime &timeByWiner);

    QString year() const;
    void setYear(const QString &year);

private:
    short m_id;
    short m_startNumber;
    short m_trackRank;
    short m_categoryRank;
    QString m_name;
    QString m_surname;
    QString m_track;
    QString m_category;
    QString m_city;
    QString m_year;
    QTime m_time;
    QDateTime m_startTime;
    QDateTime m_finishTime;
    QTime m_timeByWinner;
};

#endif // RACER_H
