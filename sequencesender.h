#ifndef SEQUENCESENDER_H
#define SEQUENCESENDER_H

#include <QObject>
#include <QThread>
#include <QSerialPort>
#include <QString>

class sequenceSender : public QObject
{
    Q_OBJECT
public:
    explicit sequenceSender(QObject *parent = nullptr);
    void doSetup(QThread &cThread, QSerialPort *port, char *seq, int size, int sendInterval, int sendCount, QString &txtData, int seqId);

signals:
    void exitThread();
    void countFinished(int);
    void writeToPort(char *, int, const QString &);

public slots:
    void doWork();
    void finishWork();

private:
    int isAlive();

    int threadStatus = 0;
    QSerialPort *localPort;
    char *sequence;
    int seqSize;
    int interval;
    int sendCnt;
    int id;
    QString textDat;

};

#endif // SEQUENCESENDER_H
