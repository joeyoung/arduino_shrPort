// LiquidCrystal_exp.h - shared I2C expansion port extension of LiquidCrystal_I2C
//
// created: May 16, 2020  Copyright (C) G. D. (Joe) Young <jyoung@islandnet.com>
//
// revised: May 22/20 - add port side selection
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

#ifndef LIQUIDCRYSTAL_EXP
#define LIQUIDCRYSTAL_EXP

#include "LiquidCrystal_I2C.h"
#include "I2Ccomm.h"
#include "Arduino.h"

#define PB07 0			//port bits to use: 0..7 or,
#define PB815 1			//  8..15

class LiquidCrystal_exp : public LiquidCrystal_I2C {

public:
	LiquidCrystal_exp( I2Ccomm * exP ,uint8_t lcd_cols, uint8_t lcd_rows, uint8_t pbits = PB07 )
		: LiquidCrystal_I2C( 0xff, lcd_cols, lcd_rows ) { _export = exP; portside = pbits; }

	void expanderWrite( uint8_t val );

private:
	I2Ccomm * _export;
	uint8_t portside;		// write to either bits 0..7, or to 8..15

}; // class LiquidCrystal_exp

#endif // LIQUIDCRYSTAL_EXP


