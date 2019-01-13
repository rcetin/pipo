#include "sequencesender.h"
#include <QDebug>

sequenceSender::sequenceSender(QObject *parent) : QObject(parent)
{

}

void sequenceSender::doSetup(QThread &cThread, QSerialPort *port, QString seq, int sendInterval)
{
    localPort = port;
    sequence = seq;
    interval = sendInterval;
    connect(&cThread, SIGNAL(started()), this, SLOT(doWork()));
    connect(this, SIGNAL(exitThread()), &cThread, SLOT(quit()));
    connect(&cThread, SIGNAL(finished()), &cThread, SLOT(deleteLater()));
    connect(&cThread, SIGNAL(finished()), this, SLOT(finish()));
}

void sequenceSender::doWork()
{
    qDebug() << "status: " << localPort->isOpen();

    for(int i = 0; i < 5; i++)
    {
        emit writeToPort((char *) sequence.toLocal8Bit().constData(), sequence.size());
        QThread::sleep(interval);
    }

    emit exitThread();
}

void sequenceSender::finish()
{
    qDebug() << "Thread is finished";
}
