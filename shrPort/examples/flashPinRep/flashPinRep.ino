// flashPinRep basic example

// created: Oct 12, 2013 G. D. (Joe) Young <jyoung@islandnet.com>

// revised: Jan 14/14 - using version 2? with resting level specified
//          Apr 26/20 - revised library, illustrate flashRep repeating flash

// Illustrate the use of non-blocking timers in the flashPin library
// to flash arduino pins.

// The pin13 object is constructed using the default parameters for
// the pin (13), resting level (LOW) and for the duration (250 msec).
// The pin12 object is constructed specifying all parameters.

#include <flashPin.h>

//flashPin pin13;            // use default parameters: pin 13, 250 msec
flashPin pin12( 9, LOW, 100 );
flashPin myPin( 8, HIGH, 500 );

unsigned long sectick;

void setup( ) {
  sectick = millis( );
}

void loop( ) {
  if( millis( ) - sectick > 3000 ) {
    sectick = millis( );
//    pin13.flash( );
    pin12.flashRep( 3 );  // 3 flashes each call
    myPin.flash( 750 );  // optionally specify new duration
  }
//  pin13.flashOff( );
//  pin12.flashRep( 1 ); // continuous on/off flashing
  pin12.flashOff( );
  myPin.flashOff( );
}
