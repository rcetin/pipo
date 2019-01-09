#ifndef SERIALCONFIG_H
#define SERIALCONFIG_H

#include <QDialog>

namespace Ui {
class serialConfig;
}

class serialConfig : public QDialog
{
    Q_OBJECT

public:
    explicit serialConfig(QWidget *parent = 0);
    ~serialConfig();

signals:
    void sendNewSerialPortInfo(QString portName, int baudRate, int dataBits, int stopBits, QString parity);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::serialConfig *ui;
};

#endif // SERIALCONFIG_H
