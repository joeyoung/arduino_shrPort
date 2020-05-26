// flashPin basic example

// created: Oct 12, 2013 G. D. (Joe) Young <jyoung@islandnet.com>

// revised: Jan 14/14 - using version 2 with resting level specified

// Illustrate the use of non-blocking timers in the flashPin library
// to flash arduino pins.

// The pin13 object is constructed using the default parameters for
// the pin (13), resting level (LOW) and for the duration (250 msec).
// The pin12 object is constructed specifying all parameters.

#include <flashPin.h>

flashPin pin13;            // use default parameters: pin 13, 250 msec
flashPin pin12( 12, HIGH, 500 );
flashPin myPin( 11, LOW, 500 );

unsigned long sectick;

void setup( ) {
  sectick = millis( ) + 2000;
}

void loop( ) {
  if( sectick < millis( ) ) {
    sectick += 2000;
    pin13.flash( );
    pin12.flash( );
    myPin.flash( 750 );  // optionally specify new duration
  }
  pin13.flashOff( );
  pin12.flashOff( );
  myPin.flashOff( );
}
