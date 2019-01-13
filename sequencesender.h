#ifndef SEQUENCESENDER_H
#define SEQUENCESENDER_H

#include <QObject>
#include <QThread>
#include <QSerialPort>

class sequenceSender : public QObject
{
    Q_OBJECT
public:
    explicit sequenceSender(QObject *parent = nullptr);
    void doSetup(QThread &cThread, QSerialPort *port, QString seq, int sendInterval);

signals:
    void exitThread();
    void writeToPort(char *, int);

public slots:
    void doWork();
    void finish();

private:
    QSerialPort *localPort;
    QString sequence;
    int interval;
};

#endif // SEQUENCESENDER_H
