#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setFixedSize(400, 400);
    ui->talkAddress->setText( "zerowpi2" ); // Initial setting

    buttonSetup();

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

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotConnectClicked(bool checked) {

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
            ui->connectButton->setChecked(false);
        }
    } else {
        connected = piPtr->detachFrom();
        ui->connectButton->setText("Connect");
        ui->talkBox->hide();
    }
}

void MainWindow::slotOKClicked(bool checked) {

    ui->okButton->setChecked(!checked);
    ui->messageTextLine->setText("ok");
    char *resp = piPtr->sendMessage( "ok\n" );
    ui->responseDisplay->setPlainText(resp);
}

void MainWindow::slotHelloClicked(bool checked) {

    ui->helloButton->setChecked(!checked);
    ui->messageTextLine->setText("hello");
    char *resp = piPtr->sendMessage( "hello\n" );
    ui->responseDisplay->setPlainText(resp);
}

void MainWindow::slotBlinkClicked(bool checked) {

    ui->blinkButton->setChecked(!checked);
    ui->messageTextLine->setText("blink");
    char *resp = piPtr->sendMessage( "blink\n" );
    ui->responseDisplay->setPlainText(resp);
}

void MainWindow::slotStopClicked(bool checked) {

    ui->stopButton->setChecked(!checked);
    ui->messageTextLine->setText("blinkstop");
    char *resp = piPtr->sendMessage( "blinkstop\n" );
    ui->responseDisplay->setPlainText(resp);
}
