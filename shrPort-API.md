####shrPort API
The share port libraries each provide the following functions which are meant to operate
similarly to the basic arduino digital pin functions--`digitalRead`, `digitalWrite`, and
 `pinMode`.
These emulation functions are:

`int pin_read( byte pin )` 

`void pin_write( byte pin, byte value )`

`void pin_mode( byte pin, MODE )`

where: `pin` is the i/o expander port pin number with values 0..7 for 8-bit ports, 0..15 for 16-bit ports;
 `value` is (or resolves to) 0 or 1; and `MODE` is a literal--one of INPUT`, `INPUT_PULLUP`, or
 `OUTPUT`.

In addition, whole-port i/o functions are provided:

`word pinState_set( )`  - returns the current state of all the port bits, sets internal state.

`void port_write( word i2cportval )` - sets all the port bits to those in `i2cportval`

`word iodir_read( )` - returns the pattern of bits in the direction register.

`void iodir_write( word dirval )` - sets the pattern of bit direction to `dirval` bits

Each bit in the direction register sets the direction of the corresponding pin with
 0 output, 1 input. Expander ports typically power on with all bits as inputs.

These functions become available to a program via an object instantiated by a declaration
such as:

	shrPrt prt( 0x38, PCF8574 );
	
`prt` is the name of the object and the I2C expander port is a PCF8574 located on the 
I2C bus at address 0x38. Similar declarations work with each of the device-specific 
share port libraries. A complete example sketch to set up for reading pin 7 and writing
the result to pin 0 once on startup follows:

```cpp

	#include <shrPort.h>
	#include <Wire.h>
	
	shrPort prt( 0x38, PCF8574 );
	
	void setup( ) {
		Wire.begin( );
		prt.begin( );
		prt.pin_mode( 7, INPUT );
		prt.pin_mode( 0, OUTPUT );
		prt.pin_write( prt.pin_read( 7 ) );
	}
	
	void loop( ) {
	
	}
		
```

#####other features

Each of the share port libraries has an optional 3rd parameter that takes a reference
to an I2C interface bus, for those boards with processors such as SAMD which (can) have
more than one I2C interface, for example the arduino MKR ZERO. This parameter defaults
to &Wire as illustrated with the example above.

See the arduino tutorial [Adding more Serial Interfaces to SAMD microcontrollers 
(SERCOM)](https://www.arduino.cc/en/Tutorial/SamdSercom)
 
The libraries define a number of 'device' literals which correspond to the port expander
devices the library supports. One of these literals is used for the second parameter in
the object constructors. The parameter specifies a 'width' of the expander - 8 bits or
16 bits when a device family is register compatible between 8 and 16 bits, such as the
PCF8574 (8 bit) and PCF8575 (16 bit). Some libraries only support one device - shrMC17, 
shrMC16, shrMC08, but the device literal can be considered as informative. If the optional I2C
specification is used, then a device literal is needed as a placeholder because if a
normally defaulted parameter is specified, then all the defaulted parameters preceding
the specified one must be included.

The port expander devices considered in these libraries all have an interrupt output
(or two) which are activated for any pin state change. This interrupt can be used to
signal the arduino microprocessor attention to the port is needed. However, since I2C
i/o with Wire cannot run within an interrupt service routine, care is required to
attend these interrupts.

Some port expanders have additional features such as polarity inversion, interrupt selection,
and so on. These features are not provided with the existing libraries, the purpose being
to render the ports functionally alike, so the simplest port's features are taken to define
the functionality of all.


