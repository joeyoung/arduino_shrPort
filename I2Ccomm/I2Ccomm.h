// I2C base class to be common to users and suppliers of I2C port chip i/o

// started: May 13, 2020 - G. D. (Joe) Young <jyoung@islandnet.com>
//
// revised:
//


#ifndef I2CPORT_H
#define I2CPORT_H

#include "Arduino.h"


class I2Ccomm {

public:
	I2Ccomm( ) { }

	virtual void pin_mode(uint8_t pinNum, byte mode) = 0;
	virtual void pin_write(uint8_t pinNum, bool level) = 0;
	virtual int pin_read(uint8_t pinNum ) = 0;

	virtual void begin( ) = 0;	// this will get initial pin state

	// read initial value for pinState
	virtual word pinState_set( ) = 0;
	// write a whole byte or word (depending on the port expander chip) to i2c port
	virtual void port_write( word i2cportval ) = 0;

private:

}; // class




#endif
