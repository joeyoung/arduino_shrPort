/* shrMC17.h - definition for MCP23017 I2C port expander sharing library
 *
 * created: May 19, 2020  Copyright (C) G. D. (Joe) Young <jyoung@islandnet.com>
 *
 * revised: May 22/20 - named shrMC17 from shrPort. shrPort is for PCF8574/5
 *
 * The shrPort libraries provide a set of I/O functions similar in function to the
 * arduino digitalRead, digitalWrite, pinMode, etc., except operating at the pins of a
 * port expander IC. The various shrPrtxxx provide these emulations for specific types
 * of expander so that only the construction of the port expander object needs to be
 * changed within a sketch. The shrMC17 library can be used by itself for these i/o
 * operations, but a shrMC17 object can also be supplied as an argument to various
 * bridging objects to extend I2C i/o to other libraries such as Keypad, LiquidCrystal
 * and others. 
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
 */

#ifndef SHRMC17_H
#define SHRMC17_H

#include "Wire.h"
#include "Arduino.h"
#include "I2Ccomm.h"

// define possible device types that can be driven 
#define MCP23017 1

class shrMC17 : public I2Ccomm {
public:
	shrMC17( byte address, byte device = MCP23017, TwoWire *_awire=&Wire ) 
		         { i2caddr = address; _device = device; _wire=_awire; }

	// port setup
	void begin( );

	void pin_mode(byte pinNum, byte mode);
	void pin_write(byte pinNum, boolean level);
	int  pin_read(byte pinNum);
	// read initial value for pinState
	word pinState_set( );
	// write a whole word to i2c port
	void port_write( word i2cportval );
	// access functions for IODIR state copy
	word iodir_read( );
	void iodir_write( word iodir );

private:
    // I2C device address
	byte _device;
    byte i2caddr;
	TwoWire *_wire;
	// I2C pin_write state persistant storage
	word pinState;
//	byte pin_iosetup( );
	// MC17 setup
	word iodir_state;    // copy of IODIR register
	void _begin( void );

}; // class shrMC17


#endif // SHRMC17_H


