#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QString>
#include <QDebug>
#include <QSerialPort>

class serialPort : public QSerialPort
{
public:
    serialPort(QString portName, int baudRate, int dataBits, int stopBits, QString parity): portName(portName),
        baudRate(baudRate), dataBits(dataBits), stopBits(stopBits), parity(parity)
    {qDebug() << "New serial port is created!";}

    void readPortConfiguration();

    QString portName;
    int baudRate;
    int dataBits;
    int stopBits;
    QString parity;

private:
};

#endif // SERIALPORT_H
