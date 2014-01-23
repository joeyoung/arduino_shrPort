/* shrPort.cpp - class implementation for shareable, I2C, i/o expansion port
 *
 * created: Jan 18, 2014  G. D. (Joe) Young <jyoung@islandnet.com>
 *
 * revised: Jan 21/14 - cleanups
 *          Jan 22/14 - add pin_mode function to maintain portDir state
 *
 *
 */


#include "shrPort.h"

void shrPort::pin_mode( byte pinNum, byte mode ) {
	word mask = 1<<pinNum;
	if( mode == INPUT ) {
		portDir |= mask;
	} else {
		portDir &= ~mask;
	}
} // pin_mode( )

void shrPort::pin_write(byte pinNum, boolean level) {
	portState = pinState_set( );
	word mask = 1<<pinNum;
	if( level == HIGH ) {
		portState |= mask;
	} else {
		portState &= ~mask;
	}
	port_write( portState );
} // I2C bus pin_write( )

bool shrPort::pin_read( byte pinNum ) {
	TwoWire::requestFrom( (int)i2caddr, (int)i2cwidth );
	portState = TwoWire::read( );
	if( i2cwidth > 1 ) {
		portState |= TwoWire::read( ) << 8;
	}
	word mask = 1<<pinNum;
	if( portState & mask ) {
		return HIGH;
	} else {
		return LOW;
	}
} // pin_read( )

void shrPort::port_write( word i2cportval ) {
	i2cportval |= portDir;
	TwoWire::beginTransmission( (int)i2caddr );
	TwoWire::write( (byte)( i2cportval & 0x00FF ) );
	if( i2cwidth > 1 ) {
		TwoWire::write( i2cportval >> 8 );
	}
	TwoWire::endTransmission( );
	portState = i2cportval;
} // port_write( )

word shrPort::pinState_set( ) {
	TwoWire::requestFrom( (int)i2caddr, (int)i2cwidth );
	portState = TwoWire::read( );
	if( i2cwidth > 1 ) {
		portState |= TwoWire::read( ) << 8;
	}
	return portState;
} // set_pinState( )

// debug
word shrPort::portDir_rd( ) {
	return portDir;
} // portDir_rd( )

