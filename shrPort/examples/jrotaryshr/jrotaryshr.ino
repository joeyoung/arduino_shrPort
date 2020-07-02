// jrotaryshr - another rotary encoder implementation
//
// started: June 4, 2020  G. D. (Joe) Young <jyoung@islandnet.com>
//
// revised: Jun 22/20 - attempt debouncing - state read after debounce time
//          Jun 23/20 - int on both, switch on prev<<2 | current, ignore invalid
//          Jun 23/20 - jrotaryshr - use shrPort for i/o
//
// Interrupt on encoder A (or B) edges, using pin-change interrupt library, then
// read both A and B to determine direction.
//
// For Bournes PEC11 with one complete cycle per detent, use one edge (FALLING)
// interrupt. For the 'other' encoder with 1/2 cycle per detent, use both (CHANGE)
// edges to interrupt.
//
#include "PinChangeInterrupt.h"
#include "shrPort.h"

#define pinI 7    // PCINT from PCF8574
#define pinA 1    // A encoder channel input on PCF8576
#define pinB 0    // B encoder channel input
#define pinC 2    // C (switch) channel

volatile uint8_t ABinput;
volatile uint8_t ABcopy, ABprev;
volatile int8_t enccount;
volatile bool intflag;
volatile uint32_t now, debint=3;

shrPort prt( 0x38, PCF8574 );

void readAB( ) {
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));   //activity indicator
  ABprev = ABinput<<2;
//  ABinput = digitalRead( pinA ) << 1;
//  ABinput |= digitalRead( pinB );
    ABinput = prt.pinState_set( ) & 0x3;     //clears pin change int from PCF8574
  ABprev |= ABinput;
  switch( ABprev ) {
    case 1:         //00 01   'forward' sequence of states
    case 7:         //01 11
    case 0xE:       //11 10
    case 8:         //10 00
      enccount++;
      break;
    case 2:         //00 10   'reverse' sequence
    case 0xB:       //10 11
    case 0xD:       //11 01
    case 4:         //01 00
      enccount--;
      break;
    default:
      // ignore
      break;
  }
}

void intonchange( ) {
  intflag = true;
  now = millis( );
}

void setup() {
  Serial.begin( 115200 );
  if( !Serial ) {/* wait */}
  Wire.begin( );
  prt.begin( );
  prt.pin_mode( pinA, INPUT );
  prt.pin_mode( pinB, INPUT );
  pinMode( LED_BUILTIN, OUTPUT );
  pinMode( pinI, INPUT_PULLUP );
  attachPCINT(digitalPinToPCINT( pinI ), intonchange, FALLING );
}

void loop() {
  if( intflag ) {
    if( millis( ) - now > debint ) {  //wait debint msec, ignore ints while waiting
      readAB( );
      intflag = false;            // pay attention to ints again
      Serial.println( enccount ); //note that ignored ints will print (unchanged) count
    }
  }

}
