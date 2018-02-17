#ifndef HARDWARE_H
#define HARDWARE_H

#include <QObject>

//#define USE_MOTOR

#ifdef USE_MOTOR
#include <wiringPi.h>
//#include <softPwm.h>
#include <linux/i2c-dev.h>
#include <wiringPiI2C.h>
#endif  // USE_MOTOR


//#define L1  23
//#define L2  24
//#define L3  27
//#define L4  25

//#define M1En    0
//#define M1Fw    2
//#define M1Rv    3
//#define M2En    6
//#define M2Fw    5
//#define M2Rv    4


class hardware // : public QObject
{

public:
    explicit hardware();

    bool setupForMotor();
    bool resetForMotor();

    bool    motorRunning = false;
    int     pi2c = 0;             // File descriptor for I2C address


signals:

public slots:
};

#endif // HARDWARE_H
