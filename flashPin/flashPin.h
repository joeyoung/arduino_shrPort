// flashPin.cpp - flash arduino pin using non-blocking timing

// created: Oct 12, 2013 Copyright(C) G. D. (Joe) Young <jyoung@islandnet.com>

// revised: Jan 14/14 - add specification of resting level
//          Jan 15/14 - virtual functions for i/o
//          Jan 16/14 - fp_set function added
//          Apr 24/20 - version 1.0.1 - Library formatting
//          Apr 25/20 - version 1.1.0 - flashRep added
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <https://www.gnu.org/licenses/>.



#ifndef flashPin_h
#define flashPin_h

#include "Arduino.h"
#include "I2Ccomm.h"

class flashPin //: public I2Ccomm
{
public:
	// constructor - allow default setting of pin, resting level, and duration
    flashPin( byte pin = 13, bool restLevel = LOW, unsigned long duration = 250 );

	// i/o wrapper functions to permit overloading to I2C ports
	virtual void pin_mode(byte pinNum, byte mode) { pinMode(pinNum, mode); }
	virtual void pin_write(byte pinNum, boolean level) { digitalWrite(pinNum, level); }
#if 0
	// in I2Ccomm, but not needed for flashPin
	virtual bool pin_read(uint8_t pinNum ) { }
	virtual void begin( ) { }	// this will get initial pin state, start wire library
	virtual word pinState_set( ) { }
	virtual void port_write( word i2cportval ) { }
#endif
	//member functions
	void flash( unsigned long duration );
	void flash( );
	void flashRep( char repeats );
	void fp_set( bool restLevel );

	void flashOff( );

private:
	byte fpin;
	bool fstate;
	bool frest;
	bool fon;
	bool ftiming;
	char frep;
	unsigned long fmark, fdur;

}; // class flashPin

#endif
