#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "serialconfig.h"
#include "addsequence.h"
#include <qlistwidget.h>
#include "serialseq.h"
#include <QSerialPort>
#include <QByteArray>

#define TEXT_BROWSER_MARGIN_IN_PX 20
#define TEXT_BROWSER_MOUSE_HOVER_THR 100
#define MAX_VISIBLE_SEQ_NAME_LEN 3
#define MAX_VISIBLE_SEQ_DATA_LEN 4
#define MAX_VISIBLE_SEQ_PER_LEN 4


enum portStatus
{
    OPENED = 0,
    CLOSED = 1,
};

enum textBrowserStatus
{
    ASCII = 0,
    HEX = 1,
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
    void addAsciiSequence(const QString &, const QString &, int, int, int);
    void addHexSequence(const QString &, const QString &, const QByteArray &, int, int, int);
    void createNewSerialPort(const QString portName, int baudRate, int dataBits, int stopBits, int parity);
    void writeToSerialPort(char *, int, const QString &);
    void readData();
private slots:

    void on_actionSerial_Port_triggered();
    void on_actionQuit_triggered();
    void on_actionStart_triggered();
    void on_pushButton_clicked();
    void on_serialSeqStartButton_clicked();
    void on_actionStop_triggered();
    void on_seq_edit_but_clicked();
    void on_actionAbout_triggered();

    void on_ascCheck_stateChanged(int arg1);

    void on_hexCheck_stateChanged(int arg1);

    void on_ckearTextBrw_clicked();

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
    int browserStatus = ASCII;
};

#endif // MAINWINDOW_H
