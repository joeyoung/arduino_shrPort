/* LcdBarCentreZero_exp.h Lcd centre-zero bargraph library
 *
 * Created: Sept 19, 2011 - Copyright (C) G. D. Young <jyoung@islandnet.com>
 *
 * Revised: Sept 20/11 - add blank bar display area before drawing
 *          Sept 20/11 - create library version
 *          Oct 14/11 - I2C connected display version
 *          May 19/20 - version for shrPort connected display
 *
 * Use LiquidCrystal library for drawing special bar-graph characters
 *
 * Similar to LcdBarGraph from Balazs Keleman; differences:
 *    - no scaling inside bar drawing - caller must scale to graph size
 *    - bars not solid, appear uniform across character columns
 *    - can draw several bar graphs on each of any line of display
 *    - because both pos and neg display, bars can grow to left or 
 *      right or both
 *
 * This version (LcdBarCentreZero_exp) uses the shrPort version (LiquidCrystal_exp) to
 * override the expanderWrite( ) i/o function in LiquidCrystal_I2C so that the
 * LiquidCrystal_I2C operation can be accomplished on a I2C port expander (eg. PCF8574,
 * or PCF8575 and others). To complete the bridge, a port expander library object (prt)
 * is created and passed as an argument to the construction of the LiquidCrystal_exp
 * object. Implementation of the needed port expander library is similar to the i/o in
 * the Keypad_I2C library. See shrPort (for PCF8474/5) and other versions for different
 * port expander IC in repository arduino_shrPort.
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see <https://www.gnu.org/licenses/>.
 * 
 */

#ifndef LcdBarCentreZero_exp_h
#define LcdBarCentreZero_exp_h

// Arduino versioning.
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"	// for digitalRead, digitalWrite, etc
#else
#include "WProgram.h"
#endif

#include <LcdBarCentreZero_exp.h>
#include <LiquidCrystal_exp.h>

class LcdBarCentreZero_exp
{
public:
	//class constructor
	LcdBarCentreZero_exp( LiquidCrystal_exp* lic2 );

	//draw a zero-centre bargraph with centre at column centre on line line
	// and longest bar length = maxbars
	void drawBar( int value, byte maxbars, byte centre, byte line );

	//load character generator with bargraph symbols
	void loadCG( void );

private:
	LiquidCrystal_exp* _lcdi2c;

	static byte _gchar0[8];
	static byte _gchar1[8];
	static byte _gchar2[8];
	static byte _gchar3[8];
	static byte _gchar4[8];
	static byte _gchar5[8];
	static byte _gchar6[8];
	static byte _gchar7[8];

}; // class LcdBarCentreZero_exp

#endif
