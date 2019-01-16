#include "addsequence.h"
#include "ui_addsequence.h"
#include "mainwindow.h"
#include <QMessageBox>
#include <QCloseEvent>
#include <QDebug>
#include <QStringList>

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
    if(((ui->seqName->toPlainText() == NULL) && (ui->hexName->toPlainText() == NULL)) ||
            ((ui->seqName->toPlainText() != NULL) && (ui->hexName->toPlainText() != NULL)))
    {
           QMessageBox::critical(this, "Error!", "Sequence name is empty or multiple sequence names are written!");
           this->serSeqParamErr = 1;
           return;
    }

    if(((ui->seqData->toPlainText() == NULL) && (ui->hexInput->toPlainText() == NULL)) ||
        ((ui->seqData->toPlainText() != NULL) && (ui->hexInput->toPlainText() != NULL)))
    {
           QMessageBox::critical(this, "Error!", "Sequence data is empty or multiple sequence data is written!");
           this->serSeqParamErr = 1;
           return;
    }

    // Now just hex or string data is written.
    if(ui->hexInput->toPlainText() != NULL)
    {
        // Hex input is given.
        QString input = ui->hexInput->toPlainText();
        QStringList list1 = input.split(" ");
        QByteArray sendData;
        for(int i = 0; i < list1.size(); i++)
        {
            QString ran = list1[i];
            bool ok;
            sendData[i]=ran.toInt(&ok, 16);
        }

        qDebug() << "hex in: " << ui->hexInput->toPlainText();
        qDebug() << "hex byte array: " << sendData;
        sendHexSeqInfo(ui->hexName->toPlainText(), ui->hexInput->toPlainText(), sendData, ui->hexPeriod->toPlainText().toInt(0, 10));
    }
    else
    {
        // Ascii input is given
        emit sendAsciiSeqInfo(ui->seqName->toPlainText(), ui->seqData->toPlainText(), ui->seq_period->toPlainText().toInt(0, 10));
    }

    qDebug() << "Val: " <<ui->seq_period->toPlainText().toInt(0, 10);
    ui->seqName->clear();
    ui->seqData->clear();
    ui->seq_period->clear();
}


void addSequence::on_hexInput_textChanged()
{
    int wrongInput = 0;
    QTextCursor curs = ui->hexInput->textCursor();
    int curPos = curs.position();
    QString text = ui->hexInput->toPlainText().toUpper();
    int sizeTxt = text.size();
    int spaceCnt = text.count(QLatin1Char(' '));
    QString lastTxt = text.replace(QRegExp("[^0-9A-F]"), "");

    if((sizeTxt - spaceCnt) > lastTxt.size())
        wrongInput = 1;

    QStringList tokens;
    for(int i = 0; i < text.length(); i += 2) {
      tokens << text.mid(i, 2);
    }
    ui->hexInput->blockSignals(true);
    QString lastStr = tokens.join(" ");
    ui->hexInput->setText(lastStr);
    ui->hexInput->moveCursor(QTextCursor::NoMove);
    if(wrongInput == 0)
    {
        if(curPos % 3 == 0)
        {
            curPos += 1;
            curs.setPosition(curPos);
            ui->hexInput->setTextCursor(curs);
        }
        else
        {
            curs.setPosition(curPos);
            ui->hexInput->setTextCursor(curs);
        }
    }
    else
    {
        curs.setPosition(curPos - 1);
        ui->hexInput->setTextCursor(curs);
    }
    ui->hexInput->blockSignals(false);
}
