/* right and left only bars - test LcdBarCentreZero library
 *
 * Created: Sept 20, 2011 - G. D. Young
 *
 * Revised: Oct 21/11 - test with I2C version of LCD library
 *
 * Draws 4 bar graphs in corners of 16 col 2 line display
 */


#include <Wire.h>
#include <shrPort.h>
#include <LiquidCrystal_exp.h>
#include <LcdBarCentreZero_exp.h>

#define MAXBARS 20        //length of bar to draw

shrPort prt( 0x20, PCF8575 );
LiquidCrystal_exp lcd( &prt, 16, 2 );  // I2C LCD instance

LcdBarCentreZero_exp zcb( &lcd );     // create bar graph instance


byte lcdNumCols = 16;     // -- number of columns in the LCD
int barCount = 0;         // -- value to plot for this example
byte up = 1;

long interval = 150;      // interval at which to inc bar (milliseconds)
long previousMillis;
long randNr;


void setup( ){
  Wire.begin( );
  prt.begin( );
  for( byte ix=0; ix<8; ix++ ) prt.pin_mode( ix, OUTPUT );
  lcd.begin( 2, lcdNumCols );
  lcd.init( );                 // start up I2C, LCD 
  zcb.loadCG( );               // load bargraph characters
  lcd.clear( );
  previousMillis = millis( );    // init interval timer for drawing
}

void loop( )
{
    if( millis( ) - previousMillis > interval ) {
      // save the last time you counted
      previousMillis = millis( );   

      // count up the number of bars to draw
      if( up ) {
        barCount += 1;
        if( barCount > MAXBARS ) {
          up = 0;
          barCount = MAXBARS;
        }
      } else {              // OR, count them down
        barCount -= 1;
        if( barCount < -MAXBARS ) {
          up = 1;
          barCount = -MAXBARS;
        }
      }
      randNr = random( -MAXBARS, 1 );    // use random number to show blanking
      zcb.drawBar( abs( barCount ), MAXBARS, 0, 0 );
      zcb.drawBar( (int)randNr, MAXBARS, 15, 0 );
      zcb.drawBar( -abs( barCount ), MAXBARS, 15, 1 );    //short bar to show clipping to max
      zcb.drawBar( (int)-randNr, 8, 0, 1 );
      
    } // if time to count
}
