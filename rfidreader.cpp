#include "rfidreader.h"
#include <QDebug>
#include <cstdio>

// Konstruktor RFIDReader, který inicializuje sériové porty
RFIDReader::RFIDReader(const QString &portName1, const QString &portName2, QObject *parent)
    : QObject(parent), serialPort1(new QSerialPort(this)), serialPort2(new QSerialPort(this)) {

    // Nastavení prvního sériového portu
    if (!portName1.isEmpty()) {
        setupSerialPort(serialPort1, portName1);
    }

    // Nastavení druhého sériového portu
    if (!portName2.isEmpty()) {
        setupSerialPort(serialPort2, portName2);
    }
}

// Nastavení sériového portu
void RFIDReader::setupSerialPort(QSerialPort *serialPort, const QString &portName) {
    serialPort->setPortName(portName);
    serialPort->setBaudRate(QSerialPort::Baud115200);
    serialPort->setDataBits(QSerialPort::Data8);
    serialPort->setParity(QSerialPort::NoParity);
    serialPort->setStopBits(QSerialPort::OneStop);
    serialPort->setFlowControl(QSerialPort::NoFlowControl);

    if (serialPort->open(QIODevice::ReadWrite)) {
        qDebug() << "Successfully opened" << portName;

        // Připojení signálů pro čtení dat
        if (serialPort == serialPort1) {
            connect(serialPort, &QSerialPort::readyRead, this, &RFIDReader::handleReadyRead1);
        } else if (serialPort == serialPort2) {
            connect(serialPort, &QSerialPort::readyRead, this, &RFIDReader::handleReadyRead2);
        }
    } else {
        qWarning() << "Failed to open" << portName << serialPort->errorString();
    }
}

// Zpracování dat z prvního zařízení
void RFIDReader::handleReadyRead1() {
    QByteArray data = serialPort1->readAll();
    emitTag(data, "Device 1");
}

// Zpracování dat z druhého zařízení
void RFIDReader::handleReadyRead2() {
    QByteArray data = serialPort2->readAll();
    emitTag(data, "Device 2");
}

// Výpis dat v hexadecimálním formátu
void RFIDReader::emitTag(const QByteArray &data, const QString &deviceName) {
    qDebug() << "Data from" << deviceName << "in hexadecimal format:";
    qDebug() << data.toHex();

    if (data.size() > 14) {
        emit tagRead(data.right(14).left(12).toHex().toUpper());
    }
}
