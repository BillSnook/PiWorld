#ifndef HARDWARE_H
#define HARDWARE_H


#define USE_HARDWARE

#ifdef USE_HARDWARE

#include <wiringPi.h>
#include <linux/i2c-dev.h>
#include <wiringPiI2C.h>

#endif  // USE_HARDWARE



class I2C {

public:
    explicit I2C( int addr );

    bool    debug;
    int     address;            // I2C address
    int     device;             // File handle to I2C device

    int i2cRead           (int reg) ;
    int i2cReadReg8       (int reg) ;
    int i2cReadReg16      (int reg) ;

    int i2cWrite          (int reg, int data) ;
    int i2cWriteReg8      (int reg, int data) ;
    int i2cWriteReg16     (int reg, int data) ;

};


class PWM {

public:
    explicit PWM( I2C *i2cBus );

    bool    debug;
    int     address;             // I2C address
    I2C     *i2c;

    void setPWMFrequency( int freq );
    void setPWM( int channel, int on, int off );
    void setPWMAll( int on, int off );
};

////class DCM {

////public:
////    explicit DCM( hardware board, int motor );

////    bool        debug;
////    hardware    controller;
////    int         motorNumber;
////};


class hardware {

public:
    explicit hardware();

    I2C     *i2c;
    PWM     *pwm;

    bool    motorsSetup;
    int     i2cAddress;
    int     i2cFrequency;


    bool setupForDCMotors();
    bool resetForDCMotors();

//    int setPin( int pin, int value );
//    int getMotor( int motor );

};

#endif // HARDWARE_H
