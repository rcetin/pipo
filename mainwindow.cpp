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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->pushButton->setToolTip("Add New Serial Sequence");
    QFont mono("Ubuntu Mono", 11, QFont::Normal);
    ui->textBrowser->setFont(mono);
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
        return;

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
void MainWindow::addSerialSequence(const QString & seqName, const QString &seqData, int seqPeriod)
{
    QPushButton *stBut = new QPushButton(this);
    stBut->setIcon(QIcon("/home/rcetin/workspace/qt_projects/pipo/img/st_seq.png"));
    stBut->setFixedSize(QSize(30, 20));
    stBut->setProperty("butId", this->gridLayLastRow);
    connect(stBut, SIGNAL(clicked(bool)), this, SLOT(on_serialSeqStartButton_clicked()));
    ui->gridLayout->addWidget(stBut, this->gridLayLastRow, 0);

    QFont mono("Ubuntu Mono", 11, QFont::Normal);

    QLabel *lName = new QLabel("Name: [" + seqName + "]", this);
    lName->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    lName->setMinimumWidth(80);
    lName->setFont(mono);
    lName->setToolTip(QString("Seq Name: %1").arg(seqName));
    ui->gridLayout->addWidget(lName, this->gridLayLastRow, 1);

    QLabel *lData = new QLabel("Data: " + seqData, this);
    lData->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    lData->setMinimumWidth(80);
    lData->setFont(mono);
    lData->setToolTip(QString("Data: %1").arg(seqData));
    ui->gridLayout->addWidget(lData, this->gridLayLastRow, 2);

    this->serialseq.addSeqToList(this->gridLayLastRow, seqPeriod, seqName, seqData);
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
    connect(newSerialSeq, SIGNAL(sendSeqInfo(const QString &, const QString &, int)), this, SLOT(addSerialSequence(const QString &, const QString &, int)));
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

        }

        QVariant propertyV = sender()->property("butId");
        if (propertyV.isValid()) {
            int seqId = propertyV.toInt();
            qDebug() << "clickedButtonId: " << seqId;
            struct serialSequenceElem *currentSeq = this->serialseq.findSerialSeq(seqId);

            if(currentSeq->status == 1)
            {
                currentSeq->sender->finishWork();
                qDebug() << "send stop signal to thread";
                currentSeq->status = 0;
                clickedButton->setIcon(QIcon("/home/rcetin/workspace/qt_projects/pipo/img/st_seq.png"));
                return;
            }

            if(!currentSeq->period)
            {
                port->write((char *) currentSeq->data.toLocal8Bit().constData(), currentSeq->data.size());
                return;
            }
            else
            {
                QThread *testThread = new QThread();
                sequenceSender *sender = new sequenceSender();
                connect(sender, SIGNAL(writeToPort(char *, int)), this, SLOT(writeToSerialPort(char *, int)));

                currentSeq->sender = sender;
                sender->deleteLater();
                sender->doSetup(*testThread, port, currentSeq->data, currentSeq->period);
                sender->moveToThread(testThread);
                                testThread->start();
            }
            clickedButton->setIcon(QIcon("/home/rcetin/workspace/qt_projects/pipo/img/sequence_stop_but.png"));
            currentSeq->status = 1;

            qDebug() << "curSeq Index: " << currentSeq->seqId << ", data: " << currentSeq->data << "period: " << currentSeq->period;
        }

    }
}

void MainWindow::createNewSerialPort(const QString portName, int baudRate, int dataBits, int stopBits, int parity)
{
    if(port == NULL)
        port = new QSerialPort();

    port->close();

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

    port->close();
    portConfig.status = CLOSED;
}

void MainWindow::writeToSerialPort(char *sendSeq, int size)
{
    int ret = port->write(sendSeq, size);
    if(ret < 0)
        qDebug() << "Serial port write error!";

    QString dat = sendSeq;

    QString format("<font color=\"%1\"><b>%2</b></font>");
    QColor color("green");
    ui->textBrowser->moveCursor(QTextCursor::End);
    ui->textBrowser->insertHtml(format.arg(color.name(), "TX: ") + dat);
    ui->textBrowser->insertPlainText("\n");

    QScrollBar *sb = ui->textBrowser->verticalScrollBar();
    if(sb->value() < (sb->maximum() - TEXT_BROWSER_MOUSE_HOVER_THR))
        return;
    sb->setValue(sb->maximum());
}
