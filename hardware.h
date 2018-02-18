#ifndef HARDWARE_H
#define HARDWARE_H

#include <QObject>

#define USE_HARDWARE

#ifdef USE_HARDWARE

#include <wiringPi.h>
#include <linux/i2c-dev.h>
#include <wiringPiI2C.h>

#endif  // USE_HARDWARE


//#define L1  23


class hardware: public QObject
{
    Q_OBJECT

public:
    explicit hardware();

    bool setupForDCMotors();
    bool resetForDCMotors();

    bool    motorsSetup = false;
    int     pi2c = 0;             // File descriptor for I2C address


signals:

public slots:

};

#endif // HARDWARE_H
