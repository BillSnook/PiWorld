#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "commpi.h"
#include "motor.h"


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

    connect(ui->okButton_2, SIGNAL (clicked(bool)), this, SLOT (m1rv(bool)));
    connect(ui->helloButton_2, SIGNAL (clicked(bool)), this, SLOT (m2rv(bool)));
    connect(ui->blinkButton_2, SIGNAL (clicked(bool)), this, SLOT (m3rv(bool)));
    connect(ui->stopButton_2, SIGNAL (clicked(bool)), this, SLOT (m4rv(bool)));
}

void MainWindow::connectSetup() {

    ui->talkAddress->setText( "develop31" ); // Initial setting
    if ( nullptr == piPtr ) {
        piPtr = new commPi();
    }
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
    connect(ui->m1Slider, SIGNAL (valueChanged(int)), this, SLOT (sliderChanged1(int)));

    ui->m2Slider->setRange( 0, 100 );
    ui->m2Slider->show();
    connect(ui->m2Slider, SIGNAL (valueChanged(int)), this, SLOT (sliderChanged2(int)));

    ui->m3Slider->setRange( 0, 100 );
    ui->m3Slider->show();
    connect(ui->m3Slider, SIGNAL (valueChanged(int)), this, SLOT (sliderChanged3(int)));

    ui->m4Slider->setRange( 0, 100 );
    ui->m4Slider->show();
    connect(ui->m4Slider, SIGNAL (valueChanged(int)), this, SLOT (sliderChanged4(int)));
}

void MainWindow::sliderChanged1(int newValue) {

    ui->messageTextLine->setText( QString::number(newValue) );
    speed[0] = newValue;
}

void MainWindow::sliderChanged2(int newValue) {

    ui->messageTextLine->setText( QString::number(newValue) );
    speed[1] = newValue;
}

void MainWindow::sliderChanged3(int newValue) {

    ui->messageTextLine->setText( QString::number(newValue) );
    speed[2] = newValue;
}

void MainWindow::sliderChanged4(int newValue) {

    ui->messageTextLine->setText( QString::number(newValue) );
    speed[3] = newValue;
}


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

    ui->talkAddress->setText( "" );

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

    for ( int i = 0; i < 4; i++ ) {
        speed[i] = 0;
    }

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
                    fprintf(stderr,"connected\n");
                    ui->connectButton->setText("IO Ready");
                    ui->talkBox->show();
                } else {
                    fprintf(stderr,"not connected\n");
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
        if ( checked ) {    // Turn on
            motor->setMtrDirSpd( 1, 1, speed[0] );  // Motor 1, forward, speed
        } else {            // Turn off
            motor->setMtrSpd( 1, 0 );               // Motor 1, stop
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
        if ( checked ) {    // Turn on
            motor->setMtrDirSpd( 2, 1, speed[1] );  // Motor 2, forward, speed
        } else {            // Turn off
            motor->setMtrSpd( 2, 0 );               // Motor 2, stop
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
        if ( checked ) {    // Turn on
            motor->setMtrDirSpd( 1, 1, speed[0] );  // Motor 1, forward, speed
            motor->setMtrDirSpd( 2, 1, speed[1] );  // Motor 2, forward, speed
        } else {            // Turn off
            motor->setMtrSpd( 1, 0 );               // Motor 1, stop
            motor->setMtrSpd( 2, 0 );               // Motor 2, stop
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

void MainWindow::m1rv(bool checked) {

    if (motorMode) {
        if ( checked ) {
            motor->setMtrDirSpd( 1, 0, speed[0] );  // Motor 1, reverse, speed
        } else {
            motor->setMtrSpd( 1, 0 );               // Motor 1, stop
        }
    }
}

void MainWindow::m2rv(bool checked) {

    if (motorMode) {
        if ( checked ) {
            motor->setMtrDirSpd( 2, 0, speed[1] );  // Motor 2, reverse, speed
        } else {
            motor->setMtrSpd( 2, 0 );               // Motor 2, reverse, speed
        }
    }
}

void MainWindow::m3rv(bool checked) {

    if (motorMode) {
        if ( checked ) {    // Turn on
            motor->setMtrDirSpd( 1, 0, speed[0] );  // Motor 1, reverses, speed
            motor->setMtrDirSpd( 2, 0, speed[1] );  // Motor 2, reverse, speed
        } else {            // Turn off
            motor->setMtrSpd( 1, 0 );               // Motor 1, stop
            motor->setMtrSpd( 2, 0 );               // Motor 2, stop
        }
    }
}

void MainWindow::m4rv(bool checked) {

    if (motorMode) {
        motor->blinkLED();
    }
}
