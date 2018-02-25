#ifndef HARDWARE_H
#define HARDWARE_H


//#define USE_HARDWARE

#ifdef USE_HARDWARE

#include <wiringPi.h>
#include <linux/i2c-dev.h>
#include <wiringPiI2C.h>

#endif  // USE_HARDWARE


// Registers/etc.
#define MODE1           0x00
#define MODE2           0x01
#define SUBADR1         0X02
#define SUBADR2         0X03
#define SUBADR3         0X04
#define PRESCALE        0XFE
#define LED0_ON_L       0X06
#define LED0_ON_H       0X07
#define LED0_OFF_L      0X08
#define LED0_OFF_H      0X09
#define ALL_LED_ON_L    0XFA
#define ALL_LED_ON_H    0XFB
#define ALL_LED_OFF_L   0XFC
#define ALL_LED_OFF_H   0XFD

// Bits
#define RESTART         0X80
#define SLEEP           0x10
#define ALLCALL         0x01
#define INVRT           0x10
#define OUTDRV          0x04


class I2C {

public:
    explicit I2C( int addr );

    bool    debug;
    int     address;             // I2C address
    int     device;             // File handle to I2C device

    int setupI2C( int addr );
    int i2cRead           (int reg) ;
    int i2cReadReg8       (int reg) ;
    int i2cReadReg16      (int reg) ;

    int i2cWrite          (int reg, int data) ;
    int i2cWriteReg8      (int reg, int data) ;
    int i2cWriteReg16     (int reg, int data) ;

};


//class PWM {

//public:
//    explicit PWM( int addr );

//    bool    debug;
//    int     address;             // I2C address
//    I2C     i2c;

//    void setPWMFrequency( int freq );
//    void setPWM( int channel, int on, int off );
//    void setPWMAll( int on, int off );
//};

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
    bool    motorsSetup;
    int     i2cAddress;
    int     i2cFrequency;
//    PWM     motors[2];


    bool setupForDCMotors();
    bool resetForDCMotors();

//    int setPin( int pin, int value );
//    int getMotor( int motor );

};

#endif // HARDWARE_H
