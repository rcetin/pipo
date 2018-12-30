#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QDebug"
#include "startsequence.h"
#include <QGridLayout>
#include <QLabel>
#include <QToolButton>
#include <QPushButton>
#include <QLineEdit>
#include <QScrollArea>
#include <QSizePolicy>
#include <QScrollBar>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(&newSerialSeq, SIGNAL(sendSeqInfo(const QString &, const QString &)), this, SLOT(addSerialSequence(const QString &, const QString &)));

    ui->pushButton->setToolTip("Add New Serial Sequence");
    int tempCorner;
    ui->textBrowser->geometry().getCoords(&this->topLeftTxtBrowser_x, &this->topLeftTxtBrowser_y, &tempCorner, &tempCorner);
    QFont serifFont("Times", 12);
    ui->textBrowser->setFont(serifFont);

    //ui->gridLayout->setVerticalSpacing(5);
    //ui->gridLayout->setContentsMargins(20, 20, 20, 20);

//    QLabel *seqLabel = new QLabel("Serial Sequences");
//    ui->gridLayout->addWidget(seqLabel, this->gridLayLastRow, 0);

//    QToolButton *addSeqBut = new QToolButton();
//    ui->gridLayout->addWidget(addSeqBut, this->gridLayLastRow, 2);
//    addSeqBut->setIcon(QIcon("/home/rcetin/workspace/qt_projects/pipo/img/add_new_seq.ico"));

//    connect(addSeqBut, SIGNAL (released()),this, SLOT (add_serial_seq_button_handler()));


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
   QMainWindow::resizeEvent(event);

   ui->textBrowser->setFixedHeight(this->size().height() - this->topLeftTxtBrowser_y - (TEXT_BROWSER_MARGIN_IN_PX + 2) - 59);
   ui->textBrowser->setFixedWidth(this->size().width() - this->topLeftTxtBrowser_x - TEXT_BROWSER_MARGIN_IN_PX);

   ui->scrollArea->setFixedHeight(this->size().height() - 140);
}

void MainWindow::on_actionSerial_Port_triggered()
{
    serialConf = new serialConfig(this);
    serialConf->setModal(true);
    serialConf->exec();
}

void MainWindow::on_actionQuit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_actionStart_triggered()
{
    ui->textBrowser->insertPlainText("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nullam fringilla justo ipsum, in finibus est dignissim ut. Pellentesque at est a ipsum luctus finibus vitae at dolor. Maecenas lobortis leo nec urna sollicitudin, sed pulvinar ex dictum. Morbi metus sapien, cursus sed tincidunt non, molestie vel magna. Donec efficitur dui odio, sollicitudin pellentesque nisl rhoncus nec. Morbi in orci ut ligula viverra iaculis ut in velit. Nullam tempus nisi non tincidunt mattis. Curabitur urna tortor, facilisis ut accumsan fermentum, facilisis ut ante. Sed condimentum egestas metus, commodo semper massa. Suspendisse laoreet erat ut ante pellentesque, sit amet pellentesque diam ultricies. Nam sed nisi nibh. Aenean ornare nunc condimentum molestie vestibulum. Integer et blandit nulla. Etiam tempus nulla ipsum, nec pretium odio pharetra interdum. \n");

    // Set scroll bar to bottom
    QScrollBar *sb = ui->textBrowser->verticalScrollBar();
    sb->setValue(sb->maximum());
}

void MainWindow::addSerialSequence(const QString & seqName, const QString &seqData)
{
    QPushButton *stBut = new QPushButton();
    stBut->setIcon(QIcon("/home/rcetin/workspace/qt_projects/pipo/img/st_seq.png"));
    stBut->setFixedSize(QSize(30, 20));
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

    qDebug() << "Parent of grid: " << ui->gridLayout->parentWidget();
    this->gridLayLastRow++;

    qDebug() << "seqName: " << seqName << "seqData: " << seqData;
}

void MainWindow::add_serial_seq_button_handler()
{
    newSerialSeq.setModal(true);
    newSerialSeq.exec();
}

void MainWindow::on_pushButton_clicked()
{
    newSerialSeq.setModal(true);
    newSerialSeq.exec();
}
