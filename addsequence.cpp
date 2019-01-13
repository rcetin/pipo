#include "addsequence.h"
#include "ui_addsequence.h"
#include "mainwindow.h"
#include <QMessageBox>
#include <QCloseEvent>
#include <QDebug>

addSequence::addSequence(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addSequence)
{
    ui->setupUi(this);
}

void addSequence::reject()
{
    qDebug() << "Here!";
    if (this->serSeqParamErr == 1)
    {
        //event->ignore(); // keep window
        this->serSeqParamErr = 0;
    } else {
        QDialog::reject(); // close window
    }
}

addSequence::~addSequence()
{
    delete ui;
}

void addSequence::on_buttonBox_accepted()
{
    if(ui->seqName->toPlainText() == NULL)
    {
           QMessageBox::critical(this, "Serial Sequence Name Error!", "Sequence name cannot be empty!");
           this->serSeqParamErr = 1;
           return;
    }

    if(ui->seqData->toPlainText() == NULL)
    {
           QMessageBox::critical(this, "Serial Sequence Data Error!", "Sequence data cannot be empty!");
           this->serSeqParamErr = 1;
           return;
    }

    emit sendSeqInfo(ui->seqName->toPlainText(), ui->seqData->toPlainText(), ui->seq_period->toPlainText().toInt(0, 10));

    qDebug() << "Val: " <<ui->seq_period->toPlainText().toInt(0, 10);
    ui->seqName->clear();
    ui->seqData->clear();
    ui->seq_period->clear();
}
