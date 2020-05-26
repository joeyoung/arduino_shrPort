// LiquidCrystal_exp.cpp - shared I2C expansion port extension of LiquidCrystal_I2C
//
// created: May 16, 2020  Copyright (C) G. D. (Joe) Young <jyoung@islandnet.com>
//
// revised: May 22/20 - port side selection
//
// override the expanderWrite( ) i/o function in LiquidCrystal_I2C so that the
// LiquidCrystal_I2C operation can be accomplished on a I2C port expander (eg. PCF8574,
// or PCF8575 and others). To complete the bridge, a port expander library object
// is created and passed as an argument to the construction of the LiquidCrystal_exp
// object. Implementation of the needed port expander library is similar to the i/o in
// the Keypad_I2C library. See shrPort (for PCF8474/5) and other versions for different
// port expander IC in repository arduino_shrPort.
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
// 

#include "LiquidCrystal_exp.h"

void LiquidCrystal_exp::expanderWrite( uint8_t val ) {
	uint16_t wval = 0;
	if( portside ) {
		wval = (_export->pinState_set( ) & 0x00ff) | (val<<8);		//use msb
	} else {
		wval = (_export->pinState_set( ) & 0xff00) | val;			//use lsb
	}
	_export->port_write( wval );
} //expanderWrite( )


