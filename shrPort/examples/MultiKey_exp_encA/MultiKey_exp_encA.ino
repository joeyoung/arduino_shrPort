/* Multikey_exp_encA - shrPort conversion of Keypad's Multikey for rotary encoder
:: G. D. (Joe) Young Feb 12, 2013
:: MC23017 version - GDY May 19, 2014
|| shrPort version - GDY May 29, 2020
|| rotary encoder  - GDY May 30, 2020
|| add getKey-like - GDY May 31, 2020
||
|| The rotary encoder common is driven by a column signal, the quadrature signals
|| connect to two rows so that the encoder scans as key A and B in this example.
|| The push switch connects to produce key C.
||
|| Multikey is needed because half of the detent positions of the encoder has both
|| outputs switched to common, a condition which is handled by MultiKey. The switch
|| on key state detects direction by maintaining a pair of boolean kA and kB that
|| corrrespond to the state of the 'key' - true when pressed, false when released.
|| Then direction 'up' rotation is given by whether kA is true (or false) when kB
|| changes to true (or false). Vice versa for down direction detection.
||
|| The remaining keys of the key pad can still function as usual - MultiKey_exp_encA
|| Not quite: if the encoder stops on both closed, then two rows are connected and
|| only first key encountered is reported. Best to have the encoder declared as its
|| own 1 X 3 keypad, not part of a keypad matrix.
||
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
int enccount, prevcount;

void loop() {

  char myKey = NO_KEY;


  // Fills kpd.key[ ] array with up-to 10 active keys.
  // Returns true if there are ANY active keys.
  if (kpd.getKeys())
  {
    bool kA, kB;
    for (int i=0; i<LIST_MAX; i++)   // Scan the whole key list.
    {
      if ( kpd.key[i].stateChanged )   // Only find keys that have changed state.
      {
        switch (kpd.key[i].kstate) {  // Report active key state : IDLE, PRESSED, HOLD, or RELEASED
            case PRESSED:
//                msg = " PRESSED.";
                if( kpd.key[i].kchar == 'B' ) {    // rotary encoder direction detection
                  if( kA ) enccount++;           //msg = " up";
                  kB = true;
                }
                if( kpd.key[i].kchar == 'A' ) {
                  if( kB ) enccount--;            //msg = " dn";
                  kA = true;
                }               // end encoder detection on make
                if( kpd.key[i].kchar != 'A' && kpd.key[i].kchar != 'B' ) myKey = kpd.key[i].kchar;
                break;
            case RELEASED:
//                msg = " RELEASED.";
                if( kpd.key[i].kchar == 'B' ) {    // rotary encoder direction detection
                  if( !kA ) enccount++;            //msg = " up";
                  kB = false;
                }
                if( kpd.key[i].kchar == 'A' ) {
                  if( !kB ) enccount--;            //msg = " dn";
                  kA = false;
                }               // end encoder detection on break
                break;
        }
 //       Serial.print("Key ");
 //       Serial.print(kpd.key[i].kchar);
 //        if( kpd.key[i].kchar == 'A' || kpd.key[i].kchar == 'B' ) Serial.println(msg);
      }
    }
  }

  if(enccount != prevcount ) {
    prevcount = enccount;
    Serial.println( enccount, DEC );
  }
  if( myKey ) {               // see if scanning found non-encoder key
    Serial.println( myKey );
  }
  
}  // End loop
