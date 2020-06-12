// shrPort.h - class definition for shareable, I2C, i/o expansion port
//
// created: Jan 18, 2014  G. D. (Joe) Young <jyoung@islandnet.com>
//
// revised: Jan 21/14 - cleanups
//          Jan 22/14 - add pin_mode function to maintain portDir state
//			May 22/20 - derive from I2Ccomm for common interface
//
//
// This version of the I2C port i/o should be cleaner in implementation than
// those specialized for certain uses of the port - Keypad_I2C, flashPin_I2C,
// etc. The port type is presumed to be PCF8574 or PCF8575 (here, but this
// organization suggests a possible all-chip version).
// 
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

#ifndef SHRPORT_H
#define SHRPORT_H

#define	PCF8574	1	// PCF8574 I/O expander device is 1 byte wide
#define PCF8575 2	// PCF8575 I/O expander device is 2 bytes wide

#include <Wire.h>
#include <Arduino.h>
#include <I2Ccomm.h>

class shrPort : public I2Ccomm {

public:

	shrPort( byte I2Caddress, byte width, TwoWire * awire = &Wire )
           { i2caddr = I2Caddress; i2cwidth = width; portDir = 0xffff; _jwire = awire; }

	void begin( );
	void pin_mode( byte pinNum, byte mode );
	void pin_write( byte pinNum, boolean level );
	int pin_read( byte pinNum );
	// read initial value for pinState
	word pinState_set( );
	void port_write( word i2cportval );
	// access functions for IODIR state copy
	word iodir_read( );
	void iodir_write( word iodir );


private:

    // I2C device address
    byte i2caddr;
    // I2C port expander device width in bytes (1 for 8574, 2 for 8575)
    byte i2cwidth;
	// I2C pin_write state persistant storage
	// least significant byte is used for 8-bit port expanders
	word portState;
	word portDir;	// debug access
	TwoWire * _jwire;

}; // class shrPort


#endif

