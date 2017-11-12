#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "commpi.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    commPi *piPtr;

    bool connected;
    void buttonSetup();

private slots:
    void slotConnectClicked(bool checked);

    void slotOKClicked(bool checked);
    void slotHelloClicked(bool checked);
    void slotBlinkClicked(bool checked);
    void slotStopClicked(bool checked);

};

#endif // MAINWINDOW_H
