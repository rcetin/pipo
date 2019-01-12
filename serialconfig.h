#ifndef SERIALCONFIG_H
#define SERIALCONFIG_H

#include <QDialog>
#include <QString>

namespace Ui {
class serialConfig;
}

class serialConfig : public QDialog
{
    Q_OBJECT

public:
    explicit serialConfig(QWidget *parent = 0);
    ~serialConfig();

private slots:
    void on_buttonBox_accepted();

signals:
    void sendNewSerialPortInfo(const QString portName, int baudRate, int dataBits, int stopBits, int parity);

private:
    Ui::serialConfig *ui;
};

#endif // SERIALCONFIG_H
