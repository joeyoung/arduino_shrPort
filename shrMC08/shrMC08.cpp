/* shrMC08.cpp - implementation for MCP23008 I2C port expander sharing library
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

#include "shrMC08.h"

#define IODIR 0x00		//MCP23008 I/O direction register
#define IOCON 0x05		//MCP23008 I/O configuration register
#define GPPU 0x06		//MCP23008 pin pullup resistor reg 1==>enable pullup
#define GPIO 0x09		//MCP23008 GPIO reg


/////// Extended shrMC08 library functions. ////////////////////////

// Initialize MCP
void shrMC08::begin(void) {
	_begin( );
	pinState = pinState_set( );
}


byte iodirec = 0xff;            //direction of each bit - reset state = all inputs.
byte MCPconfig[11] = { IODIR, iodirec, 0, 0, 0, 0, 0x00, iodirec, 0, 0, 0xff };

// Initialize MCP23008
// MCP23008 has set of 10 registers to configure before it can
//  act like the simple PCF port
void shrMC08::_begin( void ) {
	iodir_state = iodirec;
	MCPconfig[1] = iodirec;		// direction register
	MCPconfig[7] = iodirec;		// pullup resistors register--pullup inputs
	_wire->beginTransmission( (int)i2caddr );
	_wire->write( IOCON ); // turn on register address incrementing
	_wire->write( 0x00 );
	_wire->endTransmission( );
	_wire->beginTransmission( (int)i2caddr );
	_wire->write( MCPconfig, 11 ); // setup port--leave adr incr ON
	_wire->endTransmission( );
	_wire->beginTransmission( (int)i2caddr );
	_wire->write( IOCON ); // turn OFF register address incrementing
	_wire->write( 0x20 );
	_wire->endTransmission( );
	_wire->beginTransmission( (int)i2caddr );
	_wire->write( GPIO );	//point register pointer to gpio reg
	_wire->endTransmission( );
} // _begin( )

// individual pin setup - modify pin bit in IODIR reg. Don't change pullups
void shrMC08::pin_mode(byte pinNum, byte mode) {
	byte mask = 0b00000001 << pinNum;
	if( mode == OUTPUT ) {
		iodir_state &= ~mask;
	} else {     // here, could check for pullup ON/OFF - omitted for now
		iodir_state |= mask;
	} // if mode
	_wire->beginTransmission((int)i2caddr);
	_wire->write( IODIR );
	_wire->write( iodir_state );
	_wire->endTransmission();
	_wire->beginTransmission((int)i2caddr);	// reset adr pointer to gpio
	_wire->write( GPIO );
	_wire->endTransmission();
} // pin_mode( )

void shrMC08::pin_write(byte pinNum, boolean level) {
//	static byte pinState;  // allocated in constructor GDY120704
	byte mask = 1<<pinNum;
	if( level == HIGH ) {
		pinState |= mask;
	} else {
		pinState &= ~mask;
	}
	port_write( pinState );
} // MCPxWrite( )


int shrMC08::pin_read(byte pinNum) {
	byte mask = 0x1<<pinNum;
	_wire->requestFrom((int)i2caddr, 1);
	byte pinVal = _wire->read( );
	pinVal &= mask;
	if( pinVal == mask ) {
		return 1;
	} else {
		return 0;
	}
}

void shrMC08::port_write( word i2cportval ) {
// MCP23008 requires a register address on each write
	_wire->beginTransmission((int)i2caddr);
	_wire->write( GPIO );
	_wire->write( lowByte(i2cportval) );
	_wire->endTransmission();
	pinState = lowByte(i2cportval);
} // port_write( )

word shrMC08::pinState_set( ) {
	_wire->requestFrom( (int)i2caddr, 1 );
	pinState = _wire->read( );
	return (word)pinState;
} // set_pinState( )

// access functions for IODIR state copy
word shrMC08::iodir_read( ) {
	return (word)iodir_state;  // local copy is always same as chip's register
} // iodir_read( )

void shrMC08::iodir_write( word iodir ) {
	iodir_state = lowByte(iodir);
	_wire->beginTransmission((int)i2caddr);   // read current IODIR reg 
	_wire->write( IODIR );
	_wire->write( iodir_state );
	_wire->endTransmission();
	_wire->beginTransmission((int)i2caddr);	// reset adr pointer to gpio
	_wire->write( GPIO );
	_wire->endTransmission();
} // iodir_write( )


