//#define USE_MOTOR

#include "motor.h"

#ifdef USE_MOTOR
#include <wiringPi.h>
#endif  // USE_MOTOR


Motor::Motor() {

    activated = false;
}

bool Motor::setupForMotor() {

#ifdef USE_MOTOR
    int setupResult = wiringPiSetup();
    printf( "Pi version: %d\n", setupResult );

    pinMode( L1, OUTPUT );
    pinMode( L2, OUTPUT );
    pinMode( L3, OUTPUT );
    pinMode( L4, OUTPUT );
#endif  // USE_MOTOR

    activated = true;
    return activated;
}

bool Motor::resetForMotor() {

    activated = false;
    return activated;
}

void Motor::blinkLED() {

    for (int i = 0; i < 4; i++ ) {
#ifdef USE_MOTOR
        digitalWrite (L1, HIGH) ;	// On
        delay (500) ;                // mS
        digitalWrite (L1, LOW) ;	// Off
        delay (500) ;
#endif  // USE_MOTOR
    }
}

void Motor::onPin( int led ) {

#ifdef USE_MOTOR
    digitalWrite (led, HIGH) ;	// On
#endif  // USE_MOTOR
}
void Motor::offPin( int led ) {

#ifdef USE_MOTOR
    digitalWrite (led, LOW) ;	// Off
#endif  // USE_MOTOR
}
