// test I2C flashPin using port sharing library

// January 19, 2014 G. D. (Joe) Young <jyoung@islandnet.com>

#include <Wire.h>
#include <flashPin.h>
#include <shrPort.h>
#include <flashPin_shr.h>


shrPort port1( 0x20, PCF8574 );  // 8-bit I2C port
shrPort port2( 0x21, PCF8575 );  // 16-bit I2C port

flashPin_shr bit07( 7, HIGH, 500, port1 );
flashPin_shr bit7( 7, LOW, 200, port2 );
flashPin_shr bit15( 15, HIGH, 500, port2 );

unsigned long sectick;

void setup( ) {
  Wire.begin( );
//  port1.pin_mode( 7, OUTPUT );
//  port2.pin_mode( 7, OUTPUT );
//  port2.pin_mode( 15, OUTPUT );
  sectick = millis( ) + 1000;
}




void loop( ) {
  
  if( sectick < millis( ) ) {
    sectick += 1000;
    bit07.flash( );
    bit7.flash( );
    bit15.flash( );
  } // if second
  
  bit07.flashOff( );
  bit7.flashOff( );
  bit15.flashOff( );
  
} // loop( )






