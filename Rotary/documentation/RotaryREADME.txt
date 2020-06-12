Notes regarding modifications to original Rotary arduino library:

Copyright 2011 Ben Buxton. Licenced under the GNU GPL Version 3.
Contact: bb@cactii.net

June 11, 2020  -- G. D. (Joe) Young <jyoung@islandnet.com>

The original library is included in this documentation folder. The modified
files Rotary.h and Rotary.cpp still work as the original did. The modifications
consist of replacing the arduino digitalRead, digitalWrite, and pinMode with
virtual functions pin_read, pin_write, and pin_mode so that they can be
replaced with derived classes that do similar digital operations on the pins
of I2C bus port expander IC.

The example sketches rotaryp_exp and rotaryi_exp in the Rotary_exp library
folder are the sketches poll and interrupt respectively from the original 
library but altered to use the I2C port expander devices as described in github
repo arduino_shrPort.


