/*

    gfx-bitmap - Basic graphics file format

    Copyright (C) 2026  Ian Cowburn (deathstation9000@gmail.com)

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    -------------------------------------------------------------------------

    gfx-bitmap types and functions

*/
#ifndef GFX_BITMAP_H
#define GFX_BITMAP_H

#include <stddef.h>
#include <stdint.h>

/* ---------------------------------------- TYPES
*/

/* The colours
*/
typedef enum
{
    eGFX_Black = 0,
    eGFX_Blue = 1,
    eGFX_Red = 2,
    eGFX_Magenta = 3,
    eGFX_Green = 4,
    eGFX_Cyan = 5,
    eGFX_Yellow = 6,
    eGFX_White = 7,
    eGFX_Grey = 8,
    eGFX_Num_Colours = 9
} GFX_Bitmap_Colour;

/* Status codes
*/
typedef enum
{
    eGFX_Ok,
    eGFX_InvalidFile,
    eGFX_AllocFailed
} GFX_Bitmap_Status;

/* The bitmap structure.  To allocate the data for encode simply:
	b.data = malloc(b.width * b.height * sizeof *b.data);
   The GFX_BITMAP_AT macro can then be used to write pixel values.
*/
typedef struct
{
    unsigned short	width;
    unsigned short	height;
    uint8_t		*data;
} GFX_Bitmap;

/* Convenience macro to access the pixel data
*/
#define GFX_BITMAP_AT(bitmap, x, y)	(bitmap.data[(x) + (y) * bitmap.width])


/* ---------------------------------------- INTERFACES
*/

/* Decode the passed bitmap file.  On an eGFX_Ok return bitmap.data will
   be a allocated memory which must be freed with free().
*/
GFX_Bitmap_Status	GFX_Bitmap_Decode(const uint8_t *memory, size_t len,
					  GFX_Bitmap *bitmap);

/* Encode the passed bitmap into a valid gfx-bitmap file in memory.
   *memory and *len will only be valid if eGFX_Ok is returned, otherwise they
   will be NULL and zero.
*/
GFX_Bitmap_Status	GFX_Bitmap_Encode(const GFX_Bitmap *bitmap,
					  uint8_t **memory, size_t *len);

#endif
