// shrPort.cpp - class implementation for shareable, I2C, i/o expansion port
//
// created: Jan 18, 2014  G. D. (Joe) Young <jyoung@islandnet.com>
//
// revised: Jan 21/14 - cleanups
//          Jan 22/14 - add pin_mode function to maintain portDir state
//          May 18/20 - add begin - not needed for PCF8574/5 but other devices need one
//			May 22/20 - derive from I2Ccomm for common interface
//
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


#include "shrPort.h"

void shrPort::begin( ) {
	portDir = 0xffff;
	port_write( 0xffff );
} // begin( )

void shrPort::pin_mode( byte pinNum, byte mode ) {
	word mask = 1<<pinNum;
	if( mode == INPUT ) {
		portDir |= mask;
		portState |= mask;
	} else {
		portDir &= ~mask;
	}
} // pin_mode( )

void shrPort::pin_write(byte pinNum, boolean level) {
//	portState = pinState_set( );
	word mask = 1<<pinNum;
	if( level == HIGH ) {
		portState |= mask;
	} else {
		portState &= ~mask;
	}
	port_write( portState );
} // I2C bus pin_write( )

int shrPort::pin_read( byte pinNum ) {
	word mask = 1<<pinNum;
	_jwire->requestFrom( (int)i2caddr, (int)i2cwidth );
	word rdState = _jwire->read( );
	if( i2cwidth > 1 ) {
		rdState |= (_jwire->read( ) << 8);
	}
	rdState &= mask;
	if( rdState == mask ) {
		return HIGH;
	} else {
		return LOW;
	}
} // pin_read( )

void shrPort::port_write( word i2cportval ) {
	i2cportval |= portDir;
	_jwire->beginTransmission( (int)i2caddr );
	_jwire->write( (byte)( i2cportval & 0x00FF ) );
	if( i2cwidth > 1 ) {
		_jwire->write( (byte)(i2cportval >> 8) );
	}
	_jwire->endTransmission( );
	portState = i2cportval;
} // port_write( )

word shrPort::pinState_set( ) {
	_jwire->requestFrom( (int)i2caddr, (int)i2cwidth );
	portState = _jwire->read( );
	if( i2cwidth > 1 ) {
		portState |= _jwire->read( ) << 8;
	}
	return portState;
} // set_pinState( )

// debug
word shrPort::portDir_rd( ) {
	return portDir;
} // portDir_rd( )

