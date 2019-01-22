#include "sequencesender.h"
#include <QDebug>
#include <QString>

sequenceSender::sequenceSender(QObject *parent) : QObject(parent)
{

}

void sequenceSender::doSetup(QThread &cThread, QSerialPort *port, char *seq, int size, int sendInterval, int sendCount, QString &txtData, int seqId)
{
    localPort = port;
    sequence = seq;
    seqSize = size;
    interval = sendInterval;
    textDat = txtData;
    sendCnt = sendCount;
    id = seqId;
    connect(&cThread, SIGNAL(started()), this, SLOT(doWork()));
    connect(this, SIGNAL(exitThread()), &cThread, SLOT(quit()));
    connect(&cThread, SIGNAL(finished()), &cThread, SLOT(deleteLater()));
}

void sequenceSender::doWork()
{
    qDebug() << "status: " << localPort->isOpen();
    threadStatus = 1;
    int localCnt = 0;
    while(isAlive())
    {
        emit writeToPort(sequence, seqSize, textDat);
        QThread::msleep(interval);
        if(sendCnt)
        {
            localCnt++;
            if(localCnt == sendCnt)
            {
                countFinished(id);
                sequenceSender::finishWork();
            }
        }
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
