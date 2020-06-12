### arduino_shrPort

This repository contains several separate arduino format libraries, which enable
sharing of I2C port expander IC. These port expander devices connect to the I2C
bus (pins SCL, SDA, VCC, GND) and then provide 8 or 16 digital I/O bits and 
freeing the arduino digital pins for other uses.

The current version (2020) replaces an earlier start on this I/O sharing capability
that was abandonded because it required modification of other libraries, rendering
the modified versions incompatible with the base arduino. Some of the earlier files
are retained in the `history` folder

To use the library functions, either download the individual library folders 
needed, or the whole repository .zip file and unpack to a temporary folder and
place the libraries you need into the arduino libraries folder within sketchbook.
The arduino library manager doesn't work directly on this whole repository.

The libraries included here are of three different types - expander port sharing
I/O, bridging functions from arduino pin programs to the share ports, and
modified existing libraries. The expander port sharing libraries are:

* `shrPort` - handling PCF8574 and PCF8575 devices (8 and 16 bit)
* `shrPrta` - PCA9554/5 PCA9534/5/9 TCA6408/16 register compatible 8 and 16 bit 
* `shrMC17` - handling MCP23017 16-bit
* `shrMC16` - handling MCP23016 16-bit
* `shrMC08` - handling MCP23008 8-bit
* `I2Ccomm` - an empty definition class used by `shrXXXX` libraries

The bridging functions are:

* `Keypad_exp` - allow Keypad to use I2C port expander pins
* `LiquidCrystal_exp` - extends (modified) LiquidCrystal_I2C
* `flashPin_exp` - non-blocking timers to repeatedly pulse individual pins
* `LcdBarCentreZero_exp` - uses LiquidCrystal_exp to draw bargraph on character displays
* `Rotary_exp` - for rotary encoders connected to expander pins

The modified existing libraries:

* `LiquidCrystal_I2C` - see README document in this version
* `Rotary` - see README in this version

The purpose of this library development is similar to that of other libraries posted
 elsewhere that allow I2C I/O  - for example
 [`arduino_keypads`](https://github.com/joeyoung/arduino_keypads). The additional goal
compared to those previous developments was to separate the I/O port control from
the objects using the port. This arrangement has the minor advantage of not duplicating
the code controlling the port when multiple objects are instantiated, as is the case
with the libraries in `arduino_keypads`. More importantly, only one set of I/O
functions controls the state of the I/O port chip registers thus minimizing the
possibility of different objects interfering with one another.

Excerpts from an example sketch 'kpdispf5' illustrate how to use these libraries. See
also the API document which describes the functions made available with the libraries.
The example system has a Keypad, 16 X 2 LCD display and flashPin to pulse the
display backlight. Not shown in the excerpt, the sketch gets keys from the keypad,
prints them to the lcd and Serial, and pulses the backlight when the lcd line is filled.

```cpp

    #include <shrMC17.h> 
    #include <Keypad_exp.h>
    #include <LiquidCrystal_exp.h>
    #include <flashPin_exp.h>
    #include <Wire.h>
    #define I2CADDR 0x24    //MCP23017
      ....
    TwoWire * jwire;    // to test passing reference to an alternate I2C port (MKR ZERO, eg)
    shrMC17 prt( I2CADDR, MCP23017, jwire );   //create shrPort object with MCP expander
    Keypad_exp kpd( &prt, makeKeymap(keys), rowPins, colPins, ROWS, COLS); //Keypad initializers
    flashPin_exp pin7( &prt, 7, LOW, 250);     //create flashPin
    LiquidCrystal_exp  lcd( &prt, 16, 2 );     //LiquidCrystal_exp 16 col, 2 rows
      ....
	void setup() {
		Serial.begin( 9600 );
		while( !Serial ){ /*wait*/ }
		jwire->begin( );
		prt.begin( );
		kpd.begin( );
		for( byte ix=0; ix < 8; ix++ ) {  //lcd assumes its port is set to output
			prt.pin_mode( ix, OUTPUT );
		}
		lcd.init( );               //lcd start
		lcd.begin( 16, 2 );			
		lcd.print( "Hi Joe exp" ); //lcd operations
		lcd.setCursor( 0, 1 );
		lcd.backlight( );
			....
```



