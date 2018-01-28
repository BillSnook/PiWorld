#include "motor.h"
#include <wiringPi.h>

Motor::Motor() {

    activated = false;
}

bool Motor::setupForMotor() {

    int setupResult = wiringPiSetup();
    printf( "Pi version: %d\n", setupResult );

    pinMode( L1, OUTPUT );
    pinMode( L2, OUTPUT );
    pinMode( L3, OUTPUT );
    pinMode( L4, OUTPUT );

    activated = true;
    return activated;
}

bool Motor::resetForMotor() {

    activated = false;
    return activated;
}

void Motor::blinkLED() {

    for (int i = 0; i < 4; i++ ) {
       digitalWrite (L1, HIGH) ;	// On
       delay (500) ;                // mS
       digitalWrite (L1, LOW) ;	// Off
       delay (500) ;
    }
}

void Motor::onPin( int led ) {

    digitalWrite (led, HIGH) ;	// On
}
void Motor::offPin( int led ) {

    digitalWrite (led, LOW) ;	// Off
}
