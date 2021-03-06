#include "motor.h"

#include <unistd.h>			// close read write
#include <stdio.h>			// printf
#include <fcntl.h>			// open
#include <sys/ioctl.h>
#include <getopt.h>


//  SBC HAT - GPIO-controlled
//  Raspi UPS HAT V1.0
#define VREG 2
#define CREG 4
#define BUFSIZE	16
#define DEV "/dev/i2c-1"
#define ADRS 0x36


Motor::Motor() {

//    motorRunning = 1;
}

bool Motor::setupForMotor() {

#ifdef USE_MOTOR
    int setupResult = wiringPiSetup();
    if ( setupResult == -1 ) {
        fprintf( stderr, "Error setting up wiringPi." );
        return false;
    }
    fprintf( stderr, "Pi version: %d\n", setupResult );

    pinMode( L1, SOFT_PWM_OUTPUT );
    softPwmCreate( L1, 0, 100 );
    pinMode( L2, OUTPUT );
    pinMode( L3, OUTPUT );
    pinMode( L4, OUTPUT );

    pinMode( M1En, SOFT_PWM_OUTPUT );
    pinMode( M1Fw, OUTPUT );
    pinMode( M1Rv, OUTPUT );

    pinMode( M2En, SOFT_PWM_OUTPUT );
    pinMode( M2Fw, OUTPUT );
    pinMode( M2Rv, OUTPUT );

    softPwmCreate( M1En, 0, 100 );
    softPwmCreate( M2En, 0, 100 );
#endif  // USE_MOTOR

//    motorRunning = 1;
    return true;
}

bool Motor::resetForMotor() {

#ifdef USE_MOTOR
    softPwmStop( L1 );
    softPwmStop( M1En );
    softPwmStop( M2En );
#endif  // USE_MOTOR

//    motorRunning = 0;
    return false;
}

void Motor::blinkLED() {    // Now steps brightness using soft PWM

    for (int i = 10; i < 100; i += 20 ) {
#ifdef USE_MOTOR
//        digitalWrite (L1, HIGH) ;	// On
//        delay (500) ;                // mS
//        digitalWrite (L1, LOW) ;	// Off
//        delay (500) ;
        setPWMPin( L1, i );
        delay( 1000 );
#endif  // USE_MOTOR
    }
    setPWMPin( L1, 0 );
}

void Motor::onPin( int pin ) {

#ifdef USE_MOTOR
    digitalWrite (pin, HIGH) ;	// On
#endif  // USE_MOTOR
}

void Motor::offPin( int pin ) {

#ifdef USE_MOTOR
    digitalWrite (pin, LOW) ;	// Off
#endif  // USE_MOTOR
}

void Motor::setPWMPin( int pin, int value ) {

#ifdef USE_MOTOR
    softPwmWrite( pin, value ) ;
#endif  // USE_MOTOR
}

void Motor::checkMotor(int motor, int direction , int speed) {

    fprintf(stderr,"checkMotor motor == %d\n", motor);
    if ( motor == 1 ) {
        setPWMPin( M1En, 0 );
        if ( direction == 1 ) {
            onPin( M1Fw );
            offPin( M1Rv );
        } else {
            offPin( M1Fw );
            onPin( M1Rv );
        }

        setPWMPin( M1En, speed );
#ifdef USE_MOTOR
        delay( 1000 );
#endif  // USE_MOTOR
        setPWMPin( M1En, 0 );

    }
    if ( motor == 2 ) {
        setPWMPin( M2En, 0 );
        if ( direction == 1 ) {
            onPin( M2Fw );
            offPin( M2Rv );
        } else {
            offPin( M2Fw );
            onPin( M2Rv );
        }

        setPWMPin( M2En, speed );
#ifdef USE_MOTOR
        delay( 1000 );
#endif  // USE_MOTOR
        setPWMPin( M2En, 0 );

    }
}

void Motor::setMtrDirSpd(int motor, int direction , int speed) {

    fprintf(stderr,"setMtrDirSpd m %d, d: %s, s: %d\n", motor, direction ? "f" : "r", speed);
    if ( motor == 1 ) {
        if ( direction == 1 ) {
            onPin( M1Fw );
            offPin( M1Rv );
        } else {
            offPin( M1Fw );
            onPin( M1Rv );
        }
        setPWMPin( M1En, speed );
    }
    if ( motor == 2 ) {
        if ( direction == 1 ) {
            onPin( M2Fw );
            offPin( M2Rv );
        } else {
            offPin( M2Fw );
            onPin( M2Rv );
        }
        setPWMPin( M2En, speed );
    }
}

void Motor::setMtrSpd(int motor, int speed) {

    fprintf(stderr,"setMtrSpd m %d, s: %d\n", motor, speed);
    if ( motor == 1 ) {
        setPWMPin( M1En, speed );
    }
    if ( motor == 2 ) {
        setPWMPin( M2En, speed );
    }
}

int Motor::getI2CReg( int reg ) {

    int rdValue = 0;
#ifdef USE_MOTOR
    rdValue = wiringPiI2CReadReg16 (pi2c, reg) ;	// On
#endif  // USE_MOTOR
    return rdValue;
}

void Motor::putI2CReg( int reg, int newValue ) {

#ifdef USE_MOTOR
    wiringPiI2CWriteReg16 (pi2c, reg, newValue);
#endif  // USE_MOTOR
}

char *Motor::getUPS2() {

    char *statsV = (char *)valloc( 128 );
    char statsC[64];

#ifdef USE_MOTOR
    pi2c = wiringPiI2CSetup( ADRS );

    int v = getI2CReg( VREG );
    int lo = (v >> 8) & 0x00FF;
    int hi = (v << 8) & 0xFF00;
    v = hi + lo;
    sprintf( statsV, "%fV ",(((float)v)* 78.125 / 1000000.0));

    int c = getI2CReg( CREG );
    close( pi2c );

    lo = (c >> 8) & 0x00FF;
    hi = (c << 8) & 0xFF00;
    c = hi + lo;
    sprintf( statsC, "%f%%\n",(((float)c) / 256.0));
    strcat( statsV, statsC );
#endif // USE_MOTOR

    return statsV;
}
