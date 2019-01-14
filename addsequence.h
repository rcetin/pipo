#ifndef ADDSEQUENCE_H
#define ADDSEQUENCE_H

#include <QDialog>
#include <QLinkedList>

namespace Ui {
class addSequence;
}

class addSequence : public QDialog
{
    Q_OBJECT

public:
    explicit addSequence(QWidget *parent = 0);
    void reject();
    ~addSequence();


private slots:

    void on_buttonBox_accepted();

    void on_hexInput_textChanged();

signals:
    void sendSeqInfo(const QString &, const QString &, int);

private:
    Ui::addSequence *ui;

    int serSeqParamErr = 0;

};

#endif // ADDSEQUENCE_H
