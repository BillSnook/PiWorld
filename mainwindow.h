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

    int speed[4];

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

    void sliderChanged1(int newValue);
    void sliderChanged2(int newValue);
    void sliderChanged3(int newValue);
    void sliderChanged4(int newValue);

    void m1rv(bool checked);
    void m2rv(bool checked);
    void m3rv(bool checked);
    void m4rv(bool checked);


};

#endif // MAINWINDOW_H
