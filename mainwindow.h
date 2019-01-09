#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "serialconfig.h"
#include "addsequence.h"
#include <qlistwidget.h>
#include "serialseq.h"
#include "serialport.h"

#define TEXT_BROWSER_MARGIN_IN_PX 20

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
    void addSerialSequence(const QString &, const QString &);
    void createNewSerialPort(QString portName, int baudRate, int dataBits, int stopBits, QString parity);
private slots:

    void on_actionSerial_Port_triggered();
    void on_actionQuit_triggered();
    void on_actionStart_triggered();
    void on_pushButton_clicked();
    void on_serialSeqStartButton_clicked();

private:
    Ui::MainWindow *ui;
    serialConfig *serialConf;   // This will add new serial port (Dialog)
    addSequence newSerialSeq;   // This will add new sequence (Dialog)
    serialSeq serialseq;    // This will keep all serial sequences in a list
    serialPort *port = NULL;    // This will keep serial port

    int topLeftTxtBrowser_x;
    int topLeftTxtBrowser_y;
    int gridLayLastRow = 0;

    void resizeEvent(QResizeEvent* event);
};

#endif // MAINWINDOW_H
