// HelloWorld example revised to use shrPort I2C library
// Jan 24, 2014  G. D. (Joe) Young <jyoung@islandnet.com>
// Jul 10/20 HelloWorld_lcdshrTC - using LiquidCrystal_exp, shrPrta, TCA6408, MKRZERO. See
//                                 mkrTCA6408.pdf schematic for TCA level shifter hookup

#include <Wire.h> 
#include <LiquidCrystal_exp.h>
#include <shrPrta.h>

shrPrta port0( 0x21, TCA6408 );

LiquidCrystal_exp lcd(&port0,16,2);  // set the LCD address to 0x20 for a 16 chars and 2 line display
//LiquidCrystal_I2C lcd(0x20,16,2);  // set the LCD address to 0x20 for a 16 chars and 2 line display

void setup()
{
  Wire.begin( );
  for( byte i=0; i<8; i++ ) port0.pin_mode( i, OUTPUT );
  lcd.init();                      // initialize the lcd 
 
  // Print a message to the LCD.
  lcd.backlight();
  lcd.print("Hello Joe!");
  lcd.setCursor( 0, 1 );
  lcd.print( "second line too" );
}

void loop()
{
}
