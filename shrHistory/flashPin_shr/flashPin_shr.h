// flashPin_shr.h - I2C i/o (in shrPort) version of flashPin arduino library

// created: Jan 19, 2014  G. D. (Joe) Young <jyoung@islandnet.com>

// revised: Jan 21/14 - cleanups
//          Jan 22/14 - construct pin as output--mode kept up in shrPort now

// Uses overiding pin_mode and pin_write i/o functions in flashPin so that the
// flashPin operation can be simply accomplished on a I2C port expander (PCF8574,
// or PCF8575). Implementation is very similar to Keypad_I2C library.

#ifndef FLASHPIN_SHR
#define FLASHPIN_SHR

#include "flashPin.h"
#include "shrPort.h"

class flashPin_shr : public flashPin {

public:
	flashPin_shr( byte bitNr, bool restLevel, unsigned long duration, shrPort &spport )
		 : flashPin( bitNr, restLevel, duration ), bport(spport){ bport.pin_mode( bitNr, OUTPUT ); }

	void pin_mode(byte pinNum, byte mode) {}	// don't use, except in constructor
	void pin_write(byte pinNum, boolean level);
	bool pin_read( byte pinNum ) { }			// don't use pin_read

private:

	shrPort &bport;

}; // class

#endif


