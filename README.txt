arduino library for sharing I2C expansion ports

G. D. (Joe) Young - Jan 22, 2014

Arduino library shrPort provides a 'driver' package of access functions
for I2C bus expansion ports PCF8574 and PCF8575 that approximates the 
individual-pin access capability of the basic arduino (via arduino's 
functions pinMode, digitalWrite, and digitalRead).

Several illustrative examples using shrPort are provided:

  testshrPort - in library's examples folder, uses shrPort alone
  Keypad_shr - in library's examples folder, with Keypad library and
               shrPort makes the Keypad operate on I2C PCF8574/PCF8575
  flashPin/flashPin_shr - flashPin is placed in sketchbook/libraries and
               flashPin_shr in sketchbook. Non-blocking timed pulsing of
               individual arduino pins (flashPin) and similarly, with 
               shrPort, individual pins of I2C expansion ports.


