// shrRotary17 - Rotary encoder using shrPort with MCP23017
//
// started: June 15, 2020  - G. D. (Joe) Young <jyoung@islandnet.com>
//
// revised: June 16/20 - MCP23017 example
//          Jun 17/20 - adjacent pin shifting for flexible pin assignment.
//
// Using the state table and lookup method from the Rotary arduino library:
// Copyright 2011 Ben Buxton. Licenced under the GNU GPL Version 3.
// Contact: bb@cactii.net
//
// In process( ), the two bits used for the encoder are assumed to be the port's 0 and 1.
// As the MCP23017 is a 16-bit port, choice of bits to use could be greater and process
// would need to be adjusted accordingly. To illustrate, below the encoder is put on two
// adjacent bits of the upper 8-bits. The mask step in process then use the highByte( ) 
// function, plus a shift to bring them into the unsigned char used for state.
//
#define HALF_STEP       //use output/half-step table. Needs to preceed shrRotary.h
#include "shrRotary.h"
#include <shrMC17.h>


shrMC17 prt( 0x25, MCP23017 );
uint8_t state = 0;
int8_t counter;

const uint8_t pinA = 2, pinB = pinA+1;    //assign adjacent pins for A, B inputs

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
  unsigned char pinstate = (unsigned char)((highByte(prt.pinState_set( ))>>pinA) & 0x03);
  // Determine new state from the pins and state table.
  state = ttable[state & 0xf][pinstate];
  // Return emit bits, ie the generated event.
  return state & 0x30;
}
