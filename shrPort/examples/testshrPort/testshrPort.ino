// test shrPort

#include <shrPort.h>
#include <Wire.h>


shrPort port1( 0x20, PCF8574 );

shrPort port2( 0x21, PCF8575 );

byte state1;
word state2;
unsigned long sectick;


void setup( ) {
  
  Wire.begin( );
  Serial.begin( 9600 );
  
  state1 = port1.pinState_set( );
  state2 = port2.pinState_set( );
  
  port1.pin_mode( 7, OUTPUT );
  port2.pin_mode( 15, OUTPUT );
  port2.pin_mode( 7, OUTPUT );
  
  Serial.println( state1, HEX );
  Serial.println( state2, HEX );
  
  sectick = millis( ) + 1000;
}

boolean tog = HIGH;

void loop( ) {
  if( sectick < millis( ) ) {
    sectick += 1000;
    tog = !tog;
    port1.pin_write( 7, !port1.pin_read( 7 ) );
    port2.pin_write( 15, !port2.pin_read( 15 ) );
    port2.pin_write( 7, !port2.pin_read( 7 ) );
    state1 = port1.pinState_set( );
    state2 = port2.pinState_set( );
    Serial.println( state1, HEX );
    Serial.println( state2, HEX );
  }
  
}

