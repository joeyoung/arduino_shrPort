// Rotary_exp.h - Definition of bridge function between Rotary and expander port chips
//
// Started: June 10, 2020 Copyright (C) G. D. (Joe) Young <jyoung@islandnet.com>
//
// revised:
//
// override the pin_mode, pin_read, and pin_write i/o functions in Rotary so that the
// Rotary operation can be simply accomplished on a I2C port expander (eg. PCF8574,
// or PCF8575 and others). To complete the bridge, a port expander library object
// is created and passed as an argument to the construction of the Rotary_exp object.
// Implementation of the needed port expander library is similar to Keypad_I2C library.
// See shrPort (for PCF8474/5) and other versions in repository arduino_shrPort.
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


#ifndef ROTARY_EXP_H
#define ROTARY_EXP_H

#include "Rotary.h"
#include "I2Ccomm.h"

class Rotary_exp : public Rotary {
public:
	Rotary_exp( I2Ccomm * exP, char pinA, char pinB )
		: Rotary( pinA, pinB ) { _export = exP;	} //constructor
	
	void begin( );
	void pin_mode(byte pinNum, byte mode);
	void pin_write(byte pinNum, boolean level);
	int  pin_read(byte pinNum);

private:
	I2Ccomm * _export;
};


#endif // Rotary_EXP_H

