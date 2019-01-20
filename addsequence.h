#ifndef ADDSEQUENCE_H
#define ADDSEQUENCE_H

#include <QDialog>
#include <QLinkedList>
#include <QString>
#include "serialseq.h"

namespace Ui {
class addSequence;
}

class addSequence : public QDialog
{
    Q_OBJECT

public:
    explicit addSequence(QWidget *parent = 0);
    addSequence(QWidget *parent, serialSequenceElem *elem);
    void reject();
    ~addSequence();


private slots:

    void on_buttonBox_accepted();

    void on_hexInput_textChanged();

signals:
    void sendAsciiSeqInfo(const QString &, const QString &, int, int, int);
    void sendHexSeqInfo(const QString &, const QString &, const QByteArray &, int);

private:
    Ui::addSequence *ui;

    int serSeqParamErr = 0;
    int editFlag;
    int curSeqID;

};

#endif // ADDSEQUENCE_H
