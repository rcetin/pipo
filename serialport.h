#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QString>
#include <QDebug>

class serialPort
{
public:
    serialPort(QString portName, int baudRate, int dataBits, int stopBits, QString parity): portName(portName),
        baudRate(baudRate), dataBits(dataBits), stopBits(stopBits), parity(parity)
    {qDebug() << "New serial port is created!";}

    int open();
    int close();
    int write();
    int read();
    void readPortConfiguration();

private:
    QString portName;
    int baudRate;
    int dataBits;
    int stopBits;
    QString parity;
};

#endif // SERIALPORT_H
