#ifndef MOTOR_H
#define MOTOR_H

#include <QObject>


#define USE_MOTOR

#ifdef USE_MOTOR
#include <wiringPi.h>
#include <softPwm.h>
#endif  // USE_MOTOR


#define L1  23
#define L2  24
#define L3  27
#define L4  25

#define M1En    0
#define M1Fw    2
#define M1Rv    3
#define M2En    6
#define M2Fw    5
#define M2Rv    4
#define M3En    12
#define M3Fw    14
#define M3Rv    13


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
    void setMtrDirSpd(int motor, int direction , int speed);
    void setMtrSpd(int motor, int speed);
};

#endif // MOTOR_H
