// Keypad_exp.cpp - Implementation of bridge function between Keypad and expander port chips
//
// Started: May 18, 2020 Copyright (C) G. D. (Joe) Young <jyoung@islandnet.com>
//
// revised:
//
// override the pin_mode, pin_read, and pin_write i/o functions in Keypad so that the
// Keypad operation can be simply accomplished on a I2C port expander (eg. PCF8574,
// or PCF8575 and others). To complete the bridge, a port expander library object
// is created and passed as an argument to the construction of the Keypad_exp object.
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

#include "Keypad_exp.h"

// Let the user define a keymap - assume the same row/column count as defined in constructor
void Keypad_exp::begin(char *userKeymap) {
    Keypad::begin(userKeymap);
	_export->port_write( 0xffff );			//set to power-up state
	_export->pinState_set( );
}

// Initialize I2C
void Keypad_exp::begin(void) {
	_export->port_write( 0xffff );			//set to power-up state
	_export->pinState_set( );
}

void Keypad_exp::pin_mode(byte pinNum, byte mode) {
	_export->pin_mode( pinNum, mode );
}

void Keypad_exp::pin_write(byte pinNum, boolean level) {
	_export->pin_write( pinNum, level );
} // I2CxWrite( )


int Keypad_exp::pin_read(byte pinNum) {
	return _export->pin_read( pinNum );
}


