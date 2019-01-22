#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QDebug"
#include "addsequence.h"
#include <QGridLayout>
#include <QLabel>
#include <QToolButton>
#include <QPushButton>
#include <QLineEdit>
#include <QScrollArea>
#include <QSizePolicy>
#include <QScrollBar>
#include "serialseq.h"
#include <QGridLayout>
#include "serialconfig.h"
#include <QMetaEnum>
#include <QMessageBox>
#include "sequencesender.h"
#include <QDateTime>
#include <QFileDialog>
#include <QDir>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->pushButton->setToolTip("Add New Serial Sequence");
    QFont mono("Ubuntu Mono", 11, QFont::Normal);
    ui->textBrowser->setFont(mono);

    portConfig.status = CLOSED;
}

MainWindow::~MainWindow()
{
    delete ui;
    delete port;
    delete serialConf;
    delete newSerialSeq;
}

/**
 * @brief MainWindow::on_actionSerial_Port_triggered
 *
 * This function is triggered when new serial config
 * button is clicked on Menubar or Menu.
 */
void MainWindow::on_actionSerial_Port_triggered()
{
    serialConf = new serialConfig(this);
    connect(serialConf, SIGNAL(sendNewSerialPortInfo(const QString , int , int , int , int )), this, SLOT(createNewSerialPort(const QString , int , int , int , int )));
    serialConf->setModal(true);
    serialConf->exec();
}

/**
 * @brief MainWindow::on_actionQuit_triggered
 *
 * This function is triggered when Quit option is
 * clicked in File->Quit Menu.
 */
void MainWindow::on_actionQuit_triggered()
{
    QApplication::quit();
}

/**
 * @brief MainWindow::on_actionStart_triggered
 *
 * This function is triggered when play button in
 * menubar is clicked.
 */
void MainWindow::on_actionStart_triggered()
{
    if(this->port == NULL)
        return;

    if(portConfig.status == OPENED)
    {
        //qDebug() << "port already open!";
        return;
    }

    int ret = this->port->open(QIODevice::ReadWrite);
    qDebug() << "port open retval: " << ret << " name: " << port->portName() << "isope:" << port->isOpen();

    if(ret == false)
    {
        QSerialPort::SerialPortError a;
        a = this->port->error();
        QMetaEnum err= QMetaEnum::fromType<QSerialPort::SerialPortError>();

        QMessageBox::critical(this, "Port Open Error", err.valueToKey(int(a)));
        return;
    }

    portConfig.status = OPENED;
    QFont mono("Ubuntu Mono", 11, QFont::Bold);
    ui->pStatus->setText("Status: Open");
    ui->pStatus->setFont(mono);

    // Set scroll bar to bottom
    // QScrollBar *sb = ui->textBrowser->verticalScrollBar();
    // sb->setValue(sb->maximum());
}

/**
 * @brief MainWindow::addSerialSequence
 * @param seqName:  Name of Serial Sequence
 * @param seqData:  Data of Serial Sequence
 *
 * @note This function is used to add serial sequence to memory.
 */
void MainWindow::addAsciiSequence(const QString & seqName, const QString &seqData, int seqPeriod, int sendCount, int edit, int seqId)
{
    if(edit)
    {
        struct serialSequenceElem *currentSeq = this->serialseq.findSerialSeq(seqId);

        if(currentSeq->status == 1)
        {
            currentSeq->sender->finishWork();
            currentSeq->status = 0;
            currentSeq->button->setIcon(QIcon("/home/rcetin/workspace/qt_projects/pipo/img/st_seq.png"));
        }

        QString format("<font color=\"%1\"><b>[%2]:</b></font>");

        currentSeq->labelName->setText(format.arg("black", seqName.left(MAX_VISIBLE_SEQ_DATA_LEN)));
        currentSeq->labelName->setToolTip(QString("Seq Name: %1").arg(seqName));

        currentSeq->labelData->setText("[" + seqData.left(MAX_VISIBLE_SEQ_DATA_LEN) + "]");
        currentSeq->labelData->setToolTip(QString("Data: %1").arg(seqData));

        if(seqPeriod)
            currentSeq->labelPeriod->setText("[" + QString::number(seqPeriod, 10).left(MAX_VISIBLE_SEQ_PER_LEN) + "]");
        else
            currentSeq->labelPeriod->setText("[N/A]");
        currentSeq->labelPeriod->setToolTip(QString("Period: %1").arg(QString::number(seqPeriod, 10)));

        char dat[MAX_SENDABLE_DATA_LEN] = {0};
        memcpy( dat, seqData.toStdString().c_str() ,seqData.size());

        serialseq.editSeq(currentSeq, seqPeriod, sendCount, seqName, dat, seqData.size(), seqData);
        return;
    }

    QPushButton *stBut = new QPushButton(this);
    stBut->setIcon(QIcon("/home/rcetin/workspace/qt_projects/pipo/img/st_seq.png"));
    stBut->setFixedSize(QSize(30, 20));
    stBut->setProperty("butId", this->gridLayLastRow);
    connect(stBut, SIGNAL(clicked(bool)), this, SLOT(on_serialSeqStartButton_clicked()));
    ui->gridLayout->addWidget(stBut, this->gridLayLastRow, 0);

    QPushButton *editBut = new QPushButton(this);
    editBut->setIcon(QIcon("/home/rcetin/workspace/qt_projects/pipo/img/edit_seq_but.png"));
    editBut->setFixedSize(QSize(30, 20));
    editBut->setProperty("butId", this->gridLayLastRow);
    connect(editBut, SIGNAL(clicked(bool)), this, SLOT(on_seq_edit_but_clicked()));
    ui->gridLayout->addWidget(editBut, this->gridLayLastRow, 1);

    QFont mono("Ubuntu Mono", 11, QFont::Normal);

    QString format("<font color=\"%1\"><b>[%2]:</b></font>");

    QLabel *lName = new QLabel(format.arg("black", seqName.left(MAX_VISIBLE_SEQ_DATA_LEN)), this);
    lName->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    lName->setMinimumWidth(40);
    lName->setFont(mono);
    lName->setToolTip(QString("Seq Name: %1").arg(seqName));
    ui->gridLayout->addWidget(lName, this->gridLayLastRow, 2);

    QLabel *lData = new QLabel("[" + seqData.left(MAX_VISIBLE_SEQ_DATA_LEN) + "]", this);
    lData->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    lData->setMinimumWidth(40);
    lData->setFont(mono);
    lData->setToolTip(QString("Data: %1").arg(seqData));
    ui->gridLayout->addWidget(lData, this->gridLayLastRow, 3);

    QLabel *lPer;

    if(seqPeriod)
        lPer = new QLabel("[" + QString::number(seqPeriod, 10).left(MAX_VISIBLE_SEQ_PER_LEN) + "]", this);
    else
        lPer = new QLabel("[N/A]", this);
    lPer->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    lPer->setMinimumWidth(40);
    lPer->setFont(mono);
    lPer->setToolTip(QString("Period: %1").arg(QString::number(seqPeriod, 10)));
    ui->gridLayout->addWidget(lPer, this->gridLayLastRow, 4);

    char data[MAX_SENDABLE_DATA_LEN] = {0};
    memcpy( data, seqData.toStdString().c_str() ,seqData.size());
    this->serialseq.addSeqToList(this->gridLayLastRow, seqPeriod, sendCount, seqName, data, seqData.size(), seqData, stBut, SERIAL_SEQ_TYPE_ASCII, lName, lData, lPer);
    this->gridLayLastRow++;
}

void MainWindow::addHexSequence(const QString &seqName, const QString &seqDataAscii, const QByteArray &seqData, int seqPeriod, int sendCount, int edit, int seqId)
{
    if(edit)
    {
        struct serialSequenceElem *currentSeq = this->serialseq.findSerialSeq(seqId);

        if(currentSeq->status == 1)
        {
            currentSeq->sender->finishWork();
            currentSeq->status = 0;
            currentSeq->button->setIcon(QIcon("/home/rcetin/workspace/qt_projects/pipo/img/st_seq.png"));
        }
        QString format("<font color=\"%1\"><b>[%2]:</b></font>");

        currentSeq->labelName->setText(format.arg("red", seqName.left(MAX_VISIBLE_SEQ_DATA_LEN)));
        currentSeq->labelName->setToolTip(QString("Seq Name: %1").arg(seqName));

        currentSeq->labelData->setText("[" + seqDataAscii.left(MAX_VISIBLE_SEQ_DATA_LEN) + "]");
        currentSeq->labelData->setToolTip(QString("Data: %1").arg(seqDataAscii));

        if(seqPeriod)
            currentSeq->labelPeriod->setText("[" + QString::number(seqPeriod, 10).left(MAX_VISIBLE_SEQ_PER_LEN) + "]");
        else
            currentSeq->labelPeriod->setText("[N/A]");
        currentSeq->labelPeriod->setToolTip(QString("Period: %1").arg(QString::number(seqPeriod, 10)));

        serialseq.editSeq(currentSeq, seqPeriod, sendCount, seqName, seqData.data(), seqData.size(), seqDataAscii);
        return;
    }

    QPushButton *stBut = new QPushButton(this);
    stBut->setIcon(QIcon("/home/rcetin/workspace/qt_projects/pipo/img/st_seq.png"));
    stBut->setFixedSize(QSize(30, 20));
    stBut->setProperty("butId", this->gridLayLastRow);
    connect(stBut, SIGNAL(clicked(bool)), this, SLOT(on_serialSeqStartButton_clicked()));
    ui->gridLayout->addWidget(stBut, this->gridLayLastRow, 0);

    QPushButton *editBut = new QPushButton(this);
    editBut->setIcon(QIcon("/home/rcetin/workspace/qt_projects/pipo/img/edit_seq_but.png"));
    editBut->setFixedSize(QSize(30, 20));
    editBut->setProperty("butId", this->gridLayLastRow);
    connect(editBut, SIGNAL(clicked(bool)), this, SLOT(on_seq_edit_but_clicked()));
    ui->gridLayout->addWidget(editBut, this->gridLayLastRow, 1);

    QFont mono("Ubuntu Mono", 11, QFont::Normal);
    QString format("<font color=\"%1\"><b>[%2]:</b></font>");

    QLabel *lName = new QLabel(format.arg("red", seqName.left(MAX_VISIBLE_SEQ_DATA_LEN)), this);
    lName->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    lName->setMinimumWidth(40);
    lName->setFont(mono);
    lName->setToolTip(QString("Seq Name: %1").arg(seqName));
    ui->gridLayout->addWidget(lName, this->gridLayLastRow, 2);

    QLabel *lData = new QLabel("[" + seqDataAscii.left(MAX_VISIBLE_SEQ_DATA_LEN) + "]", this);
    lData->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    lData->setMinimumWidth(40);
    lData->setFont(mono);
    lData->setToolTip(QString("Data: %1").arg(seqDataAscii));
    ui->gridLayout->addWidget(lData, this->gridLayLastRow, 3);

    QLabel *lPer;

    if(seqPeriod)
        lPer = new QLabel("[" + QString::number(seqPeriod, 10).left(MAX_VISIBLE_SEQ_PER_LEN) + "]", this);
    else
        lPer = new QLabel("[N/A]", this);

    lPer->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    lPer->setMinimumWidth(40);
    lPer->setFont(mono);
    lPer->setToolTip(QString("Period: %1").arg(QString::number(seqPeriod, 10)));
    ui->gridLayout->addWidget(lPer, this->gridLayLastRow, 4);

    this->serialseq.addSeqToList(this->gridLayLastRow, seqPeriod, sendCount, seqName, seqData.data(), seqData.size(), seqDataAscii, stBut, SERIAL_SEQ_TYPE_HEX, lName, lData, lPer);
    this->gridLayLastRow++;

}

/**
 * @brief MainWindow::on_pushButton_clicked
 *
 * This fucntion is triggered when Serial Sequence
 * add button is clicked.
 */
void MainWindow::on_pushButton_clicked()
{
    newSerialSeq = new addSequence(this);
    // Connect sequence info to main window function
    connect(newSerialSeq, SIGNAL(sendAsciiSeqInfo(const QString &, const QString &, int, int, int, int)), this, SLOT(addAsciiSequence(const QString &, const QString &, int, int, int, int)));
    connect(newSerialSeq, SIGNAL(sendHexSeqInfo(const QString &, const QString &, const QByteArray&, int, int, int, int)), this, SLOT(addHexSequence(const QString &, const QString &, const QByteArray&, int, int, int, int)));
    newSerialSeq->setModal(true);
    newSerialSeq->exec();
}

/**
 * @brief MainWindow::on_serialSeqStartButton_clicked
 *
 * This function will be triggered when Serial sequence
 * start button is clicked.
 */
void MainWindow::on_serialSeqStartButton_clicked()
{
    if(port == NULL)
    {
        QMessageBox::information(this, "Information", "Please, configure new serial port before starting sequence.");
        return;
    }

    qDebug() << "Serial sequence start button is clicked!" ;
    QPushButton* clickedButton = qobject_cast< QPushButton* >( sender() );
    if ( clickedButton )
    {
        if(portConfig.status == CLOSED)
        {          
            int ret = port->open(QIODevice::ReadWrite);
            if(ret == false)
            {
                QSerialPort::SerialPortError a;
                a = port->error();
                QMetaEnum err= QMetaEnum::fromType<QSerialPort::SerialPortError>();

                QMessageBox::critical(this, "Port Open Error", err.valueToKey(int(a)));
                return;
            }

            portConfig.status = OPENED;
            QFont mono("Ubuntu Mono", 11, QFont::Bold);
            ui->pStatus->setText("Status: Open");
            ui->pStatus->setFont(mono);
        }

        QVariant propertyV = sender()->property("butId");
        if (propertyV.isValid()) {
            int seqId = propertyV.toInt();
            qDebug() << "clickedButtonId: " << seqId;
            struct serialSequenceElem *currentSeq = this->serialseq.findSerialSeq(seqId);

            if(currentSeq->status == 1)
            {
                currentSeq->sender->finishWork();
                currentSeq->status = 0;
                clickedButton->setIcon(QIcon("/home/rcetin/workspace/qt_projects/pipo/img/st_seq.png"));
                return;
            }

            if(!currentSeq->period)
            {
                writeToSerialPort((char *) currentSeq->data, currentSeq->dataLen, currentSeq->textData);
                return;
            }
            else
            {
                QThread *testThread = new QThread();
                sequenceSender *sender = new sequenceSender();
                connect(sender, SIGNAL(writeToPort(char *, int, const QString &)), this, SLOT(writeToSerialPort(char *, int, const QString &)));
                connect(sender, SIGNAL(countFinished(int)), this, SLOT(finishCountProcess(int)));
                currentSeq->sender = sender;
                sender->deleteLater();
                sender->doSetup(*testThread, port, currentSeq->data, currentSeq->dataLen, currentSeq->period, currentSeq->sendCount, currentSeq->textData, currentSeq->seqId);
                sender->moveToThread(testThread);
                testThread->start();
            }
            clickedButton->setIcon(QIcon("/home/rcetin/workspace/qt_projects/pipo/img/sequence_stop_but.png"));
            currentSeq->status = 1;

            qDebug() << "curSeq Index: " << currentSeq->seqId << ", data: " << currentSeq->data << "period: " << currentSeq->period;
        }

    }
}

void MainWindow::on_seq_edit_but_clicked()
{
    QPushButton* clickedButton = qobject_cast< QPushButton* >( sender() );
    if(clickedButton)
    {
        QVariant propertyV = sender()->property("butId");
        if (propertyV.isValid()) {
            int seqId = propertyV.toInt();
            qDebug() << "clickedButtonId: " << seqId;
            struct serialSequenceElem *currentSeq = this->serialseq.findSerialSeq(seqId);


            newSerialSeq = new addSequence(this, currentSeq);
            connect(newSerialSeq, SIGNAL(sendAsciiSeqInfo(const QString &, const QString &, int, int, int, int)), this, SLOT(addAsciiSequence(const QString &, const QString &, int, int, int, int)));
            connect(newSerialSeq, SIGNAL(sendHexSeqInfo(const QString &, const QString &, const QByteArray&, int, int, int, int)), this, SLOT(addHexSequence(const QString &, const QString &, const QByteArray&, int, int, int, int)));

            newSerialSeq->setModal(true);
            newSerialSeq->exec();
        }
    }
}

void MainWindow::createNewSerialPort(const QString portName, int baudRate, int dataBits, int stopBits, int parity)
{
    if(port == NULL)
    {
        port = new QSerialPort(this);
        connect(port, &QSerialPort::readyRead, this, &MainWindow::readData);
    }

    port->close();
    if(portConfig.status == OPENED)
        serialseq.stopAllSequences();

    port->setBaudRate(baudRate);
    port->setParity(QSerialPort::Parity(parity));
    port->setDataBits(QSerialPort::DataBits(dataBits));
    port->setStopBits(QSerialPort::StopBits(stopBits));
    port->setPortName(portName);

    //this->port = new serialPort(portName, baudRate, dataBits, stopBits, parity);
    QFont mono("Ubuntu Mono", 11, QFont::Bold);
    ui->pNameLabel->setText("Port: " + port->portName());
    ui->pNameLabel->setFont(mono);

    ui->pRateLabel->setText("Baudrate: " + QString::number(port->baudRate()));
    ui->pRateLabel->setFont(mono);

    QMetaEnum parEn = QMetaEnum::fromType<QSerialPort::Parity>();

    ui->pParLabel->setText(QString("%1 | %2 | %3").arg(QString::number(port->dataBits()), parEn.valueToKey(parity), QString::number(port->stopBits())));
    ui->pParLabel->setFont(mono);

    ui->pStatus->setText("Status: Close");
    ui->pStatus->setFont(mono);

    portConfig.baud = baudRate;
    portConfig.data = dataBits;
    portConfig.name = portName;
    portConfig.stop = stopBits;
    portConfig.parity = parity;
    portConfig.status = CLOSED;
}

/**
 * @brief MainWindow::on_actionStop_triggered
 *
 * This will close the Serialport
 */
void MainWindow::on_actionStop_triggered()
{
    if(port == NULL)
        return;

    if(portConfig.status == CLOSED)
        return;

    serialseq.stopAllSequences();
    port->close();
    portConfig.status = CLOSED;
    QFont mono("Ubuntu Mono", 11, QFont::Bold);
    ui->pStatus->setText("Status: Close");
    ui->pStatus->setFont(mono);
}

void MainWindow::writeToSerialPort(char *sendSeq, int size, const QString &textData)
{
    int ret = port->write(sendSeq, size);
    if(ret < 0)
        qDebug() << "Serial port write error!";

    QScrollBar *sb = ui->textBrowser->verticalScrollBar();

    QString currentTime(QDateTime::currentDateTime().toString("hh:mm:ss:zzz"));
    QString format("<font color=\"%1\"><b>%2 | %3: </b></font>");
    QColor color("green");
    ui->textBrowser->moveCursor(QTextCursor::End);
    ui->textBrowser->insertHtml(format.arg(color.name(), "TX", currentTime) + textData);
    ui->textBrowser->insertPlainText("\n");

    if((saveToFileFlag == 1) && (saveToFilePath != NULL))
    {
        // Save out to file
        QString fileData("%1 | %2: %3");
        *outStream << fileData.arg("TX", currentTime, textData) << endl;
    }
    // To stay not last line of text browser.
    // But bcs of ui->textBrowser->moveCursor(QTextCursor::End) can't.
//    if(sb->value() < (sb->maximum() - TEXT_BROWSER_MOUSE_HOVER_THR))
//        return;

    sb->setValue(sb->maximum());
}

void MainWindow::on_actionAbout_triggered()
{
    QString msg = "Created by rcetin<br><br>https://github.com/rcetin";
    QMessageBox msgBox(this);
    msgBox.about(this, "About Pipo", msg);
}

void MainWindow::readData()
{
    if(port == NULL)
        return;

    if(portConfig.status == CLOSED)
        return;

    QScrollBar *sb = ui->textBrowser->verticalScrollBar();

    QString currentTime(QDateTime::currentDateTime().toString("hh:mm:ss:zzz"));
    QString format("<font color=\"%1\"><b>%2 | %3: </b></font>");
    QColor color("blue");

    const QByteArray data = port->readAll();
    QString dataStr = QString::fromStdString(data.toStdString());
    ui->textBrowser->moveCursor(QTextCursor::End);
    if(browserStatus == ASCII)
    {
        ui->textBrowser->insertHtml(format.arg(color.name(), "RX", currentTime) + dataStr);
        ui->textBrowser->insertPlainText("\n");

        if((saveToFileFlag == 1) && (saveToFilePath != NULL))
        {
            // Save out to file
            QString fileData("%1 | %2: %3");
            *outStream << fileData.arg("RX", currentTime, dataStr) << endl;
        }

    }
    else
    {
        QByteArray hexx = data.toHex();
        hexx = hexx.toUpper();
        QString hexStr = QString::fromStdString(hexx.toStdString());

        QString hexWithSpaceStr;
        int j = 0;
        for(int i = 0; i < hexStr.size(); i++)
        {
            hexWithSpaceStr[j] = hexStr[i];
            j += 1;
            if((i+1) % 2 == 0)
            {
                hexWithSpaceStr[j + 1] = ' ';
                j = j + 1;
            }
        }

        ui->textBrowser->insertHtml(format.arg(color.name(), "RX", currentTime) + hexWithSpaceStr);
        ui->textBrowser->insertPlainText("\n");


        if((saveToFileFlag == 1) && (saveToFilePath != NULL))
        {
            // Save out to file
            QString fileData("%1 | %2: %3");
            *outStream << fileData.arg("RX", currentTime, hexWithSpaceStr) << endl;
        }
    }


    sb->setValue(sb->maximum());
}

void MainWindow::finishCountProcess(int seqId)
{
    struct serialSequenceElem *currentSeq = this->serialseq.findSerialSeq(seqId);

    currentSeq->status = 0;
    currentSeq->button->setIcon(QIcon("/home/rcetin/workspace/qt_projects/pipo/img/st_seq.png"));

}

void MainWindow::on_ascCheck_stateChanged(int arg1)
{
    if(arg1 == Qt::Checked)
    {
        browserStatus = ASCII;
        ui->hexCheck->setCheckState(Qt::Unchecked);
    }
    else
    {
        if(ui->hexCheck->isChecked() == false)
        {
            ui->ascCheck->setCheckState(Qt::Checked);
            browserStatus = ASCII;
        }
    }
}

void MainWindow::on_hexCheck_stateChanged(int arg1)
{
    if(arg1 == Qt::Checked)
    {
        browserStatus = HEX;
        ui->ascCheck->setCheckState(Qt::Unchecked);
    }
    else
    {
        if(ui->ascCheck->isChecked() == false)
        {
            ui->hexCheck->setCheckState(Qt::Checked);
            browserStatus = HEX;
        }
    }
}

void MainWindow::on_ckearTextBrw_clicked()
{
    ui->textBrowser->clear();
}

void MainWindow::on_saveBrwToFile_clicked()
{
    if(ui->saveBrwToFile->isChecked())
        saveToFilePath = QFileDialog::getOpenFileName(this, "Select File to Save", QDir::homePath());

    if((saveToFilePath != NULL) && (saveToFileFlag == 0))
    {
        // First time file is selected. Open file and start recording.
        file = new QFile(saveToFilePath);
        if(file->open(QIODevice::WriteOnly))
            outStream = new QTextStream(file);
        else
        {
            qDebug() << "file can not be opened. File: " << saveToFilePath;
            return;
        }

        saveToFileFlag = 1;
        ui->saveBrwToFile->setChecked(true);

    }
    else
    {
        saveToFileFlag = 0;
        ui->saveBrwToFile->setChecked(false);
        file->close();
        delete file;
        delete outStream;
        file = NULL;
        outStream = NULL;
    }

    qDebug() << "save file path: " << saveToFilePath;
}
