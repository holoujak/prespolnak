#ifndef READER_H
#define READER_H

#include <QDialog>

#include "rfidreader.h"

namespace Ui {
class Reader;
}

class Reader : public QDialog
{
    Q_OBJECT

public:
    explicit Reader(RFIDReader* rfidReader, QWidget *parent = nullptr);
    ~Reader();

private:
    Ui::Reader *ui;
    RFIDReader* m_rfidReader;

private slots:
    void onNewTagRead(QString tagId);
};

#endif // READER_H
