#include "motor.h"


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
