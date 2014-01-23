// flashPin.cpp - flash arduino pin using non-blocking timing

// created: Oct 12, 2013 G. D. (Joe) Young

// revised: Jan 14/14 - add specification of resting level
//          Jan 15/14 - use i/o wrapper functions pin_mode, pin_write
//          Jan 16/14 - fp_set function added



#include "flashPin.h"

// constructor - allow default setting of pin and duration
	flashPin::flashPin( byte pin, bool restLevel, unsigned long duration ) {
		fpin = pin;
		fdur = duration;
		fstate = restLevel;
		frest = restLevel;
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
		fend = millis( ) + duratn;		// setup flashOff with end time
	} // flash

// flash - default duration
	void flashPin::flash( ) {
		pin_write( fpin, !frest );
		fstate = !fstate;
		fend = millis( ) + fdur;	// setup flashOff with constructed end time
	} // flash

// flashOff - call at loop rate to find time to turn off pin
	void flashPin::flashOff( ) {
		if( fstate == frest ) return;	// quick exit if already off
		if( fend < millis( ) ) {
			pin_write( fpin, frest );
			fstate = frest;
		} // if timeup
	} // flashOff
