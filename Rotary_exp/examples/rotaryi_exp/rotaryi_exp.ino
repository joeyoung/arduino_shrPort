/*
 * Example using the Rotary library, dumping integers to the serial
 * port. The integers increment or decrement depending on the direction
 * of rotation.
 *
 * This example uses interrupt from port expander IC to signal encoder movement.
 * 
 * June 10/20 - rotaryi_exp convert interrupt to use shrPort via Rotary_exp
 *            - Since cannot do I2C i/o in interrupt service, the processing of
 *              the encoder state read from the expander port is similar to the
 *              polled version of Rotary.
 */

#include <Rotary.h>
#include <Rotary_exp.h>
#include <shrPort.h>
#include <Wire.h>

shrPort prt( 0x38, PCF8574 );

// Rotary encoder is wired with the common to ground and the two
// outputs to pins 0 and 1 on PCF8574.
Rotary_exp rotary( &prt, 0, 1 );

// Counter that will be incremented or decremented by rotation.
volatile int counter = 0;
volatile bool iflag;      //set when each interrupt occurs

void setup() {
  Serial.begin(57600);
  Wire.begin( );
  prt.begin( );
  rotary.begin( );
  pinMode( 2, INPUT_PULLUP );
  attachInterrupt(digitalPinToInterrupt(2), rotate, FALLING);
}

void loop() {
  if( iflag ) {
    unsigned char result = rotary.process();
    if (result == DIR_CW) {
      counter++;
      Serial.println(counter);
    } else if (result == DIR_CCW) {
      counter--;
      Serial.println(counter);
    }
    iflag = false;
  }
}

// rotate is called anytime the rotary inputs change state on the expander.
void rotate() {
  iflag = true;     //just indicate interrupt, can't do I2C i/o in int svc
}
