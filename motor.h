#ifndef MOTOR_H
#define MOTOR_H

#include <QObject>

#define L1  23
#define L2  24
#define L3  27
#define L4  25

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

};

#endif // MOTOR_H
