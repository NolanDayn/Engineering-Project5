#ifndef _UTILS_H
#define _UTILS_H

// General purpose bit-twiddling macros
// WPS 30 April 2020

#define LOW( value )                        ((value) & 0xFF)

// Utility macros to set, clear, and flip specified bits
#define SET_BITS( port, bits )            	((port) |= (bits))
#define CLR_BITS( port, bits )            	((port) &= ~(bits))
#define FLIP_BITS( port, bits )           	((port) ^= (bits))

// Utility macro to force the bits identified by mask to take on the values of the matching bits in value
//#define FORCE_BITS( port, mask, value)	    (port) = ((port) & LOW(~(mask)) | ((value) & (mask)))
#define FORCE_BITS( port, mask, value)	    (port) = (((port) & (~(mask))) | ((value) & (mask)))

#define BIT_IS_SET( port, bits )			((port) & (bits))
#define BIT_IS_CLR( port, bits )			((~(port))&(bits))

#define LO_NYBBLE( value )  ((value) & 0x0F)
#define HI_NYBBLE( value )  (((value) >> 4) & 0x0F)

#endif
