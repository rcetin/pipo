#include "serialport.h"
#include <QDebug>

void serialPort::readPortConfiguration()
{
    qDebug() << "Port Name: " << this->portName;
    qDebug() << "Baud Rate: " << this->baudRate;
    qDebug() << "Data Bits: " << this->dataBits;
    qDebug() << "Stop Bits: " << this->stopBits;
    qDebug() << "Parity: " << this->parity;
}
