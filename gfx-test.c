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

    gfx-bitmap test

*/
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>

#include "gfx-bitmap.h"

int main(int argc, char *argv[])
{
    GFX_Bitmap source = {0};
    uint8_t *encoded = NULL;
    size_t len = 0;
    GFX_Bitmap destination = {0};

    srand(time(NULL));

    source.width = 320;
    source.height = 200;
    source.data = malloc(source.width * source.height * sizeof *source.data);
    assert(source.data);

    for(int y = 0; y < source.height; y++)
    {
	for(int x = 0; x < source.width; x++)
	{
	    if (y < eGFX_Num_Colours)
	    {
	    	GFX_BITMAP_AT(source, x, y) = y;
	    }
	    else
	    {
	    	GFX_BITMAP_AT(source, x, y) = rand() % eGFX_Num_Colours;
	    }
	}
    }

    GFX_Bitmap_Status encode_status =
    	GFX_Bitmap_Encode(&source, &encoded, &len);

    assert(encode_status == eGFX_Ok);
    assert(encoded);
    assert(len);

    printf("Length of encoded data = %zu\n", len);

    GFX_Bitmap_Status decode_status =
    	GFX_Bitmap_Decode(encoded, len, &destination);

    assert(decode_status == eGFX_Ok);
    assert(destination.data);
    assert(destination.width == 320);
    assert(destination.height == 200);

    for(int y = 0; y < source.height; y++)
    {
	for(int x = 0; x < source.width; x++)
	{
	    assert(GFX_BITMAP_AT(source, x, y) ==
	    		GFX_BITMAP_AT(destination, x, y));
	}
    }

    return EXIT_SUCCESS;
}
