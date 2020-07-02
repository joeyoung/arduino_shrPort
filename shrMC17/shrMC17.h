/* shrMC17.h - definition for MCP23017 I2C port expander sharing library
 *
 * created: May 19, 2020  Copyright (C) G. D. (Joe) Young <jyoung@islandnet.com>
 *
 * revised: May 22/20 - named shrMC17 from shrPort. shrPort is for PCF8574/5
 *          Jun 30/20 - add register read, write. ver 1.5
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
#include "MC17bitDef.h"

// define possible device types that can be driven 
#define MCP23017 1
#define GPIOA 0x12		//MCP23017 GPIO reg
#define GPIOB 0x13		//MCP23017 GPIO reg
#define IODIRA 0x00		//MCP23017 I/O direction register
#define IODIRB 0x01		//MCP23017 I/O direction register
#define IOCON 0x0a		//MCP23017 I/O configuration register
#define GPPUA 0x0c		//MCP23017 pullup resistors control
#define IPOLA 0x02		//interrupt polarity
#define GPINTENA 0x04	//interrupt enable bits
#define DEFVALA	0x06	//default values for int change
#define INTCONA 0x08	//interrupt control
#define INTFA 0x0e		//interrupt flags
#define INTCAPA 0x10	//interrupt capture
#define OLATA 0x14		//output latch


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
	word reg_read( byte reg );
	void reg_write( byte reg, word val );

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


