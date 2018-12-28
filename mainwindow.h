#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "serialconfig.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_actionSerial_Port_triggered();

    void on_actionQuit_triggered();

    void on_actionStart_triggered();

private:
    Ui::MainWindow *ui;
    serialConfig *serialConf;

    void resizeEvent(QResizeEvent* event);
};

#endif // MAINWINDOW_H
