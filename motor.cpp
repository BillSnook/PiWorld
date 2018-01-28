#include "motor.h"
#include <wiringPi.h>

#define LED1Pin     23
#define LED2Pin     24
#define LED3Pin     27
#define LED4Pin     25

Motor::Motor()
{

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

    int wPin = physPinToGpio( LED1Pin );
    for (int i = 0; i < 4; i++ )
     {
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

/*
 * wpiPinToGpio:
 *	Translate a wiringPi Pin number to native GPIO pin number.
 *	Provided for external support.
 *********************************************************************************
 */

//int wpiPinToGpio (int wpiPin)
//{
//  return pinToGpio [wpiPin & 63] ;
//}


/*
 * physPinToGpio:
 *	Translate a physical Pin number to native GPIO pin number.
 *	Provided for external support.
 *********************************************************************************
 */

//int physPinToGpio (int physPin)
//{
//  return physToGpio [physPin & 63] ;
//}
