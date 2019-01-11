#include "serialconfig.h"
#include "ui_serialconfig.h"
#include <QDebug>
#include <QtSerialPort/QSerialPortInfo>
#include <QList>
#include <QMessageBox>

serialConfig::serialConfig(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::serialConfig)
{
    ui->setupUi(this);

    // CSS for serialPortConfig GroupBox
    //ui->groupBox->setStyleSheet("QGroupBox {  border: 1px solid gray; "
     //                           "border-radius: 5px;"
     //                           "background: white;}");

    QList<qint32> listBaudRates;
    listBaudRates = QSerialPortInfo::standardBaudRates();
    if(listBaudRates.size())
    {
        for(int i = 0; i < listBaudRates.size(); i++)
            ui->speed_list->addItem(QString::number(listBaudRates[i]));
    }

    QList<QSerialPortInfo> listPorts;
    listPorts = QSerialPortInfo::availablePorts();
    if(listPorts.size())
    {
        for(int i = 0; i < listPorts.size(); i++)
            ui->port_name->addItem(listPorts[i].portName());
    }

    ui->data_list->addItem("5");
    ui->data_list->addItem("6");
    ui->data_list->addItem("7");
    ui->data_list->addItem("8");

    ui->stop_list->addItem("1");
    ui->stop_list->addItem("2");

    ui->parity_list->addItem("None");
    ui->parity_list->addItem("Mark");
    ui->parity_list->addItem("Even");
    ui->parity_list->addItem("Odd");
    ui->parity_list->addItem("Space");

}

serialConfig::~serialConfig()
{
    delete ui;
}

void serialConfig::on_buttonBox_accepted()
{
    if(ui->port_name->currentText() == NULL)
    {
        QMessageBox::critical(this, "Serial Port Config Error", "Serial Port Name is empty!");
        return;
    }

    //emit sendNewSerialPortInfo()
    qDebug() << "Config is accepted!";
    qDebug() << "speed_list val: " << ui->speed_list->currentText();
    qDebug() << "data_list val: " << ui->data_list->currentText();
    qDebug() << "stop_list val: " << ui->stop_list->currentText();
    qDebug() << "parity_list val: " << ui->parity_list->currentText();
}
