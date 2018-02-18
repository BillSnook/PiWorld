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

//    motorsSetup = true;
//    pi2c = 0;
}

bool hardware::setupForDCMotors() {

#ifdef USE_HARDWARE
    int setupResult = wiringPiSetup();
    if ( setupResult == -1 ) {
        fprintf( stderr, "Error setting up wiringPi." );
        return false;
    }
    fprintf( stderr, "Pi version: %d\n", setupResult );

#endif  // USE_HARDWARE

//    motorsSetup = true;

    return true; //motorsSetup;
}

bool hardware::resetForDCMotors() {

#ifdef USE_HARDWARE
#endif  // USE_HARDWARE

//    motorsSetup = false;
    return false; //motorsSetup;
}

