/* shrMC17.cpp - MCP23017 I2C port expander sharing library
 *
 * created: May 19, 2020  Copyright (C) G. D. (Joe) Young <jyoung@islandnet.com>
 *
 * revised: Jun 30/20 - add register write, read, use in other functions. ver 1.5.
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


#include "shrMC17.h"


// Initialize MC17
void shrMC17::begin(void) {
	_begin( );
	pinState = pinState_set( );
}

word iodirec = 0xffff;            //direction of each bit - reset state = all inputs.
byte iocon = 0x10;                // reset state for bank, disable slew control

// Initialize MCP23017
// MCP23017 byte registers act in word pairs

void shrMC17::_begin( void ) {
	iodir_state = iodirec;
	_wire->beginTransmission( (int)i2caddr );
	_wire->write( IOCON ); // same as when reset
	_wire->write( iocon );
	_wire->endTransmission( );
	reg_write( GPPUA, 0xffff );
	reg_write( IODIRA, iodirec );
	reg_write( GPIOA, iodirec );
} // _begin( )

// individual pin setup - modify pin bit in IODIR reg.
void shrMC17::pin_mode(byte pinNum, byte mode) {
	word mask = 0b0000000000000001 << pinNum;
	if( mode == OUTPUT ) {
		iodir_state &= ~mask;
	} else {
		iodir_state |= mask;
	} // if mode
	reg_write( IODIRA, iodir_state );
} // pin_mode( )

void shrMC17::pin_write(byte pinNum, boolean level) {
	word mask = 1<<pinNum;
	if( level == HIGH ) {
		pinState |= mask;
	} else {
		pinState &= ~mask;
	}
	reg_write( GPIOA, pinState );
} // MC17xWrite( )


int shrMC17::pin_read(byte pinNum) {
	word mask = 0x1<<pinNum;
	word pinVal = reg_read( GPIOA );
	pinVal &= mask;
	if( pinVal == mask ) {
		return 1;
	} else {
		return 0;
	}
} // pin_read( )

void shrMC17::port_write( word i2cportval ) {
	reg_write( GPIOA, i2cportval );
	pinState = i2cportval;
} // port_write( )

word shrMC17::pinState_set( ) {
	pinState = reg_read( GPIOA );
	return pinState;
} // set_pinState( )

// access functions for IODIR state copy
word shrMC17::iodir_read( ) {
	return iodir_state;  // local copy is always same as chip's register
} // iodir_read( )

void shrMC17::iodir_write( word iodir ) {
	iodir_state = iodir;
	reg_write( IODIRA, iodir_state );
} // iodir_write( )

// version 1.5 register read 
word shrMC17::reg_read( byte reg ) {
	_wire->beginTransmission((int)i2caddr);
	_wire->write( reg );
	_wire->endTransmission( );
	_wire->requestFrom( (int)i2caddr, 2 );
	word rval = _wire->read( );
	rval |= (_wire->read( )<<8);
	return rval;
} // reg_read( )

// version 1.5 register write
void shrMC17::reg_write( byte reg, word regval ) {
	_wire->beginTransmission((int)i2caddr);   // read current IODIR reg 
	_wire->write( reg );
	_wire->write( lowByte( regval ) );
	_wire->write( highByte( regval ) );
	_wire->endTransmission();
} // reg_write( )

