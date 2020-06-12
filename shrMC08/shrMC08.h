/* shrMC08.h - definition for MCP23008 I2C port expander sharing library
 *
 * created: June 12, 2020  Copyright (C) G. D. (Joe) Young <jyoung@islandnet.com>
 *
 * revised:
 *
 * The shrPort libraries provide a set of I/O functions similar in function to the
 * arduino digitalRead, digitalWrite, pinMode, etc., except operating at the pins of a
 * port expander IC. The various shrPrtxxx provide these emulations for specific types
 * of expander so that only the construction of the port expander object needs to be
 * changed within a sketch. The shrMC08 library can be used by itself for these i/o
 * operations, but a shrMC08 object can also be supplied as an argument to various
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
#ifndef SHRMC08_H
#define SHRMC08_H

#include "Arduino.h"
#include "I2Ccomm.h"
#include "Wire.h"

// define possible device types that can be driven (only one for now)
#define MCP23008 1

class shrMC08 : public I2Ccomm {
public:
	shrMC08( byte address, byte device = MCP23008, TwoWire *_awire=&Wire ) 
		         { i2caddr = address; _device = device; _wire=_awire; }

	// Wire function
	void begin(void);

	void pin_mode(byte pinNum, byte mode);
	void pin_write(byte pinNum, boolean level);
	int  pin_read(byte pinNum);
	// read initial value for pinState
	word pinState_set( );
	// write a whole byte to i2c port
	void port_write( word i2cportval );
	// access functions for IODIR state copy
	word iodir_read( );
	void iodir_write( word iodir );

private:
    // I2C device address
    byte i2caddr;
	byte _device;
	// I2C pin_write state persistant storage
	byte pinState;
	byte pin_iosetup( );
	// MCP setup
	byte iodir_state;    // copy of IODIR register
	void _begin( void );
	TwoWire * _wire;
};


#endif // SHRMC08_H

