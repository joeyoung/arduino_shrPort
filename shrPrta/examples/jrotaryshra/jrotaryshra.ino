// jrotaryshr - another rotary encoder implementation
//
// started: June 4, 2020  G. D. (Joe) Young <jyoung@islandnet.com>
//
// revised: Jun 22/20 - attempt debouncing - state read after debounce time
//          Jun 23/20 - int on both, switch on prev<<2 | current, ignore invalid
//          Jun 23/20 - jrotaryshr - use shrPort for i/o
//          Jul  7/20 - jrotaryshra - PCA9555, enc on bits 8, 9, use new reg_read/write
//
// Interrupt on encoder A (or B) edges, using pin-change interrupt library, then
// read both A and B to determine direction.
//
// For Bournes PEC11 with one complete cycle per detent, use one edge (FALLING)
// interrupt. For the 'other' encoder with 1/2 cycle per detent, use both (CHANGE)
// edges to interrupt.
//
// For jrotaryshra, interrupt from port comes on all edges for both A and B, int to arduino
// is falling edge or the int out from the port. All edges cause read/analysis so the PEC11
// gives 4 counts per detent.
//
#include <PinChangeInterrupt.h>
#include <shrPrta.h>

#define pinI 7    // PCINT from PCA9555
#define pinA 8    // A encoder channel input on PCA9555
#define pinB 9    // B encoder channel input
#define pinC 10   // C (switch) channel

volatile uint8_t ABinput;
volatile uint8_t ABcopy, ABprev;
volatile int8_t enccount;
volatile bool intflag;
volatile uint32_t now, debint=3;

int junk;

shrPrta prt( 0x20, PCA9555 );

void readAB( ) {
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));   //activity indicator
  ABprev = ABinput<<2;                                 //keep previous state
  ABinput = highByte( prt.pinState_set( ) ) & 0x3;     //clears pin change int from PCF8574
  ABprev |= ABinput;                                //combine with current for lookup
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
} //readAB( )

void intonchange( ) {
  intflag = true;           //signal to loop for port read, int clear
  now = millis( );
}

void setup() {
  Serial.begin( 115200 );
  if( !Serial ) {/* wait */}
  Wire.begin( );
  prt.begin( );
  prt.pin_mode( pinA, INPUT );
  prt.pin_mode( pinB, INPUT );
  prt.reg_write( VREG, bit(pinA) );        //invert polarity, changes direction of rotation
  Serial.println( prt.reg_read( VREG ), HEX );    //check setups
  Serial.println( prt.reg_read( IODIR ), HEX );
  pinMode( LED_BUILTIN, OUTPUT );
  pinMode( pinI, INPUT_PULLUP );
  attachPCINT(digitalPinToPCINT( pinI ), intonchange, FALLING );
} // setup( )

void loop() {
  if( intflag ) {
    if( millis( ) - now > debint ) {  //wait debint msec, ignore ints while waiting
      readAB( );
      intflag = false;            // pay attention to ints again
      Serial.println( enccount ); //note that ignored ints will print (unchanged) count
//      Serial.println( prt.pinState_set( ), HEX );
    }
  }

} //loop( )
