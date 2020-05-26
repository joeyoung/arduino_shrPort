arduino library for sharing I2C expansion ports

G. D. (Joe) Young - Jan 21, 2014

Arduino library shrPort provides a 'driver' package of access functions
for I2C bus expansion ports PCF8574 and PCF8575 that approximates the 
individual-pin access capability of the basic arduino (via arduino's 
functions pinMode, digitalWrite, and digitalRead).

The library allows instantiation of 'port' objects on a per-chip basis
with a private copy of the current state of all of the 8 (PCF8574) or
16 (PCF8575) port chip pins. This private state is kept current as 
individual pins are accessed, so that the required whole byte or word
port access can simulate the individual-pin access.

The emulating functions provided are:

	void pin_mode( byte pinNum, byte mode ) {}	// empty for these devices
	void pin_write( byte pinNum, boolean level );
	bool pin_read( byte pinNum );

In addition, two whole-port access functions:

	word pinState_set( );	// sets private copy to the current state
	void port_write( word i2cportval );

See the short test program in the examples folder to illustrate how
these functions are used.


