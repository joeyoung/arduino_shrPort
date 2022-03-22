// shrPrta.cpp - class implementation for shareable, I2C, i/o expansion port
//
// created: May 26, 2020 Copyright (C) G. D. (Joe) Young <jyoung@islandnet.com>
//
// revised: Jul  8/20 - add reg_write, reg_read
//          Mar 17/22 - correct reg_write arguments in _begin( ) - Thanks nicolas buis
//
//
// This version of the I2C port i/o should be cleaner in implementation than
// those specialized for certain uses of the port - Keypad_I2C, flashPin_I2C,
// etc. The same specialization is instead implemented by using one of the 
// available, similarly named, 'bridge' libraries - Keypad_exp, 
// LiquidCrystal_exp, etc. This shrPort library supports the family of 8 and 
// 16 bit port expander IC which have compatible register sets: PCA9554, 
// PCA9555, PCA9539, PCA9534, PCA9535, TCA6408, TCA6416, and possibly others.
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

#include "shrPrta.h"

// Initialize I2C
void shrPrta::begin(void) {
	_begin( );
}


// configure port registers as if just power-on reset
void shrPrta::_begin( ) {
	reg_write( OREG, 0xffff );
	reg_write( VREG, 0x0000 );
	reg_write( IODIR, 0xffff );
	iodir_state = 0xffff;
	pinState = pinState_set( );
} // _begin( )


void shrPrta::pin_mode(byte pinNum, byte mode) {
	word mask = 1<<pinNum;
	if( mode == OUTPUT ) {
		iodir_state &= ~mask;
	} else {
		iodir_state |= mask;
	}
	reg_write( IODIR, iodir_state );
} // pin_mode( )


void shrPrta::pin_write(byte pinNum, boolean level) {
	word mask = 1<<pinNum;
	if( level == HIGH ) {
		pinState |= mask;
	} else {
		pinState &= ~mask;
	}
	reg_write( OREG, pinState );
} // I2CxWrite( )


int shrPrta::pin_read(byte pinNum) {
	word mask = 0x1<<pinNum;
	word pinVal = reg_read( IREG );
	pinVal &= mask;
	if( pinVal == mask ) {
		return 1;
	} else {
		return 0;
	}
} // pin_read( )

void shrPrta::port_write( word i2cportval ) {
	reg_write( OREG, i2cportval );
} // port_write( ) - public access


void shrPrta::reg_write( byte reg, word i2cportval ) {
	wire_->beginTransmission((int)i2caddr);
	wire_->write( reg<<(i2cwidth-1) );			//twice as many regs for 9555
	wire_->write( i2cportval & 0x00FF );
	if (i2cwidth > 1) {
		wire_->write( i2cportval >> 8 );
	}
	wire_->endTransmission();
	if( reg == OREG) pinState = pinState_set( );
} // reg_write( ) - private


word shrPrta::pinState_set( ) {
	return reg_read( IREG );
} // pinState_set( )


word shrPrta::reg_read( byte reg ) {
	wire_->beginTransmission( (int)i2caddr );
	wire_->write( reg<<(i2cwidth-1) );
	wire_->endTransmission( );
	wire_->requestFrom( (int)i2caddr, (int)i2cwidth );
	word regState = wire_->read( );
	if (i2cwidth > 1) {
		regState |= wire_->read( ) << 8;
	}
	return regState;
} // reg_read( )


word shrPrta::iodir_read( ) {
	return iodir_state;
} // iodir_read( )


void shrPrta::iodir_write( word iodir ) {
	reg_write( IODIR, iodir );
	iodir_state = iodir;
} // iodir_write( )


