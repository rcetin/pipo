#ifndef ADDSEQUENCE_H
#define ADDSEQUENCE_H

#include <QDialog>

namespace Ui {
class addSequence;
}

class addSequence : public QDialog
{
    Q_OBJECT

public:
    explicit addSequence(QWidget *parent = 0);
    ~addSequence();

private slots:

    void on_buttonBox_accepted();

signals:
    void sendSeqInfo(const QString &, const QString &);

private:
    Ui::addSequence *ui;
};

#endif // ADDSEQUENCE_H
