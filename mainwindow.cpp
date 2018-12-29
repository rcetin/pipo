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

    int tempCorner;
    ui->textBrowser->geometry().getCoords(&this->topLeftTxtBrowser_x, &this->topLeftTxtBrowser_y, &tempCorner, &tempCorner);
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
}
