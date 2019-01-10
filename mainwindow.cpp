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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(&newSerialSeq, SIGNAL(sendSeqInfo(const QString &, const QString &)), this, SLOT(addSerialSequence(const QString &, const QString &)));
    //connect(port, SIGNAL(sendSeqInfo(const QString &, const QString &)), this, SLOT(addSerialSequence(const QString &, const QString &)));

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
    ui->textBrowser->insertPlainText("Lorem ipsum dolor sit amet, consectetur adipiscing elit. \n");

    // Set scroll bar to bottom
    QScrollBar *sb = ui->textBrowser->verticalScrollBar();
    sb->setValue(sb->maximum());
}

/**
 * @brief MainWindow::addSerialSequence
 * @param seqName:  Name of Serial Sequence
 * @param seqData:  Data of Serial Sequence
 *
 * @note This function is used to add serial sequence to memory.
 */
void MainWindow::addSerialSequence(const QString & seqName, const QString &seqData)
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

    this->serialseq.addSeqToList(this->gridLayLastRow, seqName, seqData);
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
    newSerialSeq.setModal(true);
    newSerialSeq.exec();
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

            qDebug() << "curSeq Index: " << currentSeq->seqId << ", data: " << currentSeq->data;
        }

    }
}

void MainWindow::createNewSerialPort(QString portName, int baudRate, int dataBits, int stopBits, QString parity)
{
    if(this->port != NULL)
    {
        qDebug() << "A serial port is already created!";
        return;
    }

    this->port = new serialPort(portName, baudRate, dataBits, stopBits, parity);
}
