// kpdispfTest - one large object with I2C i/o inherits keypad, lcd, flashPin
//
// created: May 8, 2020   G. D. (Joe) Young <jyoung@islandnet.com>
//
// revised: May 14/20 - make keypad 4X4 to check possible flashPin interference
//

#include <kpdispf.h>
#include <Wire.h>
#define I2CADDR 0x20

const byte ROWS = 4; //four rows
const byte COLS = 4; //three (four) columns - note flashPin on col 4
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
// Digitran keypad, bit numbers of PCF8574 i/o port
byte rowPins[ROWS] = {8, 9, 10, 11}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {12, 13, 14, 15}; //connect to the column pinouts of the keypad

TwoWire *jwire = &Wire;   //test passing pointer to keypad lib
kpdispf kpdf( makeKeymap(keys), rowPins, colPins, ROWS, COLS, //Keypad initializers
              15, HIGH, 250,                                  //flashPin inits
              16, 2, I2CADDR, PCF8575, jwire );               //LiquidCrystal_I2C, then I2C port

byte ccount = 0;

void setup() {
  Serial.begin( 9600 );
  while( !Serial ){ /*wait*/ }
  jwire->begin( );
  kpdf.begin( );
//  kpdf.pin_write( 15, LOW );    //This doesn't hurt scanning, but scanning dims LED
  kpdf.init( );                 //lcd start
  kpdf.begin( 16, 2 );
  kpdf.print( "Hi Joe" );
  kpdf.setCursor( 0, 1 );
  kpdf.backlight( );
  Serial.print( "kpdispfTest. Initial pinstate: ");
  Serial.println( kpdf.pinState_set( ), HEX );
}

void loop() {
//  kpdf.flashRep(2);
  kpdf.flashOff( );
  char key = kpdf.getKey( );
  if( key ) {
    Serial.print( key );
    Serial.print( ' ' );
    kpdf.print( key );
    kpdf.print( ' ' );
    ccount += 2;
    if( ccount >= 16 ){
      ccount = 0;
      kpdf.setCursor( 0, 1 );
      Serial.println( "" );
      kpdf.flashRep( 3 );     //does not interfere with scanning
    }
  }
}
