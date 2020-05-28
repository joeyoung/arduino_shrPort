// flashPinRep basic example

// created: May 27, 2020 G. D. (Joe) Young <jyoung@islandnet.com>

// revised: 

// Illustrate the use of non-blocking timers in the flashPin library
// to flash arduino pins.

// A minimal implementation - default params for pin 13, use completing
// timer feature of flashRep to blink the built-in arduino led


#include <flashPin.h>

flashPin pin13;            // use default parameters: pin 13, 250 msec

void setup( ) {

}

void loop( ) {
  pin13.flashRep( 1 );
  pin13.flashOff( );
}
