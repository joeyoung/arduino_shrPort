// flashPin.cpp - flash arduino pin using non-blocking timing

// created: Oct 12, 2013 G. D. (Joe) Young

// revised: Jan 14/14 - add specification of resting level
//          Jan 15/14 - virtual functions for i/o
//          Jan 16/14 - fp_set function added



#ifndef flashPin_h
#define flashPin_h

#include "Arduino.h"

class flashPin
{
public:
	// constructor - allow default setting of pin, resting level, and duration
    flashPin( byte pin = 13, bool restLevel = LOW, unsigned long duration = 250 );

	// i/o wrapper functions to permit overloading to I2C ports
	virtual void pin_mode(byte pinNum, byte mode) { pinMode(pinNum, mode); }
	virtual void pin_write(byte pinNum, boolean level) { digitalWrite(pinNum, level); }

	//member functions
	void flash( unsigned long duration );
	void flash( );
	void fp_set( bool restLevel );

	void flashOff( );

private:
	byte fpin;
	bool fstate;
	bool frest;
	unsigned long fend, fdur;

}; // class flashPin

#endif
