/* Multikey_exp - shrPort conversion of Keypad's Multikey
:: G. D. (Joe) Young Feb 12, 2013
:: MC23017 version - GDY May 19, 2014
|| shrPort version - GDY May 29, 2020
::
|| @file MultiKey.ino
|| @version 1.0
|| @author Mark Stanley
|| @contact mstanley@technologist.com
||
|| @description
|| | The latest version, 3.0, of the keypad library supports up to 10
|| | active keys all being pressed at the same time. This sketch is an
|| | example of how you can get multiple key presses from a keypad or
|| | keyboard.
|| #
*/

#include <shrPort.h>        //I2C i/o port control library
#include <Keypad.h>
#include <Keypad_exp.h>     // I2C i/o bridge library for Keypad
#include <Wire.h> 

#define I2CADDR 0x38        // address of PCF8574 chip on I2C bus

const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {0, 1, 2, 3}; //connect to the row pinouts of the kpd
byte colPins[COLS] = {4,5,6,7}; //connect to the column pinouts of the kpd

shrPort prt( I2CADDR, PCF8574 );

// add pointer to port to modify constructor for I2C i/o
Keypad_exp kpd( &prt, makeKeymap(keys), rowPins, colPins, ROWS, COLS );



void setup() {
  Serial.begin(9600);
  while( !Serial ){/*wait*/}   //for USB serial switching boards
  Wire.begin( );
  prt.begin( );
  kpd.setDebounceTime(1);
  Serial.print( "start with pinState = " );
  Serial.println( prt.pinState_set( ), HEX );
}

unsigned long loopCount = 0;
unsigned long startTime = millis();
String msg = "";



void loop() {

  loopCount++;
  if ( (millis()-startTime)>1000 ) {
//      Serial.println(loopCount);
      startTime = millis();
      loopCount = 0;
  }

  // Fills kpd.key[ ] array with up-to 10 active keys.
  // Returns true if there are ANY active keys.
  if (kpd.getKeys())
  {
    for (int i=0; i<LIST_MAX; i++)   // Scan the whole key list.
    {
      if ( kpd.key[i].stateChanged )   // Only find keys that have changed state.
      {
        switch (kpd.key[i].kstate) {  // Report active key state : IDLE, PRESSED, HOLD, or RELEASED
            case PRESSED:
                msg = " PRESSED.";
                break;
            case HOLD:
                msg = " HOLD.";
                break;
            case RELEASED:
                msg = " RELEASED.";
                break;
            case IDLE:
                msg = " IDLE.";
        }
        Serial.print("Key ");
        Serial.print(kpd.key[i].kchar);
        Serial.println(msg);
      }
    }
  }
}  // End loop
