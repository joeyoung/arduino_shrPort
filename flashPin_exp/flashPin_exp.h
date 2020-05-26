// flashPin_exp.h - definition of I2C expander version of flashPin arduino library
//
// created: May 16, 2020  Copyright (C) G. D. (Joe) Young <jyoung@islandnet.com>
//
// revised: 
//
// derived from flashPin_I2C at Apr 22/20. Attempting generic I2C i/o.
// Override the pin_mode and pin_write i/o functions in flashPin so that the
// flashPin operation can be simply accomplished on a I2C port expander (PCF8574,
// or PCF8575). Implementation is very similar to Keypad_I2C library.
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

#ifndef FLASHPIN_EXP
#define FLASHPIN_EXP

#include "flashPin.h"
#include "I2Ccomm.h"


class flashPin_exp : public flashPin {

public:
	flashPin_exp( I2Ccomm * exPort, byte bitNr, bool restLevel = LOW, unsigned long duration = 250 )
		  : flashPin( bitNr, restLevel, duration )
			{ _export = exPort; }

	void pin_mode(byte pinNum, byte mode);
	void pin_write(byte pinNum, boolean level);

	void begin( );	// this will get initial pin state

private:
	I2Ccomm * _export;

}; // class


#endif

