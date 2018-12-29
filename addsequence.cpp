#include "addsequence.h"
#include "ui_addsequence.h"
#include "mainwindow.h"

addSequence::addSequence(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addSequence)
{
    ui->setupUi(this);

}

addSequence::~addSequence()
{
    delete ui;
}

void addSequence::on_buttonBox_accepted()
{
    emit sendSeqInfo(ui->seqName->toPlainText(), ui->seqData->toPlainText());
    ui->seqName->clear();
    ui->seqData->clear();
}
