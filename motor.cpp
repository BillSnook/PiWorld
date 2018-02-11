#include "motor.h"

#include <unistd.h>			// close read write
#include <stdio.h>			// printf
#include <fcntl.h>			// open
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <getopt.h>


#define VREG 2
#define CREG 4
#define BUFSIZE	16
#define DEV "/dev/i2c-1"
#define ADRS 0x36


static int readReg(int busfd, __uint16_t reg, unsigned char *buf, int bufsize)
{
    unsigned char reg_buf[2];

    reg_buf[0] = (reg >> 0) & 0xFF;
    reg_buf[1] = (reg >> 8) & 0xFF;

    int ret = write(busfd, reg_buf, 2);

    if (ret < 0) {
        printf("Write failed trying to read reg: %04x (0x%02x 0x%02x)\n", reg, reg_buf[0], reg_buf[1]);
        return ret;
    }

    return read(busfd, buf, bufsize);
}



Motor::Motor() {

//    activated = false;
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

//    activated = true;q
    return true;
}

bool Motor::resetForMotor() {

#ifdef USE_MOTOR
    softPwmStop( L1 );
    softPwmStop( M1En );
    softPwmStop( M2En );
#endif  // USE_MOTOR
//    activated = false;
    return false;
}

void Motor::blinkLED() {

    for (int i = 10; i < 100; i += 20 ) {
#ifdef USE_MOTOR
//        digitalWrite (L1, HIGH) ;	// On
//        delay (500) ;                // mS
//        digitalWrite (L1, LOW) ;	// Off
//        delay (500) ;
        setPin( L1, i );
        delay( 1000 );
#endif  // USE_MOTOR
    }
    setPin( L1, 0 );
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

void Motor::setPin( int pin, int value ) {

#ifdef USE_MOTOR
    softPwmWrite( pin, value ) ;
#endif  // USE_MOTOR
}

void Motor::checkMotor(int motor, int direction , int speed) {

    fprintf(stderr,"checkMotor motor == %d\n", motor);
    if ( motor == 1 ) {
        setPin( M1En, 0 );
        if ( direction == 1 ) {
            onPin( M1Fw );
            offPin( M1Rv );
        } else {
            offPin( M1Fw );
            onPin( M1Rv );
        }

        setPin( M1En, speed );
#ifdef USE_MOTOR
        delay( 1000 );
#endif  // USE_MOTOR
        setPin( M1En, 0 );

    }
    if ( motor == 2 ) {
        setPin( M2En, 0 );
        if ( direction == 1 ) {
            onPin( M2Fw );
            offPin( M2Rv );
        } else {
            offPin( M2Fw );
            onPin( M2Rv );
        }

        setPin( M2En, speed );
#ifdef USE_MOTOR
        delay( 1000 );
#endif  // USE_MOTOR
        setPin( M2En, 0 );

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
        setPin( M1En, speed );
    }
    if ( motor == 2 ) {
        if ( direction == 1 ) {
            onPin( M2Fw );
            offPin( M2Rv );
        } else {
            offPin( M2Fw );
            onPin( M2Rv );
        }
        setPin( M2En, speed );
    }
}

void Motor::setMtrSpd(int motor, int speed) {

    fprintf(stderr,"setMtrSpd m %d, s: %d\n", motor, speed);
    if ( motor == 1 ) {
        setPin( M1En, speed );
    }
    if ( motor == 2 ) {
        setPin( M2En, speed );
    }
}

void Motor::getUPS() {

    int vOpt = 1, cOpt = 1;
    unsigned char buf[BUFSIZE] = {0};

    int busfd;
    if ((busfd = open(DEV, O_RDWR)) < 0) {
        printf("can't open %s (running as root?)\n",DEV);
        return;
    }

    int ret = ioctl(busfd, I2C_SLAVE, ADRS);
    if (ret < 0) {
        printf("i2c device initialisation failed\n");
        return;
    }

    readReg(busfd, VREG, buf, 2);

    int hi,lo;
    hi = buf[0];
    lo = buf[1];
    int v = (hi << 8)+lo;
    if (vOpt) {
        fprintf( stderr, "%fV ",(((float)v)* 78.125 / 1000000.0));
    }

    readReg(busfd, CREG, buf, 2);
    hi = buf[0];
    lo = buf[1];
    v = (hi << 8)+lo;
    if (!cOpt && !vOpt) {
        fprintf( stderr, "%i",(int)(((float)v) / 256.0));
    }

    if (cOpt) {
        fprintf( stderr, "%f%%",(((float)v) / 256.0));
    }

    fprintf( stderr, "\n");

    close(busfd);
}
