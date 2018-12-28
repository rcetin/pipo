#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QDebug"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    qDebug() << "he: " << this->size().height();
    qDebug() << "wid: " << this->size().width();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
   QMainWindow::resizeEvent(event);

   ui->textBrowser->setFixedHeight(this->size().height() - 100);
   ui->textBrowser->setFixedWidth(this->size().width() - 200);
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
    ui->textBrowser->insertPlainText("Test browser\n");
}
