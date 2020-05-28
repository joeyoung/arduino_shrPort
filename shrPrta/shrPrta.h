// shrPrta.cpp - class implementation for shareable, I2C, i/o expansion port
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

#ifndef SHRPRTA_H
#define SHRPRTA_H

#include "Wire.h"
#include "I2Ccomm.h"

// literals to use to define the width of the port--last parameter in constructors
#define PCA9554 1	// PCA9554 I/O expander device is 1 byte wide
#define PCA9555 2	// PCA9555 I/O expander device is 2 bytes wide
#define PCA9539 2
#define PCA9534 1
#define PCA9535 2
#define TCA6408 1
#define TCA6416 2


class shrPrta : public I2Ccomm {
public:
	shrPrta( byte address, byte width = 1, TwoWire *awire_ = &Wire ) 
		   { i2caddr = address; i2cwidth = width; iodir_state = 0xffff; wire_=awire_; }	
	

	// Wire function setup port
	void begin(void);

	void pin_mode(byte pinNum, byte mode);
	void pin_write(byte pinNum, boolean level);
	int  pin_read(byte pinNum);
	// read initial value for pinState
	word pinState_set( );
	// write a whole byte or word (depending on the port expander chip) to i2c
	void port_write( word i2cportval );
	// access functions for IODIR state copy
	word iodir_read( );
	void iodir_write( word iodir );

private:
    // I2C device address
    byte i2caddr;
	TwoWire * wire_;	//for optional alternate WireX port
    // I2C port expander device width in bytes (1 for 8574, 2 for 8575)
    byte i2cwidth;
	// I2C pin_write state persistant storage
	// least significant byte is used for 8-bit port expanders
	word pinState;
	word iodir_state;    // copy of IODIR register
	void _begin( void );
	void p_write( word i2cportval, byte reg );
};


#endif // SHRPRTA_H


