#ifndef SEQUENCESENDER_H
#define SEQUENCESENDER_H

#include <QObject>
#include <QThread>

class sequenceSender : public QObject
{
    Q_OBJECT
public:
    explicit sequenceSender(QObject *parent = nullptr);
    void doSetup(QThread &cThread);

signals:
    void exitThread();
public slots:
    void doWork();
    void finish();
};

#endif // SEQUENCESENDER_H
