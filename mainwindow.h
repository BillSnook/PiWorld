#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "common.h"

class commPi;
class hardware;

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
    Ui::MainWindow  *ui;
    commPi          *piPtr;
    hardware        *hw = nullptr;

    bool connected;
    bool commMode;
    bool motorMode;

    int slider[4];
    speed_array speed[SPEED_ARRAY];

    void menuSetup();
    void buttonSetup();
    void connectSetup();
    void sliderSetup();

private slots:
    void slotCommMenu();
    void slotMotorMenu();

    void slotConnectClicked(bool checked);

    void sliderChanged1(int newValue);
    void sliderChanged2(int newValue);
    void sliderChanged3(int newValue);
    void sliderChanged4(int newValue);

    void b1l(bool checked);
    void b2l(bool checked);
    void b3l(bool checked);
    void b4l(bool checked);

    void b1r(bool checked);
    void b2r(bool checked);
    void b3r(bool checked);
    void b4r(bool checked);

    void t1(bool checked);
    void t2(bool checked);
    void t3(bool checked);
    void t4(bool checked);


};

#endif // MAINWINDOW_H
