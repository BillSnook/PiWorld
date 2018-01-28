#ifndef MOTOR_H
#define MOTOR_H

//#include <QObject>
//#include <wiringPi.h>

class Motor // : public QObject
{
//    Q_OBJECT
public:
    explicit Motor();

    int sockfd, portno;

    bool setupForMotor();

};

#endif // MOTOR_H
