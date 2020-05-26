Notes for LcdBarCentreZero_exp library

This version of the bar graph drawing library is only minimally modified from the
original enhancement of the LiquidCrystal_I2C library to permit using the shrPort
functions for I2C i/o. The shrPort revisions are introduced to disconnect the I2C
device operation from the extended libraries that use them - Keypad, LiquidCrystal,
flashPin, and so on. This way then permits the object's construction for these
functions to be nearly the same as they are for when the objects use the arduino
pins directly. Also, sharing the pins of one expander chip among several functions
tends to be cleaner and not have interaction among the functions. So, for example,
a system with a Keypad and an LCD display can be connected to share one 16-bit
port expander IC.

In the examples, the versions of the bargraph drawing sketches that originally used
the LiquidCrystal_I2C library are left in the folder so that the differences with
the _exp versions can be studied. They are minor:
 * #include <shrPort.h>
 * construct a prt object: shrPort prt( i2caddress, port type )
 * use LiquidCrystal_exp.h>   (not the _I2C version)
   to construct lcd with &prt instead of the I2C address
 * Add to setup( ):
   Wire.begin( );
   prt.begin( );
   set pins used for lcd to OUTPUT

Finally, the LiquidCrystal_I2C library version 2.0 as d/l from the Mario H. web
site:
  http://www.xs4all.nl/~hmario/arduino/LiquidCrystal_I2C/LiquidCrystal_I2C.zip
requires a couple of modifications:
 * The return value from the function LiquidCrystal_I2C::write(uint8_t value)
   must be 1, not 0. Otherwise only single character is o/p with lcd.print( ).
 * The last-minute inclusion of _backlight value in the expanderWrite( ) must
   be fixed by including it in all the functions that call expanderWrite.
 * In the header file, the declaration of expanderWrite has the added virtual
   characteristic. This permits re-defining expanderWrite in the bridging
   object to be constructed with LiquidCrystal_exp. This bridging library allows
   for shrPort to specify other i2c port expanders than the PCF8574 8-bit port
   which was originally used the the LiquidCrystal_I2C lib.


