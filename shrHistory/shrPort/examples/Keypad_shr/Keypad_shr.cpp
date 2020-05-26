/*
||
|| @file Keypad_shr.cpp
|| @version 2.1 - use shrPort library for PCF8574/PCF8575 access
|| @version 2.0 - PCF8575 support added by Paul Williamson
|| @author G. D. (Joe) Young, ptw
|| @contact "G. D. (Joe) Young" <gdyoung@telus.net>
||
|| @description
|| | Keypad_I2C provides an interface for using matrix keypads that
|| | are attached with I2C port expanders. It supports multiple keypads,
|| | user selectable pins, and user defined keymaps.
|| | Keypad_shr moves the port i/o out to separate shrPort library so
|| | I2C access function private storage data are not duplicated. The
|| | several begin( ) functions are not necessary, but does require that
|| | setup( ) has Wire.begin( )--typically needed for other libraries
|| | anyway. The pin_mode function is re-activated (empty in Keypad_I2C),
|| | and the pin_read function here explicitly calls the pin_mode after
|| | reading because a zero left on an input pin is captured as part of
|| | pinState by other operations (another keypad, eg) and causes the
|| | scanning to 'stick' at the zero pin. The mechanism for keeping track
|| | of inputs (which must be written HIGH to remain useful as inputs) is
|| | to keep portDir up to date. portDir is initialized in the port
|| | constructor to 0xffff (all inputs) as as power-up, then updated by
|| | using pin_mode as the use of the bits changes.
|| #
||
|| @license
|| | This library is free software; you can redistribute it and/or
|| | modify it under the terms of the GNU Lesser General Public
|| | License as published by the Free Software Foundation; version
|| | 2.1 of the License.
|| |
|| | This library is distributed in the hope that it will be useful,
|| | but WITHOUT ANY WARRANTY; without even the implied warranty of
|| | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
|| | Lesser General Public License for more details.
|| |
|| | You should have received a copy of the GNU Lesser General Public
|| | License along with this library; if not, write to the Free Software
|| | Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
|| #
||
*/

#include "Keypad_shr.h"

// Let the user define a keymap - assume the same row/column count as defined in constructor
void Keypad_shr::begin(char *userKeymap) {
    Keypad::begin(userKeymap);
}


void Keypad_shr::pin_mode( byte pinNum, byte mode ) {

	kport.pin_mode( pinNum, mode );
}


void Keypad_shr::pin_write(byte pinNum, boolean level) {

	kport.pin_write( pinNum, level);

} // I2CxWrite( )


int Keypad_shr::pin_read(byte pinNum) {

	if( kport.pin_read( pinNum )==HIGH ) {
		kport.pin_mode( pinNum, INPUT );
		return 1;
	} else {
		kport.pin_mode( pinNum, INPUT );
		return 0;
	}

}


/*
|| @changelog
|| |
|| | 2.1 2014-01-22 - Joe Young : using shrPort library for I2C ports
|| |
|| | 2.0 2013-08-31 - Paul Williamson : Added i2cwidth parameter for PCF8575 support
|| |
|| | 1.0 2012-07-12 - Joe Young : Initial Release
|| #
*/
