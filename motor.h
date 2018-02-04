#ifndef MOTOR_H
#define MOTOR_H

#include <QObject>


//#define USE_MOTOR

#ifdef USE_MOTOR
#include <wiringPi.h>
#endif  // USE_MOTOR


#define L1  23
#define L2  24
#define L3  27
#define L4  25

#define M1En    11
#define M1Fw    13
#define M1Rv    15
#define M2En    22
#define M2Fw    18
#define M2Rv    16


class Motor : public QObject
{
//    Q_OBJECT
public:
    explicit Motor();

    bool activated;

    bool setupForMotor();
    bool resetForMotor();

    void blinkLED();
    void onPin( int led );
    void offPin( int led );

    void setPin( int pin, int value );
    void checkMotor( int motor, int direction, int speed );
};

#endif // MOTOR_H
