// flashPin.cpp - flash arduino pin using non-blocking timing

// created: Oct 12, 2013 Copyright(C) G. D. (Joe) Young <jyoung@islandnet.com>

// revised: Jan 14/14 - add specification of resting level
//          Jan 15/14 - use i/o wrapper functions pin_mode, pin_write
//          Jan 16/14 - fp_set function added
//          Apr 24/20 - version 1.0.1 - Library formatting
//          Apr 25/20 - version 1.1.0 - flashRep added
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



#include "flashPin.h"

// constructor - allow default setting of pin and duration
	flashPin::flashPin( byte pin, bool restLevel, unsigned long duration ) {
		fpin = pin;
		fdur = duration;
		fstate = restLevel;
		frest = restLevel;
		frep = 0;
		fon = false;
		ftiming = false;
		pin_mode( fpin, OUTPUT );
		pin_write( fpin, frest );
	} // flashPin constructor

// set - change resting level
	void flashPin::fp_set( bool restLevel ) {
		frest = restLevel;
		fstate = frest;
		pin_write( fpin, frest );
	} // fp_set( )

// flash - with duration specified
	void flashPin::flash( unsigned long duratn ) {
		pin_write( fpin, !frest );
		fstate = !fstate;
		fdur = duratn;			// reset default duration 
		fon	= true;			// indicate on timing
		fmark = millis( );		// setup flashOff with end time
	} // flash

// flash - default duration
	void flashPin::flash( ) {
		pin_write( fpin, !frest );
		fstate = !fstate;
		fon	= true;			// indicate on timing
		fmark = millis( );	// setup flashOff with constructed end time
	} // flash

// flashRep - flash repeat 
	void flashPin::flashRep( char repeats ) {
		frep = repeats;
		if( ftiming ) return;
		flash( );
		ftiming = true;
	} // flashRep

// flashOff - call at loop rate to find time to turn off pin
	void flashPin::flashOff( ) {
		if( fstate == frest && frep == 0 ) return;	// quick exit if off and finished repeats
		if( fon && millis( ) - fmark > fdur ) {
			pin_write( fpin, frest );
			fstate = frest;
			fon = false;
			if( frep > 0 ) fmark = millis( );		// setup OFF timing if repeats pending
		} // if timeup
		if( !fon && frep > 0 && millis( ) - fmark > fdur ) {
			frep--;
			if( frep > 0 ) {
				flash( );
			} else {
				ftiming = false;
			}
		}
	} // flashOff


