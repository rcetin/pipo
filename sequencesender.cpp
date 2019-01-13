#include "sequencesender.h"
#include <QDebug>

sequenceSender::sequenceSender(QObject *parent) : QObject(parent)
{

}

void sequenceSender::doSetup(QThread &cThread)
{
    connect(&cThread, SIGNAL(started()), this, SLOT(doWork()));
    connect(this, SIGNAL(exitThread()), &cThread, SLOT(quit()));
    connect(&cThread, SIGNAL(finished()), &cThread, SLOT(deleteLater()));
    connect(&cThread, SIGNAL(finished()), this, SLOT(finish()));
}

void sequenceSender::doWork()
{
    for(int i = 0; i < 5; i++)
    {
        qDebug() << "Test: " << i;
        QThread::sleep(1);
    }

    emit exitThread();
}

void sequenceSender::finish()
{
    qDebug() << "Thread is finished";
}
