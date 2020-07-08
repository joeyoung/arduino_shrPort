###shrPort API
The share port libraries each provide the following functions which are meant to operate
similarly to the basic arduino digital pin functions--`digitalRead`, `digitalWrite`, and
 `pinMode`.
These emulation functions are:

`int pin_read( byte pin )` 

`void pin_write( byte pin, byte value )`

`void pin_mode( byte pin, MODE )`

where: `pin` is the i/o expander port pin number with values 0..7 for 8-bit ports, 0..15 for 16-bit ports;
 `value` is (or resolves to) 0 or 1; and `MODE` is a literal--one of `INPUT`, `INPUT_PULLUP`, or
 `OUTPUT`.

In addition, whole-port read and write functions are provided:

`word pinState_set( )`  - returns the current state of all the port pins, sets internal state.

`void port_write( word i2cportval )` - sets all the port bits to those in `i2cportval`

`word iodir_read( )` - returns the pattern of bits in the direction register.

`void iodir_write( word dirval )` - sets the pattern of bit direction to `dirval` bits

Each bit in the direction register sets the direction of the corresponding pin with
 0 output, 1 input. Expander ports typically power on with all bits as inputs.

These functions become available to a program via an object constructed by a declaration
with the general format:

	libname objname( I2Caddress, device, [I2Cbusref] );
	
For example:

	shrPort prt( 0x38, PCF8574 );
	
In this example, `shrPort` is the library included with `#include <shrPort.h>, `prt` is 
the name of the object, and the I2C expander port is a PCF8574 8-bit expander located on the 
I2C bus at address 0x38. The optional reference to an alternate I2C bus is defaulted to Wire.
Similar declarations work with each of the device-specific share port libraries. A complete,
simple, example sketch to set up a port expander for reading its pin 7 and writing
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

###Additional features

####Optional alternate I2C bus specification
Each of the share port libraries' constructors have an optional 3rd parameter that takes
a reference to an I2C interface bus for the object to use, when boards with processors 
such as SAMD (arduino MKR ZERO) which (can) have more than one I2C interface. This parameter
defaults to &Wire as illustrated with the example above.

See the arduino tutorial [Adding more Serial Interfaces to SAMD microcontrollers 
(SERCOM)](https://www.arduino.cc/en/Tutorial/SamdSercom)

####Literal definitions
The libraries define a number of `device` literals which correspond to the port expander
devices the library supports. One of these literals is used for the second parameter in
the object constructors. The parameter specifies a 'width' of the expander - 8 bits or
16 bits when a device family is register compatible between 8 and 16 bits, such as the
PCF8574 (8 bit) and PCF8575 (16 bit). Some libraries only support one device -- shrMC17, 
shrMC16, shrMC08 -- but the required `device` literal can be considered as informative. While
the `device` parameter for singe-type libraries could be omitted or defaulted, if
the optional I2C specification is used, then a `device` literal is needed as a placeholder
because if a normally defaulted parameter is specified (the alternate bus), then all the
defaulted parameters in the parameter list preceding the specified one must be included.

####Use of interrupts
The port expander devices considered in these libraries all have an interrupt output
(or two) which are activated for any pin state change. This interrupt can be used to
signal the arduino microprocessor attention to the port is needed. However, since I2C
i/o with `Wire` cannot run within an interrupt service routine, care is required to
attend these interrupts. In particular, to clear the interrupt signal from the expander,
the input port register is read. Consequently, the arduino interrupt service must only
set a flag to signal a routine running in `loop( )` to read the expander port, and the
arduino must not respond to another interrupt from the expander until after the port is
read. Using one edge triggered interrupts prevents the level from the expander from causing
another interrupt in the arduino while the port is being cleared.

Also, some devices (MCP23017) require configuration setup to use interrupts -- see the
next section, and the example `shrRotaryi17`.

####Access to all registers
Some port expanders have additional features such as input polarity inversion, interrupt 
selection, and so on. These features are not always provided with the existing libraries 
because the main purpose of the libraries is to render the ports' access functionally alike
to simplify a user program's ability to substitute one port for another. Consequently, the
simplest port's features are taken to define the functionality of all.

Nevertheless, the libraries `shrMC17` and `shrPrta` are now revised by adding a pair of 
register read/write functions which permit access to all of the chip's registers so that 
more of the flexible configurations available to the supported devices can be used. The 
functions are:

```cpp

	void reg_write( byte register, word value );
	word reg_read( byte register );
```

where `register` is the address within the expander to be accessed, and `value` is the 16-bit
word to write to the register. The libraries have defined literals for the names of the registers
and for the names of the bits of the MC23017 `IOCON` register. Note that the r/w operations for
the MCP23017 _assume_ that it is being used as a 16-bit port, so the `BANK` bit in `IOCON` _must_ 
remain as its power-on value of 0. This also means that the address-incrementing must remain on
and that both addresses of `IOCON` are written/read. See the interrupt setup example in `shrMC17,
examples, shrRotaryi17` that illustrates these considerations.

The shrPrta devices are simpler than the MCP23017 and the `reg_read/reg_write` functions only add
access to the input inversion feature to the whole-port functions already available in all libraries.


