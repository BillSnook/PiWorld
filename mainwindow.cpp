#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "commpi.h"
#include "hardware.h"


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

    // Eventually we will get these from a file
    for ( int i = 0; i < SPEED_ARRAY; i++ ) {
        speed[i].left = 0;
        speed[i].right = 0;
    }
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
    ui->okButton_2->setCheckable(false);
    ui->helloButton_2->setCheckable(false);
    ui->blinkButton_2->setCheckable(false);
    ui->stopButton_2->setCheckable(false);
    ui->t0->setCheckable(false);
    ui->t1->setCheckable(false);
    ui->t2->setCheckable(false);
    ui->t3->setCheckable(false);

    connect(ui->connectButton, SIGNAL (clicked(bool)), this, SLOT (slotConnectClicked(bool)));
    connect(ui->okButton, SIGNAL (clicked(bool)), this, SLOT (b0l(bool)));
    connect(ui->helloButton, SIGNAL (clicked(bool)), this, SLOT (b1l(bool)));
    connect(ui->blinkButton, SIGNAL (clicked(bool)), this, SLOT (b2l(bool)));
    connect(ui->stopButton, SIGNAL (clicked(bool)), this, SLOT (b3l(bool)));

    connect(ui->okButton_2, SIGNAL (clicked(bool)), this, SLOT (b0r(bool)));
    connect(ui->helloButton_2, SIGNAL (clicked(bool)), this, SLOT (b1r(bool)));
    connect(ui->blinkButton_2, SIGNAL (clicked(bool)), this, SLOT (b2r(bool)));
    connect(ui->stopButton_2, SIGNAL (clicked(bool)), this, SLOT (b3r(bool)));

    connect(ui->t0, SIGNAL (clicked(bool)), this, SLOT (tb0(bool)));
    connect(ui->t1, SIGNAL (clicked(bool)), this, SLOT (tb1(bool)));
    connect(ui->t2, SIGNAL (clicked(bool)), this, SLOT (tb2(bool)));
    connect(ui->t3, SIGNAL (clicked(bool)), this, SLOT (tb3(bool)));
}

void MainWindow::connectSetup() {

    ui->talkAddress->setText( "develop32" ); // Initial setting
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

    ui->m0Slider->setRange( 0, SLIDER_RESOLUTION );
    ui->m0Slider->show();
    connect(ui->m0Slider, SIGNAL (valueChanged(int)), this, SLOT (sliderChanged0(int)));

    ui->m1Slider->setRange( 0, SLIDER_RESOLUTION );
    ui->m1Slider->show();
    connect(ui->m1Slider, SIGNAL (valueChanged(int)), this, SLOT (sliderChanged1(int)));

    ui->m2Slider->setRange( 0, 16 );
    ui->m2Slider->show();
    connect(ui->m2Slider, SIGNAL (valueChanged(int)), this, SLOT (sliderChanged2(int)));

    ui->m3Slider->setRange( 0, SLIDER_RESOLUTION );
    ui->m3Slider->show();
    connect(ui->m3Slider, SIGNAL (valueChanged(int)), this, SLOT (sliderChanged3(int)));

    showSpeed();
}

void MainWindow::sliderChanged0(int newValue) {

    fprintf(stderr,"sliderChanged0\n");
    ui->messageTextLine->setText( QString::number(newValue) );
    slider[0] = newValue;
}

void MainWindow::sliderChanged1(int newValue) {

    fprintf(stderr,"sliderChanged1\n");
    ui->messageTextLine->setText( QString::number(newValue) );
    slider[1] = newValue;
}

void MainWindow::sliderChanged2(int newValue) {

    fprintf(stderr,"sliderChanged2\n");
    ui->messageTextLine->setText( QString::number(newValue) );
    slider[2] = newValue;
    ui->m0Slider->setValue( speed[newValue].left );
    ui->m1Slider->setValue( speed[newValue].right );
}

void MainWindow::sliderChanged3(int newValue) {

    fprintf(stderr,"sliderChanged3\n");
    ui->messageTextLine->setText( QString::number(newValue) );
    slider[3] = newValue;
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
    ui->t0->hide();
    ui->t1->hide();
    ui->t2->hide();
    ui->t3->hide();

    ui->m0Slider->hide();
    ui->m1Slider->hide();
    ui->m2Slider->hide();
    ui->m3Slider->hide();
}

void MainWindow::slotMotorMenu() {

    commMode = false;
    motorMode = true;

    ui->talkAddress->setText( "" );

    ui->connectButton->setText("Setup IO");
    ui->connectBox->show();

    ui->okButton->setText("left -");
    ui->helloButton->setText("right -");
    ui->blinkButton->setText("speed -");
    ui->stopButton->setText("-");
    ui->okButton_2->setText("+ left");
    ui->helloButton_2->setText("+ right");
    ui->blinkButton_2->setText("+ speed");
    ui->stopButton_2->setText("+");
    ui->t0->setText("Start");
    ui->t1->setText("Stop");
    ui->t2->setText("Accept");
    ui->t3->setText("Reverse");

    ui->okButton_2->show();
    ui->helloButton_2->show();
    ui->blinkButton_2->show();
    ui->stopButton_2->show();
    ui->t0->show();
    ui->t1->show();
    ui->t2->show();
    ui->t3->show();

    slider[0] = speed[0].left;
    slider[1] = speed[0].right;
    slider[2] = 0;
    slider[3] = 0;

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
                if ( nullptr == hw ) {
                    hw = new hardware();
                }
                connected = hw->setupForDCMotors();
                ui->connectButton->setText("Setting Up");
                ui->talkAddress->setText( "Test Setup" ); // ?
                if ( connected ) {
                    fprintf(stderr,"connected\n");
                    ui->connectButton->setText("Cleanup IO");
                    ui->talkBox->show();
                } else {
                    fprintf(stderr,"not connected\n");
                    ui->connectButton->setText("Not Ready");
                    ui->talkBox->hide();
                    ui->connectButton->setChecked(false);
                }
            } else {
                ui->talkAddress->setText( "" );
                connected = hw->resetForDCMotors();
                hw = nullptr;
                ui->connectButton->setText("Setup IO");
                ui->talkBox->hide();
            }
        } else {
            ui->talkAddress->setText( "No mode set - use menu" );
        }
    }
}

void MainWindow::b0l(bool checked) {

    if (commMode ) {
        ui->okButton->setChecked(!checked);
        ui->messageTextLine->setText("ok");
        char *resp = piPtr->sendMessage( "ok\n" );
        ui->responseDisplay->setPlainText(resp);
    }
    if (motorMode) {
        ui->okButton->setChecked(false);
        if ( slider[0] > 0 ) {
            ui->m0Slider->setValue( slider[0] - 1 );
        } else {
            ui->m0Slider->setValue( 0 );
        }
    }
}

void MainWindow::b1l(bool checked) {

    if (commMode ) {
        ui->helloButton->setChecked(!checked);
        ui->messageTextLine->setText("hello");
        char *resp = piPtr->sendMessage( "hello\n" );
        ui->responseDisplay->setPlainText(resp);
    }
    if (motorMode) {
        ui->helloButton->setChecked(false);
        if ( slider[1] > 0 ) {
            ui->m1Slider->setValue( slider[1] - 1 );
        } else {
            ui->m1Slider->setValue( 0 );
        }
    }
}

void MainWindow::b2l(bool checked) {

    if (commMode ) {
        ui->blinkButton->setChecked(!checked);
        ui->messageTextLine->setText("blink");
        char *resp = piPtr->sendMessage( "blink\n" );
        ui->responseDisplay->setPlainText(resp);
    }
    if (motorMode) {
        ui->blinkButton->setChecked(false);
        fprintf(stderr,"b2l s2: %d\n", slider[2]);
        if ( slider[2] > 0 ) {
            ui->m2Slider->setValue( slider[2] - 1 );
            ui->m0Slider->setValue( speed[slider[2]].left );
            ui->m1Slider->setValue( speed[slider[2]].right );
        } else {
            ui->m2Slider->setValue( 0 );
            ui->m0Slider->setValue( speed[0].left );
            ui->m1Slider->setValue( speed[0].right );
        }
    }
}

void MainWindow::b3l(bool checked) {

    if (commMode ) {
        ui->stopButton->setChecked(!checked);
        ui->messageTextLine->setText("blinkstop");
        char *resp = piPtr->sendMessage( "blinkstop\n" );
        ui->responseDisplay->setPlainText(resp);
    }
    if (motorMode) {
        ui->stopButton->setChecked(false);
        if ( slider[3] > 0 ) {
            ui->m3Slider->setValue( slider[3] - 1 );
        }
    }
}

void MainWindow::b0r(bool checked) {

    if (motorMode) {
        ui->okButton->setChecked(false);
        if ( slider[0] < SLIDER_RESOLUTION ) {
            ui->m0Slider->setValue( slider[0] + 1 );
        } else {
            ui->m0Slider->setValue( SLIDER_RESOLUTION - 1 );
        }
    }
}

void MainWindow::b1r(bool checked) {

    if (motorMode) {
        ui->helloButton->setChecked(false);
        if ( slider[1] < SLIDER_RESOLUTION ) {
            ui->m1Slider->setValue( slider[1] + 1 );
        } else {
            ui->m1Slider->setValue( SLIDER_RESOLUTION - 1 );
        }
    }
}

void MainWindow::b2r(bool checked) {

    if (motorMode) {
        ui->blinkButton->setChecked(false);
        fprintf(stderr,"b2r s2: %d\n", slider[2]);
        if ( slider[2] < 16 ) {
            ui->m2Slider->setValue( slider[2] + 1 );
            ui->m0Slider->setValue( speed[slider[2]].left );
            ui->m1Slider->setValue( speed[slider[2]].right );
        } else {
            ui->m2Slider->setValue( SPEED_ARRAY - 1 );
            ui->m0Slider->setValue( speed[SPEED_ARRAY - 1].left );
            ui->m1Slider->setValue( speed[SPEED_ARRAY - 1].right );
        }
    }
}

void MainWindow::b3r(bool checked) {

    if (motorMode) {
        ui->stopButton->setChecked(false);
        if ( slider[3] < SLIDER_RESOLUTION ) {
            ui->m3Slider->setValue( slider[3] + 1 );
        }
    }
}

void MainWindow::tb0(bool checked) {

    fprintf(stderr,"tb0\n");
}

void MainWindow::tb1(bool checked) {

    fprintf(stderr,"tb1, s0: %d, s1: %d, s2: %d\n", slider[0], slider[1], slider[2]);
    showSpeed();
}

void MainWindow::tb2(bool checked) {

    fprintf(stderr,"tb2, s0: %d, s1: %d, s2: %d\n", slider[0], slider[1], slider[2]);
    speed[slider[2]].left = slider[0];
    speed[slider[2]].right = slider[1];
    showSpeed();
}

void MainWindow::tb3(bool checked) {

    fprintf(stderr,"tb3\n");
}
void MainWindow::showSpeed() {

    fprintf(stderr,"showSpeed\n");
    for ( int i = 0; i < 4; i++ ) {
        fprintf(stderr,"%d l: %d, r: %d\n", i, speed[i].left, speed[i].right);
    }
}
