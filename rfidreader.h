#ifndef RFIDREADER_H
#define RFIDREADER_H

#include <QObject>

class RFIDReader : public QObject
{
    Q_OBJECT
public:
    explicit RFIDReader(QObject *parent = nullptr);
    ~RFIDReader();

    void parseTagId(unsigned char *pTag, unsigned char bTagLength);
    bool addTagBuffer(const unsigned char *pBuffer, const unsigned short iTagLength, const unsigned short iTagNum);
    static void CallBackFunc(int msg, int param1, unsigned char *param2, int param3,unsigned char *param4);
signals:
    void tagRead(QString tagId);
};

#endif // RFIDREADER_H
