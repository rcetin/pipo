#ifndef STARTSEQUENCE_H
#define STARTSEQUENCE_H

#include <QtCore>
#include <QTextBrowser>

class startSequence:public QThread
{
public:
    QString msgToSend;
    QTextBrowser *browser;
    startSequence(QString &, QTextBrowser *br);
    void run();
};

#endif // STARTSEQUENCE_H
