# gfx-bitmap

Very basic 9 colour bitmap for my emulators.  It uses a simple form of RLE
to slightly compress the data.  The format has changed from the original
implementation used by the emulators so it now stores a version number, width
and height of the bitmap.

# License

This code is released under the GNU General Public License, verion 3.

# Building

There is just a Makefile to build a test program which can be built with `make`.

To use the library, it is intended that it be used as a submodule and the
parent project include the `gfx-bitmap.c` file and reference the path in the
include path so the `gfx-bitmap.h` header can be used.

The code is ISO C, so should be portable to any platform.

# Colours

The gfx-bitmap format has a fixed palette of 9 colours (these will never
change and only be added to) which are encoded as:

|**Pixel Value**|**Colour**|
|-|-|
|0|Black|
|1|Blue|
|2|Red|
|3|Magenta|
|4|Green|
|5|Cyan|
|6|Yellow|
|7|White|
|8|Grey|

Whether the end user actually honours these colours is up to the caller.
The format specifies no RGB values for the colours, so for instance, a caller
would be free to use any shade of grey.

# File format

|**Offset**|**Size**|**Description**|
|-|-|-|
|0|4|Magic string and version number.  Currently GFX0|
|4|2|Width stored as a 16-bit unsigned number, low byte first|
|6|2|Height stored as a 16-bit unsigned number, low byte first|
|8|varies|Bitmap data.  See following text for description.|

Bitmap data is stored as a steam of bytes,  the first byte representing a pixel
value, followed either by another pixel value or a repeat value.  This repeats
till the bitmap is full.

The repeat value is indicated by having a value of 0x10 or over.  The indicates
a repeat value of (value - 0x0e), hence the smallest repeat value is two.

Once the pixel value is repeated the next byte can either be another pixel
value, or a further repeat value to indicate the original value is repeated
again.
