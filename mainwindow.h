#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "commpi.h"
#include "motor.h"


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
    Motor *motor;

    bool connected;
    bool commMode;
    bool motorMode;

    void menuSetup();
    void buttonSetup();
    void connectSetup();
    void sliderSetup();

private slots:
    void slotCommMenu();
    void slotMotorMenu();

    void slotConnectClicked(bool checked);

    void slotOKClicked(bool checked);
    void slotHelloClicked(bool checked);
    void slotBlinkClicked(bool checked);
    void slotStopClicked(bool checked);

    void sliderChanged(int newValue);

};

#endif // MAINWINDOW_H
