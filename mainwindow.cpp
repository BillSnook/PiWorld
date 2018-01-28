#include "mainwindow.h"
#include "ui_mainwindow.h"


//#define USE_MOTOR


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);
    setFixedSize(400, 400);

    menuSetup();

    ui->connectBox->hide();
    ui->talkBox->hide();

    buttonSetup();
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

    connect(ui->connectButton, SIGNAL (clicked(bool)), this, SLOT (slotConnectClicked(bool)));
    connect(ui->okButton, SIGNAL (clicked(bool)), this, SLOT (slotOKClicked(bool)));
    connect(ui->helloButton, SIGNAL (clicked(bool)), this, SLOT (slotHelloClicked(bool)));
    connect(ui->blinkButton, SIGNAL (clicked(bool)), this, SLOT (slotBlinkClicked(bool)));
    connect(ui->stopButton, SIGNAL (clicked(bool)), this, SLOT (slotStopClicked(bool)));
}

void MainWindow::connectSetup() {

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

void MainWindow::motorSetup() {

    commMode = false;
    motorMode = true;

    ui->talkAddress->setText( "Test Setup" ); // Initial setting
#ifdef USE_MOTOR
    motor = new Motor();
    motor->setupForMotor();
#endif  // USE_MOTOR
    ui->connectButton->setText("Setup IO");
    ui->talkBox->hide();

    ui->horizontalSlider->setRange( 0, 100 );
    connect(ui->horizontalSlider, SIGNAL (valueChanged(int)), this, SLOT (sliderChanged(int)));
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
        if (motorMode) {                        // Setup for motor test
            if (checked) {
                ui->connectButton->setText("Setting Up");
#ifdef USE_MOTOR
                connected = motor->activated;
#else
                connected = true;
#endif  // USE_MOTOR
                if ( connected ) {
                    ui->connectButton->setText("IO Ready");
                    ui->talkBox->show();
                } else {
                    ui->connectButton->setText("Not Ready");
                    ui->talkBox->hide();
                }
                ui->connectButton->setChecked(connected);
            } else {
#ifdef USE_MOTOR
                connected = motor->resetForMotor();
#endif  // USE_MOTOR
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
        ui->horizontalSlider->setValue( 0 );
#ifdef USE_MOTOR
        if ( checked ) {
            motor->onPin(L1);
        } else {
            motor->offPin(L1);
        }
#endif  // USE_MOTOR
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
        ui->horizontalSlider->setValue( 0 );
#ifdef USE_MOTOR
        if ( checked ) {
            motor->onPin(L2);
        } else {
            motor->offPin(L2);
        }
#endif  // USE_MOTOR
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
        ui->horizontalSlider->setValue( 0 );
#ifdef USE_MOTOR
        if ( checked ) {
            motor->onPin(L3);
        } else {
            motor->offPin(L3);
        }
#endif  // USE_MOTOR
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
        ui->horizontalSlider->setValue( 0 );
#ifdef USE_MOTOR
        if ( checked ) {
            motor->onPin(L4);
        } else {
            motor->offPin(L4);
        }
#endif  // USE_MOTOR
    }
}
