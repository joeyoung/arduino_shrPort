// shrRotary - Rotary encoder using shrPort
//
// started: June 15, 2020  - G. D. (Joe) Young <jyoung@islandnet.com>
//
// revised:
//
// Using the state table and lookup method from the Rotary arduino library:
// Copyright 2011 Ben Buxton. Licenced under the GNU GPL Version 3.
// Contact: bb@cactii.net
//
//
#define HALF_STEP       //use output/half-step table. Needs to preceed shrRotary.h
#include "shrRotary.h"
#include <shrPort.h>


shrPort prt( 0x38, PCF8574 );
uint8_t state = 0;
int8_t counter;


void setup( ) {
  Serial.begin(57600);
  Wire.begin( );
  prt.begin( );
  Wire.setClock( 400000 );
}

void loop( ) {
  unsigned char result = process();
  if (result == DIR_CW) {
    counter++;
    Serial.println(counter);
  } else if (result == DIR_CCW) {
    counter--;
    Serial.println(counter);
  }
}

//state table lookup - could go into shrRotary.h
unsigned char process() {
  // Grab state of input pins.
  unsigned char pinstate = (unsigned char)(prt.pinState_set( ) & 0x03);
  // Determine new state from the pins and state table.
  state = ttable[state & 0xf][pinstate];
  // Return emit bits, ie the generated event.
  return state & 0x30;
}
