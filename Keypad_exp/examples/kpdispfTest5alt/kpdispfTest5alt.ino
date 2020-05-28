// kpdispfTest5-- separate objects with I2C i/o - keypad, lcd, flashPin
//
// created: May 14, 2020   G. D. (Joe) Young <jyoung@islandnet.com>
//
// revised: May 16/20 - test _exp port sharing method - kpdispfTest3
//          May 18/20 - Keypad_exp error fixed
//          May 19/20 - using shrPrtMC17 library
//          May 22/20 - renamed shrMC17, use I2Ccomm base to allow differen libs
//          May 23/20 - both shrPort and shrMC17
//          May 27/20 - test with MKR ZERO, alternate I2C port
//
// This test file is to compare the memory size and port interaction with
// kpdispfTest. This case has each of Keypad, LiquidCrystal, and flashPin separate
// objects specifying the same i2c port. Memory size for kpdispf 6590, cf this one
// size 6748. The combined version allows both keypad scan and flashPin to use 
// pin15 without obvious interference. Whereas, the separate objects version scanning
// diables the flashPin on 15. Going to the 4X3 keypad frees pin 15 and all works.
//
// kpdispfTest3 using the _exp libraries and shrPort allows creating separate objects
// for kpd, lcd, and flashPin as for the kpdispfTest2, but now only one instance of
// the port is used. Size 6458 with 4X3. 6460 wit 4X4 pin15 flashes OK, level dim with
// scanning same. With shrPort direction control added back, 4X3, 6752 bytes.
//
// kpdispfTest4 using the MC17 version of shrPort, with the i/o object name created by
// shrPort prt( ... ). The _exp libraries all want the prt specification to be of
// type shrPort *, so can't have unique library names for each i/o type??? Memory size
// of kpdispfTest4 in 7220 bytes.
//
// May 22/20 With each shrPort library inheriting the abstract I2Ccomm, all their port
// objects can be referenced as I2Ccomm *, whether shrPort, shrMC17, shrMC08, etc.
// Size is now 7308 4X3, 7310 4X4. With MC17 lib, 4X4 scanning foils flashPin on 15.
//
// May 23/20 both shrMC17 and shrPort with keypads, MC17 with LCD and flashPin. memory
// size 8436

#include <shrMC17.h>
#include <shrPort.h>
#include <Keypad_exp.h>
#include <LiquidCrystal_exp.h>
#include <flashPin_exp.h>
#include <Wire.h>

#include "wiring_private.h"
TwoWire jwire(&sercom3, 0, 1);   // Create the new wire instance assigning it to sda 0 scl 1

#define I2CADDR 0x24    //MCP23017
#define I2CADDR2 0x38   //PCF8574 - 2nd keypad

const byte ROWS = 4; //four rows
const byte COLS = 4; //three (four) columns - note flashPin on col 4

char keys[ROWS][COLS] = {         // the 4X4 keypad with pin15 included in
  {'1','2','3','A'},              // keypad scanning 'disables' the flashPin on pin 15
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
#if 0
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
#endif
// Digitran keypad, bit numbers of PCF8575 i/o port
byte rowPins[ROWS] = {8, 9, 10, 11}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {12, 13, 14, 15}; //connect to the column pinouts of the keypad
byte rowPins2[ROWS] = {0, 1, 2, 3}; //connect to the row pinouts of the keypad
byte colPins2[COLS] = {4, 5, 6, 7}; //connect to the column pinouts of the keypad


//TwoWire *jwire = &Wire;   //test passing pointer to shrPort lib
shrPort prt2( I2CADDR2, PCF8574, &jwire );
shrMC17 prt( I2CADDR, MCP23017, &jwire );
Keypad_exp kpd( &prt, makeKeymap(keys), rowPins, colPins, ROWS, COLS); //Keypad initializers
flashPin_exp pin15( &prt, 7, LOW, 250);                    //flashPin                
LiquidCrystal_exp  lcd( &prt, 16, 2 );  //LiquidCrystal_exp
Keypad_exp kpd2( &prt2, makeKeymap(keys), rowPins2, colPins2, ROWS, COLS); // 2nd keypad

byte ccount = 0;

void setup() {
  Serial.begin( 9600 );
  while( !Serial ){ /*wait*/ }

  jwire.begin( );
  pinPeripheral(0, PIO_SERCOM);   //Assign SDA function to pin 0
  pinPeripheral(1, PIO_SERCOM);   //Assign SCL function to pin 1

  prt.begin( );
  prt2.begin( );
  kpd.begin( );
  kpd2.begin( );
  for( byte ix=0; ix < 8; ix++ ) {  //lcd assumes its port is set to output
    prt.pin_mode( ix, OUTPUT );
  }
//  prt.pin_mode( 15, OUTPUT );
//  prt.pin_write( 15, LOW );    //This doesn't hurt scanning, but scanning dims LED
  lcd.init( );                 //lcd start
  lcd.begin( 16, 2 );
//  lcd.clear( );
  lcd.print( "Hi Joe exp" );
  lcd.setCursor( 0, 1 );
  lcd.backlight( );
  Serial.print( "kpdispfTest. Initial pinstate: ");
  Serial.println( prt.pinState_set( ), HEX );
}

void loop() {
//  pin15.flashRep(2);
  pin15.flashOff( );

  char key = kpd.getKey( );
  char key2 = kpd2.getKey( );
  if( key ) {
    Serial.print( key );
    Serial.print( ' ' );
    lcd.print( key );
    lcd.print( ' ' );
    ccount += 2;
    if( ccount >= 16 ){
      ccount = 0;
      lcd.setCursor( 0, 1 );
      Serial.println( "" );
      pin15.flashRep( 3 );     //does not interfere with scanning
    }
  }
  if( key2 ) {
    Serial.print( key2 );
    Serial.print( ' ' );
    lcd.print( key2 );
    lcd.print( ' ' );
    ccount += 2;
    if( ccount >= 16 ){
      ccount = 0;
      lcd.setCursor( 0, 1 );
      Serial.println( "" );
      pin15.flashRep( 3 );     //does not interfere with scanning
    }
  }

}
