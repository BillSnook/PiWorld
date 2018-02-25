#include "hardware.h"

#include <unistd.h>			// close read write
#include <stdio.h>			// printf
#include <fcntl.h>			// open
#include <sys/ioctl.h>
#include <getopt.h>


#define VREG 2
#define CREG 4
#define BUFSIZE	16
#define DEV "/dev/i2c-1"
#define ADRS 0x36


I2C::I2C(int addr) {

    debug = false;
    address = addr;
}

int I2C::i2cRead(int reg) {

    int rdValue = 0;
#ifdef USE_HARDWARE
    rdValue = wiringPiI2CReadReg8 ( device, reg ) ;	// Read 8 bits from register reg on device
#endif  // USE_HARDWARE
    return rdValue;
}

int I2C::i2cReadReg8(int reg) {

    int rdValue = 0;
#ifdef USE_HARDWARE
    rdValue = wiringPiI2CReadReg8 ( device, reg ) ;	// Read 8 bits from register reg on device
#endif  // USE_HARDWARE
    return rdValue;
}

int I2C::i2cReadReg16(int reg) {

    int rdValue = 0;
#ifdef USE_HARDWARE
    rdValue = wiringPiI2CReadReg16 ( device, reg ) ;	// Read 16 bits from register reg on device
#endif  // USE_HARDWARE
    return rdValue;
}


int I2C::i2cWrite(int reg, int data) {

#ifdef USE_HARDWARE
    wiringPiI2CWriteReg8(device, reg, data);
#endif  // USE_HARDWARE
}

int I2C::i2cWriteReg8(int reg, int data) {

#ifdef USE_HARDWARE
    wiringPiI2CWriteReg8(device, reg, data);
#endif  // USE_HARDWARE
}

int I2C::i2cWriteReg16(int reg, int data) {

#ifdef USE_HARDWARE
    wiringPiI2CWriteReg16(device, reg, data);
#endif  // USE_HARDWARE
}

// Addresses
#define MODE1                   0x00
#define MODE2                   0x01
#define SUBADR1                 0x02
#define SUBADR2                 0x03
#define SUBADR3                 0x04
#define PRESCALE                0xFE

#define CHANNEL0_ON_L           0x06
#define CHANNEL0_ON_H           0x07
#define CHANNEL0_OFF_L          0x08
#define CHANNEL0_OFF_H          0x09

#define CHANNEL0_ON_L           0x06
#define CHANNEL0_ON_H           0x07
#define CHANNEL0_OFF_L          0x08
#define CHANNEL0_OFF_H          0x09

// Bits
#define RESTART                 0x80
#define SLEEP                   0x10
#define ALLCALL                 0x01
#define INVRT                   0x10
#define OUTDRV                  0x04

//PWM::PWM( int device ) {

//    debug = false;
//    i2c = new I2C();
//    i2c->setupI2C( device );
//    setPWMAll( 0, 0 );                  // Clear all to 0
//    i2c->i2cWrite( MODE2, OUTDRV );
//    i2c->i2cWrite( MODE1, ALLCALL );
//    i2c->i2cWrite( CHANNEL0_OFF_L, 0 );
//    i2c->i2cWrite( CHANNEL0_OFF_H, 0 );

//    delay( 1 );                         // Millisecond to let oscillator setup

//    int mode1 = i2c->i2cRead( MODE1 );
//    mode1 = mode1 & ~SLEEP;             // Turn off sleep - wake up
//    i2c->i2cWrite( MODE1, mode1 );

//    delay( 1 );                         // Millisecond to let oscillator stabileize


//}

//void PWM::setPWMFrequency( int freq ) {
//}

//void PWM::setPWM( int channel, int on, int off ) {

//    i2c->i2cWrite( CHANNEL0_ON_L + (4 * channel), on & 0xFF );
//    i2c->i2cWrite( CHANNEL0_ON_H + (4 * channel), on >> 8 );
//    i2c->i2cWrite( CHANNEL0_OFF_L + (4 * channel), off & 0xFF );
//    i2c->i2cWrite( CHANNEL0_OFF_H + (4 * channel), off >> 8 );
//}

//void PWM::setPWMAll( int on, int off ) {

//    i2c->i2cWrite( ALLCHANNEL_ON_L, on & 0xFF );
//    i2c->i2cWrite( ALLCHANNEL_ON_H, on >> 8 );
//    i2c->i2cWrite( ALLCHANNEL_OFF_L, off & 0xFF );
//    i2c->i2cWrite( ALLCHANNEL_OFF_H, off >> 8 );
//}


////DCM::DCM( hardware board, int motor ) {

////    controller = board;
////    motorNumber = motor;
////}}




hardware::hardware() {

    motorsSetup = false;

#ifdef USE_HARDWARE
    int setupResult = wiringPiSetup();
    if ( setupResult == -1 ) {
        fprintf( stderr, "Error setting up wiringPi." );
        return;
    }
    fprintf( stderr, "Pi version: %d\n", setupResult );
#endif  // USE_HARDWARE

    i2c = new I2C( 0x6F );


}

bool hardware::setupForDCMotors() {


    motorsSetup = true;
    return motorsSetup;
    return true;
}

bool hardware::resetForDCMotors() {

#ifdef USE_HARDWARE
#endif  // USE_HARDWARE

    delete i2c;

    motorsSetup = false;
    return motorsSetup;
}


//int hardware::getI2CReg( int reg ) {

//    int rdValue = 0;
//#ifdef USE_HARDWARE
//    rdValue = wiringPiI2CReadReg16 (pi2c, reg) ;	// On
//#endif  // USE_HARDWARE
//    return rdValue;
//}

//void hardware::putI2CReg( int reg, int newValue ) {

//#ifdef USE_HARDWARE
//    wiringPiI2CWriteReg16 (pi2c, reg, newValue);
//#endif  // USE_HARDWARE
//}

//char *hardware::getUPS2() {

//    char *statsV = (char *)valloc( 128 );
//    char statsC[64];

//#ifdef USE_HARDWARE
//    pi2c = wiringPiI2CSetup( ADRS );

//    int v = getI2CReg( VREG );
//    int lo = (v >> 8) & 0x00FF;
//    int hi = (v << 8) & 0xFF00;
//    v = hi + lo;
//    sprintf( statsV, "%fV ",(((float)v)* 78.125 / 1000000.0));

//    int c = getI2CReg( CREG );
//    close( pi2c );

//    lo = (c >> 8) & 0x00FF;
//    hi = (c << 8) & 0xFF00;
//    c = hi + lo;
//    sprintf( statsC, "%f%%\n",(((float)c) / 256.0));
//    strcat( statsV, statsC );
//#endif // USE_HARDWARE

//    return statsV;
//}

//void hardware::setPin( int pin, int value ) {

//    if ( ( pin < 0 ) || ( pin > 15 ) ) {
//        return;
//    }
//    if ( value == 0 ) {

//    }
//    if ( value == 1 ) {

//    }
//    return;
//}

//DCM hardware::getMotor( int motor ) {
//    if ( ( motor < 1 ) || ( motor > 4 ) ) {
//        return nullptr;
//    }
//    return motors[motor]
//}
