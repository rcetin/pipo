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
}

void sequenceSender::doWork()
{
    qDebug() << "status: " << localPort->isOpen();
    threadStatus = 1;

    while(isAlive())
    {
        emit writeToPort((char *) sequence.toLocal8Bit().constData(), sequence.size());
        QThread::sleep(interval);
    }

    emit exitThread();
}

/**
 * @brief sequenceSender::finishWork
 *
 * This will stop already started threads.
 */
void sequenceSender::finishWork()
{
    threadStatus = 0;
}

int sequenceSender::isAlive()
{
    return threadStatus;
}
