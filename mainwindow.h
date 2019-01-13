#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "serialconfig.h"
#include "addsequence.h"
#include <qlistwidget.h>
#include "serialseq.h"
#include <QSerialPort>

#define TEXT_BROWSER_MARGIN_IN_PX 20

enum portStatus
{
    OPENED = 0,
    CLOSED = 1,
};

struct serialportConf
{
    QString name;
    int parity;
    int baud;
    int data;
    int stop;
    int flowControl;
    int status;
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void addSerialSequence(const QString &, const QString &, int);
    void createNewSerialPort(const QString portName, int baudRate, int dataBits, int stopBits, int parity);
private slots:

    void on_actionSerial_Port_triggered();
    void on_actionQuit_triggered();
    void on_actionStart_triggered();
    void on_pushButton_clicked();
    void on_serialSeqStartButton_clicked();

    void on_actionStop_triggered();

private:
    Ui::MainWindow *ui;
    serialConfig *serialConf = NULL;   // This will add new serial port (Dialog)
    addSequence *newSerialSeq = NULL;   // This will add new sequence (Dialog)
    serialSeq serialseq;    // This will keep all serial sequences in a list
    QSerialPort *port = NULL;    // This will keep serial port

    int topLeftTxtBrowser_x;
    int topLeftTxtBrowser_y;
    int gridLayLastRow = 0;
    struct serialportConf portConfig;
};

#endif // MAINWINDOW_H
