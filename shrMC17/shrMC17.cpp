/* shrMC17.cpp - MCP23017 I2C port expander sharing library
 *
 * created: May 19, 2020  Copyright (C) G. D. (Joe) Young <jyoung@islandnet.com>
 *
 * revised:
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

#define GPIOA 0x12		//MCP23017 GPIO reg
#define GPIOB 0x13		//MCP23017 GPIO reg
#define IODIRA 0x00		//MCP23017 I/O direction register
#define IODIRB 0x01		//MCP23017 I/O direction register
#define IOCON 0x0a		//MCP23017 I/O configuration register
#define GPPUA 0x0c		//MCP23017 pullup resistors control


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
	_wire->beginTransmission( (int)i2caddr );
	_wire->write( GPPUA ); // enable pullups on all inputs
	_wire->write( 0xff );
	_wire->write( 0xff );
	_wire->endTransmission( );
	_wire->beginTransmission( (int)i2caddr );
	_wire->write( IODIRA ); // setup port direction - all inputs to start
	_wire->write( lowByte( iodirec ) );
	_wire->write( highByte( iodirec ) );
	_wire->endTransmission( );
	_wire->beginTransmission( (int)i2caddr );
	_wire->write( GPIOA );	//point register pointer to gpio reg
	_wire->write( lowByte(iodirec) ); // make o/p latch agree with pulled-up pins
	_wire->write( highByte(iodirec) );
	_wire->endTransmission( );
} // _begin( )

// individual pin setup - modify pin bit in IODIR reg.
void shrMC17::pin_mode(byte pinNum, byte mode) {
	word mask = 0b0000000000000001 << pinNum;
	if( mode == OUTPUT ) {
		iodir_state &= ~mask;
	} else {
		iodir_state |= mask;
	} // if mode
	_wire->beginTransmission((int)i2caddr);
	_wire->write( IODIRA );
	_wire->write( lowByte( iodir_state ) );
	_wire->write( highByte( iodir_state ) );
	_wire->endTransmission();
} // pin_mode( )

void shrMC17::pin_write(byte pinNum, boolean level) {
	word mask = 1<<pinNum;
	if( level == HIGH ) {
		pinState |= mask;
	} else {
		pinState &= ~mask;
	}
	port_write( pinState );
} // MC17xWrite( )


int shrMC17::pin_read(byte pinNum) {
	_wire->beginTransmission((int)i2caddr);
	_wire->write( GPIOA );
	_wire->endTransmission( );
	word mask = 0x1<<pinNum;
	_wire->requestFrom((int)i2caddr, 2);
	word pinVal = 0;
	pinVal = _wire->read( );
	pinVal |= ( _wire->read( )<<8 );
	pinVal &= mask;
	if( pinVal == mask ) {
		return 1;
	} else {
		return 0;
	}
}

void shrMC17::port_write( word i2cportval ) {
// MCP23017 requires a register address on each write
	_wire->beginTransmission((int)i2caddr);
	_wire->write( GPIOA );
	_wire->write( lowByte( i2cportval ) );
	_wire->write( highByte( i2cportval ) );
	_wire->endTransmission();
	pinState = i2cportval;
} // port_write( )

word shrMC17::pinState_set( ) {
	_wire->beginTransmission((int)i2caddr);
	_wire->write( GPIOA );
	_wire->endTransmission( );
	_wire->requestFrom( (int)i2caddr, 2 );
	pinState = 0;
	pinState = _wire->read( );
	pinState |= (_wire->read( )<<8);
	return pinState;
} // set_pinState( )

// access functions for IODIR state copy
word shrMC17::iodir_read( ) {
	return iodir_state;  // local copy is always same as chip's register
} // iodir_read( )

void shrMC17::iodir_write( word iodir ) {
	iodir_state = iodir;
	_wire->beginTransmission((int)i2caddr);   // read current IODIR reg 
	_wire->write( IODIRA );
	_wire->write( lowByte( iodir_state ) );
	_wire->write( highByte( iodir_state ) );
	_wire->endTransmission();
} // iodir_write( )


