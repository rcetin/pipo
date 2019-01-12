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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->pushButton->setToolTip("Add New Serial Sequence");
    int tempCorner;
    ui->textBrowser->geometry().getCoords(&this->topLeftTxtBrowser_x, &this->topLeftTxtBrowser_y, &tempCorner, &tempCorner);
    QFont serifFont("Times", 12);
    ui->textBrowser->setFont(serifFont);
}

MainWindow::~MainWindow()
{
    delete ui;
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

    int ret = this->port->open(QIODevice::ReadWrite);
    if(ret == false)
        qDebug() << "Error in opening serial port. Err: " << this->port->error();

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
    QPushButton *stBut = new QPushButton();
    stBut->setIcon(QIcon("/home/rcetin/workspace/qt_projects/pipo/img/st_seq.png"));
    stBut->setFixedSize(QSize(30, 20));
    stBut->setProperty("butId", this->gridLayLastRow);
    connect(stBut, SIGNAL(clicked(bool)), this, SLOT(on_serialSeqStartButton_clicked()));
    ui->gridLayout->addWidget(stBut, this->gridLayLastRow, 0);

    QLineEdit *sqName = new QLineEdit;
    sqName->setText(seqName);
    QString n = QString("Seq Name: %1").arg(seqName);
    sqName->setToolTip(n);

    sqName->setReadOnly(1);
    ui->gridLayout->addWidget(sqName, this->gridLayLastRow, 1);

    QLineEdit *sqData = new QLineEdit;
    sqData->setText(seqData);
    QString str = QString("Data: %1").arg(seqData);
    sqData->setToolTip(str);
    sqData->setReadOnly(1);
    ui->gridLayout->addWidget(sqData, this->gridLayLastRow, 2);

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
        QVariant propertyV = sender()->property("butId");
        if (propertyV.isValid()) {
            int seqId = propertyV.toInt();
            qDebug() << "clickedButtonId: " << seqId;
            struct serialSequenceElem *currentSeq = this->serialseq.findSerialSeq(seqId);
            if(!currentSeq->period)
                ui->textBrowser->insertPlainText(currentSeq->data);
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

    port->close();
    port = NULL;
}
