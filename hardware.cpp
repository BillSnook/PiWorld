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


hardware::hardware() //  : QObject(parent)
{

    motorRunning = true;
    pi2c = 0;
}

bool hardware::setupForMotor() {

#ifdef USE_MOTOR
    int setupResult = wiringPiSetup();
    if ( setupResult == -1 ) {
        fprintf( stderr, "Error setting up wiringPi." );
        return false;
    }
    fprintf( stderr, "Pi version: %d\n", setupResult );

#endif  // USE_MOTOR

//    motorRunning = 0;

    return motorRunning;
}

bool hardware::resetForMotor() {

#ifdef USE_MOTOR
#endif  // USE_MOTOR

//    motorRunning = false;
    return motorRunning;
}

