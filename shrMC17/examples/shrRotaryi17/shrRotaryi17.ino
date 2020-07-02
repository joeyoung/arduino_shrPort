// shrRotaryi17 - Rotary encoder using shrPort with MCP23017
//
// started: June 15, 2020  - G. D. (Joe) Young <jyoung@islandnet.com>
//
// revised: June 16/20 - MCP23017 example
//          Jun 17/20 - adjacent pin shifting for flexible pin assignment.
//          Jul  2/20 - interrupt version shrRotaryi17 for int setup example with MC17v1.5
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
volatile uint8_t state = 0;
volatile int8_t counter;
volatile bool iflag;

const uint8_t pinA = 2, pinB = pinA+1;    //assign adjacent pins for A, B inputs

void setup( ) {
  Serial.begin(57600);
  while( !Serial ){/* wait */}
  Wire.begin( );
  Wire.setClock( 400000 );        //speed up I2C bus for faster rotations
  prt.begin( );

//interrupt setup for MCP23017; tie interrupt o/p together, enable enc pins
  prt.reg_write( IOCON, prt.reg_read( IOCON ) | bit(MIRROR) | (bit(MIRROR)<<8) );
  prt.reg_write( GPINTENA, bit(pinA) | bit(pinB) );
  Serial.print( "IOCON " );                       //verify port setups
  Serial.println( prt.reg_read( IOCON ), HEX );
  Serial.print( "GPINTENA " );
  Serial.println( prt.reg_read( GPINTENA ), HEX );

//interrupt setup for arduino to respond to expander int
  pinMode( 2, INPUT_PULLUP );
  attachInterrupt(digitalPinToInterrupt(2), rotate, FALLING);
} //setup( )

void loop( ) {
  if( iflag ) {
    unsigned char result = process();
    if (result == DIR_CW) {
      counter++;
      Serial.println(counter);
    } else if (result == DIR_CCW) {
      counter--;
      Serial.println(counter);
    }
    iflag = false;
  }
  //should not happen - missed interrupt
  if( digitalRead( 2 ) == LOW ) prt.reg_read( GPIOA );  //clear missed int
} // loop( )

// rotate is called anytime the rotary inputs change state on the expander.
void rotate() {
  iflag = true;     //just indicate interrupt, can't do I2C i/o in int svc
}

//state table lookup - could go into shrRotary.h
unsigned char process() {
  // Grab state of input pins.
  unsigned char pinstate = (unsigned char)(((prt.pinState_set( ))>>pinA) & 0x03);
  // Determine new state from the pins and state table.
  state = ttable[state & 0xf][pinstate];
  // Return emit bits, ie the generated event.
  return state & 0x30;
}
