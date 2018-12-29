#include "serialconfig.h"
#include "ui_serialconfig.h"
#include <QDebug>

serialConfig::serialConfig(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::serialConfig)
{
    ui->setupUi(this);

    // CSS for serialPortConfig GroupBox
    ui->groupBox->setStyleSheet("QGroupBox {  border: 1px solid gray; "
                                "border-radius: 5px;"
                                "background: white;}");

    ui->speed_list->addItem("300");
    ui->speed_list->addItem("1200");
    ui->speed_list->addItem("2400");
    ui->speed_list->addItem("4800");
    ui->speed_list->addItem("9600");
    ui->speed_list->addItem("19200");
    ui->speed_list->addItem("38400");
    ui->speed_list->addItem("57600");
    ui->speed_list->addItem("115200");

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
    qDebug() << "Config is accepted!";
    qDebug() << "speed_list val: " << ui->speed_list->currentText();
    qDebug() << "data_list val: " << ui->data_list->currentText();
    qDebug() << "stop_list val: " << ui->stop_list->currentText();
    qDebug() << "parity_list val: " << ui->parity_list->currentText();
}
