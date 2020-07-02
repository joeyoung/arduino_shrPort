// MC17bitDef.h - definition of MCP23017 register bits
//
// Started: June 30, 2020  G. D. (Joe) Young <jyoung@islandnet.com>
//
// Revised:
//
// With access to all of the expander port's registers, a convenience is to be able
// to refer to individual bits using the manufacturer's names for the bits.
//

// I/O control register, IOCON reset state 0x00
#define BANK 7      // 0 for 16-bit, 1 for two 8-bit port
#define MIRROR 6    // 0 int outputs connected, 1 separated
#define SEQOP 5     // 0 address incrementing each I2C write, 1 no increment
#define DISSLW 4    // 0 slew rate controlled, 1 slew rate not controlled (faster)
#define HAEN 3      // not used for I2C port
#define ODR 2       // 0 active driver int o/p, 1 open drain, overrides INTPOL
#define INTPOL 1    // 0 active low int o/p, 1 active high o/p
//bit 0 undefined.
