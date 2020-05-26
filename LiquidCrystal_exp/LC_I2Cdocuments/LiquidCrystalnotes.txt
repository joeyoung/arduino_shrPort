LiquidCrystal_I2C notes. May 24, 2020

The .cpp and .h files in this folder are from the 'version 2.0' file d/l
from author's website, but with repaired return value in write( ). This fault
(as with version 1.0 it was return 0, or no return) causes the object using the
library to only output the first character of values printed. With say,
lcd.print( 58 ), only the 5 is o/p. Earlier arduino stream implementation could 
use void write(...), but now it expects a return and 0 maybe means an error, or
don't send more or something.

This corrected version thus works with all of the sketches that #include 
LiquidCrystal_I2C. However, it has two defects for further use with 
LiquidCrystal_exp (see repo arduino_shrPort):
 * The operation of the I2C port is all handled by one function, expanderWrite,
   however it has a last-minute inclusion of the backlight state. To be able to
   pass this one-byte write along to a different I/O, the backlight state needs
   to be included before calling expanderWrite. There are only 3 or 4 higher
   functions that call expanderWrite so including the backlight state in those
   calls fixes this problem.
 * So that a bridging function to an I/O port object can override it,
   expanderWrite has to be declared virtual in the LiquidCrystal.h file.

These two changes are made in the version currently in libraries. They do not 
harm the use of LiquidCrystal_I2C as it was originally intended, so this 
modified library can replace the original and it then is also workable with
LiquidCrystal_exp and the various shrPorts.



   

