/*
 * Example using the Rotary library, dumping integers to the serial
 * port. The integers increment or decrement depending on the direction
 * of rotation.
 *
 * This example uses polling rather than interrupts.
 * 
 * June 10/20 - rotaryp_exp convert poll to use shrPort via Rotary_exp
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
int counter = 0;

void setup() {
  Serial.begin(57600);
  Wire.begin( );
  rotary.begin( );
}

void loop() {
  unsigned char result = rotary.process();
  if (result == DIR_CW) {
    counter++;
    Serial.println(counter);
  } else if (result == DIR_CCW) {
    counter--;
    Serial.println(counter);
  }
}
