#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);
    setFixedSize(600, 500);

    menuSetup();

    ui->connectBox->hide();
    ui->talkBox->hide();

    buttonSetup();

    slotMotorMenu();    // Or slotCommMenu(); // if we want to start with that as default
}

void MainWindow::menuSetup() {

    commMode = false;
    motorMode = false;
    connect(ui->CommTest, SIGNAL (triggered()), this, SLOT (slotCommMenu()));
    connect(ui->MotorTest, SIGNAL (triggered()), this, SLOT (slotMotorMenu()));
}

void MainWindow::buttonSetup() {

    ui->connectButton->setCheckable(true);
    ui->okButton->setCheckable(true);
    ui->helloButton->setCheckable(true);
    ui->blinkButton->setCheckable(true);
    ui->stopButton->setCheckable(true);
    ui->okButton_2->setCheckable(true);
    ui->helloButton_2->setCheckable(true);
    ui->blinkButton_2->setCheckable(true);
    ui->stopButton_2->setCheckable(true);

    connect(ui->connectButton, SIGNAL (clicked(bool)), this, SLOT (slotConnectClicked(bool)));
    connect(ui->okButton, SIGNAL (clicked(bool)), this, SLOT (slotOKClicked(bool)));
    connect(ui->helloButton, SIGNAL (clicked(bool)), this, SLOT (slotHelloClicked(bool)));
    connect(ui->blinkButton, SIGNAL (clicked(bool)), this, SLOT (slotBlinkClicked(bool)));
    connect(ui->stopButton, SIGNAL (clicked(bool)), this, SLOT (slotStopClicked(bool)));

    connect(ui->okButton_2, SIGNAL (clicked(bool)), this, SLOT (slotOKClicked(bool)));
    connect(ui->helloButton_2, SIGNAL (clicked(bool)), this, SLOT (slotHelloClicked(bool)));
    connect(ui->blinkButton_2, SIGNAL (clicked(bool)), this, SLOT (slotBlinkClicked(bool)));
    connect(ui->stopButton_2, SIGNAL (clicked(bool)), this, SLOT (slotStopClicked(bool)));
}

void MainWindow::connectSetup() {

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

void MainWindow::sliderSetup() {

    ui->m1Slider->setRange( 0, 100 );
    ui->m1Slider->show();
    connect(ui->m1Slider, SIGNAL (valueChanged(int)), this, SLOT (sliderChanged(int)));

    ui->m2Slider->setRange( 0, 100 );
    ui->m2Slider->show();
    connect(ui->m2Slider, SIGNAL (valueChanged(int)), this, SLOT (sliderChanged(int)));

    ui->m3Slider->setRange( 0, 100 );
    ui->m3Slider->show();
    connect(ui->m3Slider, SIGNAL (valueChanged(int)), this, SLOT (sliderChanged(int)));

    ui->m4Slider->setRange( 0, 100 );
    ui->m4Slider->show();
    connect(ui->m4Slider, SIGNAL (valueChanged(int)), this, SLOT (sliderChanged(int)));
}

void MainWindow::sliderChanged(int newValue) {

    ui->messageTextLine->setText( QString::number(newValue) );
}

//ui->messageTextLine->setText("ok");
//ui->responseDisplay->setPlainText(resp);


MainWindow::~MainWindow() {
    delete ui;
}

//MARK: Menu actions

void MainWindow::slotCommMenu() {

    commMode = true;
    motorMode = false;

    connectSetup();

    ui->connectBox->show();

    ui->okButton->setText("OK");
    ui->helloButton->setText("Hello");
    ui->blinkButton->setText("Blink");
    ui->stopButton->setText("Stop");
    ui->okButton_2->hide();
    ui->helloButton_2->hide();
    ui->blinkButton_2->hide();
    ui->stopButton_2->hide();

    ui->m1Slider->hide();
    ui->m2Slider->hide();
    ui->m3Slider->hide();
    ui->m4Slider->hide();
}

void MainWindow::slotMotorMenu() {

    commMode = false;
    motorMode = true;

    ui->connectButton->setText("Setup IO");
    ui->connectBox->show();

    ui->okButton->setText("M1 Forward");
    ui->helloButton->setText("M2 Forward");
    ui->blinkButton->setText("M3 Forward");
    ui->stopButton->setText("M4 Forward");
    ui->okButton_2->setText("M1 Reverse");
    ui->helloButton_2->setText("M2 Reverse");
    ui->blinkButton_2->setText("M3 Reverse");
    ui->stopButton_2->setText("M4 Reverse");

    ui->okButton_2->show();
    ui->helloButton_2->show();
    ui->blinkButton_2->show();
    ui->stopButton_2->show();

    sliderSetup();
}

// MARK: Button actions

void MainWindow::slotConnectClicked(bool checked) {

    if (commMode ) {                            // Connect to enable communication test
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
            connected = true; // piPtr->connectTo( tAddr );
            free( tAddr );
            if ( connected ) {
                ui->connectButton->setText("Disconnect");
                ui->talkBox->show();
            } else {
                ui->connectButton->setText("Connect");
                ui->talkBox->hide();
            }
            ui->connectButton->setChecked(connected);
        } else {
            connected = piPtr->detachFrom();
            ui->connectButton->setText("Connect");
            ui->talkBox->hide();
        }
    } else {
        if (motorMode) {                        // Setup for motor test
            if (checked) {
                if ( nullptr == motor ) {
                    motor = new Motor();
                }
                connected = motor->setupForMotor();
                ui->connectButton->setText("Setting Up");
                ui->talkAddress->setText( "Test Setup" ); // ?
                if ( connected ) {
                    ui->connectButton->setText("IO Ready");
                    ui->talkBox->show();
                } else {
                    ui->connectButton->setText("Not Ready");
                    ui->talkBox->hide();
                    ui->connectButton->setChecked(false);
                }
            } else {
                ui->talkAddress->setText( "" );
                connected = motor->resetForMotor();
                ui->connectButton->setText("Setup IO");
                ui->talkBox->hide();
            }
        } else {
            ui->talkAddress->setText( "In slotConnectClicked but no mode is set" );
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
        ui->m1Slider->setValue( 0 );
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
        ui->m2Slider->setValue( 0 );
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
        ui->m3Slider->setValue( 0 );
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
        ui->m4Slider->setValue( 0 );
        if ( checked ) {
            motor->onPin(L4);
        } else {
            motor->offPin(L4);
        }
    }
}
