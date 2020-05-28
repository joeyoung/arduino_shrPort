// shrPrta.h - class definition for shareable, I2C, i/o expansion port
//
// created: May 26, 2020 Copyright (C) G. D. (Joe) Young <jyoung@islandnet.com>
//
// revised: 
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

#define IREG 0x00	// input port location
#define OREG 0x01	// output port
#define VREG 0x02	// polarity inversion (not used, but configured)
#define CREG 0x03	// configuration reg - IODIR
#define IODIR 0x03	// common alias for config


// Initialize I2C
void shrPrta::begin(void) {
//	wire_->begin();
	_begin( );
}


// configure port registers as if just power-on reset
void shrPrta::_begin( ) {
	p_write( 0xffff, OREG );
	p_write( 0x0000, VREG );
	p_write( 0xffff, IODIR );
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
	p_write( iodir_state, IODIR );
} // pin_mode( )


void shrPrta::pin_write(byte pinNum, boolean level) {
	word mask = 1<<pinNum;
	if( level == HIGH ) {
		pinState |= mask;
	} else {
		pinState &= ~mask;
	}
	p_write( pinState, OREG );
} // I2CxWrite( )


int shrPrta::pin_read(byte pinNum) {
	word mask = 0x1<<pinNum;
	wire_->beginTransmission( (int)i2caddr );
	wire_->write( IREG );
	wire_->endTransmission( );
	wire_->requestFrom((int)i2caddr, (int)i2cwidth);
	word pinVal = wire_->read( );
	if (i2cwidth > 1) {
		pinVal |= wire_->read( ) << 8;
	} 
	pinVal &= mask;
	if( pinVal == mask ) {
		return 1;
	} else {
		return 0;
	}
} // pin_read( )

void shrPrta::port_write( word i2cportval ) {
	p_write( i2cportval, OREG );
} // port_write( ) - public access


void shrPrta::p_write( word i2cportval, byte reg ) {
	wire_->beginTransmission((int)i2caddr);
	wire_->write( reg<<(i2cwidth-1) );			//twice as many regs for 9555
	wire_->write( i2cportval & 0x00FF );
	if (i2cwidth > 1) {
		wire_->write( i2cportval >> 8 );
	}
	wire_->endTransmission();
//	if( reg == OREG) pinState = i2cportval;		//not quite right - re-read i/p??
	if( reg == OREG) pinState = pinState_set( );
} // p_write( ) - private


word shrPrta::pinState_set( ) {
	wire_->beginTransmission( (int)i2caddr );
	wire_->write( IREG );
	wire_->endTransmission( );
	wire_->requestFrom( (int)i2caddr, (int)i2cwidth );
	pinState = wire_->read( );
	if (i2cwidth > 1) {
		pinState |= wire_->read( ) << 8;
	}
	return pinState;
} // set_pinState( )


word shrPrta::iodir_read( ) {
//	wire_->requestFrom( (int)i2caddr, (int)i2cwidth );
//	iodir_state = wire_->read( );
//	if( i2cwidth > 1 ) {
//		iodir_state |= wire_->read( ) << 8;
//	}
	return iodir_state;
} // iodir_read( )


void shrPrta::iodir_write( word iodir ) {
	p_write( iodir, IODIR );
	iodir_state = iodir;
} // iodir_write( )


