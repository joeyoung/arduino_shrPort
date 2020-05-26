// flashPin_shr.cpp - I2C i/o (in shrPort) version of flashPin arduino library

// created: Jan 19, 2014  G. D. (Joe) Young <jyoung@islandnet.com>

// revised: Jan 21/14 - cleanups

// Uses overriding of pin_mode and pin_write i/o functions in flashPin so that the
// flashPin operation can be simply accomplished on a I2C port expander (PCF8574,
// or PCF8575). Implementation is very similar to Keypad_I2C library.


#include "flashPin_shr.h"

void flashPin_shr::pin_write(byte pinNum, boolean level) {

	bport.pin_write( pinNum, level );

} // pin_write( )



