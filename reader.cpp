#include "reader.h"
#include "ui_reader.h"

#include <QTableWidget>

Reader::Reader(RFIDReader* rfidReader, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Reader),
    m_rfidReader(rfidReader)
{
    ui->setupUi(this);
    connect(m_rfidReader, &RFIDReader::tagRead, this, &Reader::onNewTagRead);
}

Reader::~Reader()
{
    disconnect(m_rfidReader, &RFIDReader::tagRead, this, &Reader::onNewTagRead);
    delete ui;
}

void Reader::onNewTagRead(QString tagId)
{
    ui->tableWidget->insertRow(0);
    ui->tableWidget->setItem(0,0,new QTableWidgetItem(tagId));

    ui->tableWidget->viewport()->update();
}
