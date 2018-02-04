#include "motor.h"


Motor::Motor() {

    activated = false;
}

bool Motor::setupForMotor() {

#ifdef USE_MOTOR
    int setupResult = wiringPiSetup();
    if ( setupResult == -1 ) {
        printf( "Error setting up wiringPi." );
        return;
    }
    printf( "Pi version: %d\n", setupResult );

    pinMode( L1, OUTPUT );
    pinMode( L2, OUTPUT );
    pinMode( L3, OUTPUT );
    pinMode( L4, OUTPUT );

    pinMode( M1En, PWM_OUTPUT );
    pinMode( M1FW, OUTPUT );
    pinMode( M1Rv, OUTPUT );

    pinMode( M2En, PWM_OUTPUT );
    pinMode( M2Fw, OUTPUT );
    pinMode( M2Rv, OUTPUT );
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
    pwmWrite (pin, value) ;
#endif  // USE_MOTOR
}

void Motor::checkMotor(int motor, int direction , int speed) {

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
        delay( 1 );
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
        delay( 1 );
#endif  // USE_MOTOR
        setPin( M2En, 0 );

    }
}
