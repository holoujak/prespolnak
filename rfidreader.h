#ifndef RFIDREADER_H
#define RFIDREADER_H

#include <QObject>
#include <QSerialPort>
#include <QByteArray>
#include <QString>

class RFIDReader : public QObject {
    Q_OBJECT

public:
    // Konstruktor třídy RFIDReader s volitelnými názvy portů
    explicit RFIDReader(const QString &portName1 = "", const QString &portName2 = "", QObject *parent = nullptr);

signals:
    // Signál, který bude emitován, když jsou přečtena data z portu
    void tagRead(const QString tagId);

private slots:
    // Sloty pro čtení dat z obou sériových zařízení
    void handleReadyRead1();
    void handleReadyRead2();

private:
    QSerialPort *serialPort1;  // První sériový port
    QSerialPort *serialPort2;  // Druhý sériový port

    // Metoda pro nastavení sériového portu
    void setupSerialPort(QSerialPort *serialPort, const QString &portName);

    // Metoda pro výpis dat v hexadecimálním formátu
    void emitTag(const QByteArray &data, const QString &deviceName);
};

#endif // RFIDREADER_H
