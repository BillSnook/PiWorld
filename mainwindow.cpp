#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(400, 400);

    menuSetup();

    ui->connectBox->hide();
    ui->talkBox->hide();

    buttonSetup();
}

void MainWindow::menuSetup()
{

    commMode = false;
    motorMode = false;
    connect(ui->CommTest, SIGNAL (triggered()), this, SLOT (slotCommMenu()));
    connect(ui->MotorTest, SIGNAL (triggered()), this, SLOT (slotMotorMenu()));
}

void MainWindow::buttonSetup()
{

    ui->connectButton->setCheckable(true);
    ui->okButton->setCheckable(true);
    ui->helloButton->setCheckable(true);
    ui->blinkButton->setCheckable(true);
    ui->stopButton->setCheckable(true);

    connect(ui->connectButton, SIGNAL (clicked(bool)), this, SLOT (slotConnectClicked(bool)));
    connect(ui->okButton, SIGNAL (clicked(bool)), this, SLOT (slotOKClicked(bool)));
    connect(ui->helloButton, SIGNAL (clicked(bool)), this, SLOT (slotHelloClicked(bool)));
    connect(ui->blinkButton, SIGNAL (clicked(bool)), this, SLOT (slotBlinkClicked(bool)));
    connect(ui->stopButton, SIGNAL (clicked(bool)), this, SLOT (slotStopClicked(bool)));
}

void MainWindow::connectSetup()
{

    commMode = true;
    motorMode = false;

    ui->talkAddress->setText( "develop31" ); // Initial setting
    piPtr = new commPi();
    connected = piPtr->getCommStateConnected();
    if (connected) {
        ui->connectButton->setText("Disconnect");
        ui->talkBox->show();
    } else {
        ui->connectButton->setText("Connect");
        ui->talkBox->hide();
    }
    ui->connectButton->setChecked(connected);
}

void MainWindow::motorSetup()
{

    commMode = false;
    motorMode = true;

    ui->talkAddress->setText( "Test Setup" ); // Initial setting
    motor = new Motor();
    motor->setupForMotor();
    ui->connectButton->setText("Setup IO");
    ui->talkBox->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotCommMenu() {

    buttonSetup();
    connectSetup();

    ui->connectBox->show();

    ui->okButton->setText("OK");
    ui->helloButton->setText("Hello");
    ui->blinkButton->setText("Blink");
    ui->stopButton->setText("Stop");

}

void MainWindow::slotMotorMenu() {

    motorSetup();

    ui->connectButton->setText("Setup IO");
    ui->connectBox->show();

    ui->okButton->setText("M1");
    ui->helloButton->setText("M2");
    ui->blinkButton->setText("M3");
    ui->stopButton->setText("M4");

}

void MainWindow::slotConnectClicked(bool checked) {

    if (commMode ) {
        if (checked) {
            ui->connectButton->setText("Connecting");
            QString targetString = ui->talkAddress->text();
            const char *targetAddr = targetString.toUtf8().constData();
            size_t len = targetString.length();
            char *tAddr = (char *)malloc( len + 7 );
            memcpy( tAddr, targetAddr, len );
            tAddr[len] = 0;
            strcat( tAddr, ".local" );
            fprintf(stderr,"Target address: %s\n", tAddr);
            connected = piPtr->connectTo( tAddr );
            free( tAddr );
            if ( connected ) {
                ui->connectButton->setText("Disconnect");
                ui->talkBox->show();
            } else {
                ui->connectButton->setText("Connect");
            }
            ui->connectButton->setChecked(!connected);
        } else {
            connected = piPtr->detachFrom();
            ui->connectButton->setText("Connect");
            ui->talkBox->hide();
        }
    } else {
        if (motorMode) {
            if (checked) {
                ui->connectButton->setText("Setting Up");
                connected = motor->activated;
                if ( connected ) {
                    ui->connectButton->setText("IO Ready");
                    ui->talkBox->show();
                } else {
                    ui->connectButton->setText("Not Ready");
                    ui->talkBox->hide();
                }
                ui->connectButton->setChecked(connected);
            } else {
                connected = motor->resetForMotor();
                ui->connectButton->setText("Setup IO");
                ui->talkBox->hide();
            }
        }
    }
}

void MainWindow::slotOKClicked(bool checked) {

    if (commMode ) {
        ui->okButton->setChecked(!checked);
        ui->messageTextLine->setText("ok");
        char *resp = piPtr->sendMessage( "ok\n" );
        ui->responseDisplay->setPlainText(resp);
    }
    if (motorMode) {
        if ( checked ) {
            motor->onPin(L1);
        } else {
            motor->offPin(L1);
        }
    }
}

void MainWindow::slotHelloClicked(bool checked) {

    if (commMode ) {
        ui->helloButton->setChecked(!checked);
        ui->messageTextLine->setText("hello");
        char *resp = piPtr->sendMessage( "hello\n" );
        ui->responseDisplay->setPlainText(resp);
    }
    if (motorMode) {
        if ( checked ) {
            motor->onPin(L2);
        } else {
            motor->offPin(L2);
        }
    }
}

void MainWindow::slotBlinkClicked(bool checked) {

    if (commMode ) {
        ui->blinkButton->setChecked(!checked);
        ui->messageTextLine->setText("blink");
        char *resp = piPtr->sendMessage( "blink\n" );
        ui->responseDisplay->setPlainText(resp);
    }
    if (motorMode) {
        if ( checked ) {
            motor->onPin(L3);
        } else {
            motor->offPin(L3);
        }
    }
}

void MainWindow::slotStopClicked(bool checked) {

    if (commMode ) {
        ui->stopButton->setChecked(!checked);
        ui->messageTextLine->setText("blinkstop");
        char *resp = piPtr->sendMessage( "blinkstop\n" );
        ui->responseDisplay->setPlainText(resp);
    }
    if (motorMode) {
        if ( checked ) {
            motor->onPin(L4);
        } else {
            motor->offPin(L4);
        }
    }
}
