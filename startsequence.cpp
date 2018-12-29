#include "startsequence.h"
#include <QtCore>

startSequence::startSequence(QString &msg, QTextBrowser *br)
{
    this->msgToSend = msg;
    this->browser = br;
    qDebug() << "New sequence is created";
}

void startSequence::run()
{

    while(1)
    {
        sleep(1);
        this->browser->insertHtml("<font color=\"red\">TX : </font>");
        this->browser->insertHtml("<font color=\"black\"></font>");
        this->browser->insertHtml(this->msgToSend);
        this->browser->insertPlainText("\n");
        qDebug() << this->msgToSend;
    }
}
