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

    gfx-bitmap implementation

*/
#include <stdlib.h>
#include <string.h>
#include "gfx-bitmap.h"

#define GFX_MAGIC	"GFX0"
#define STREAM_BLOCK	256
#define COUNT_BASE	0x0e
#define COUNT_MAX	(0xff - 0x0e)

#define PUSH(s, b, l)					\
    do							\
    {							\
	if (!StreamPush(s, b, l))			\
	{						\
	    return eGFX_AllocFailed;			\
	}						\
    } while (0)

typedef struct
{
    uint8_t	*data;
    size_t	len;
    size_t	alloc;
} Stream;

static const uint8_t *Consume(const uint8_t *source, size_t *source_len,
			      uint8_t *dest, size_t dest_len)
{
    while(dest_len && *source_len)
    {
    	*dest++ = *source++;
	dest_len--;
	(*source_len)--;
    }

    return source;
}

static int StreamPush(Stream *stream, uint8_t *bytes, size_t len)
{
    while(len)
    {
	if (stream->len == stream->alloc)
	{
	    stream->alloc += STREAM_BLOCK;
	    stream->data = realloc(stream->data, stream->alloc);

	    if (!stream->data)
	    {
		return 0;
	    }
	}

	stream->data[stream->len++] = *bytes++;

	len--;
    }

    return 1;
}

GFX_Bitmap_Status GFX_Bitmap_Decode(const uint8_t *memory, size_t len,
				    GFX_Bitmap *bitmap)
{
    if (len < 9)
    {
	return eGFX_InvalidFile;
    }

    uint8_t magic[4];

    memory = Consume(memory, &len, magic, sizeof magic);

    for(int f = 0; f < sizeof magic; f++)
    {
    	if (magic[f] != GFX_MAGIC[f])
	{
	    return eGFX_InvalidFile;
	}
    }

    uint8_t word[2];

    memory = Consume(memory, &len, word, sizeof word);
    bitmap->width = word[0] | ((unsigned short)word[1] << 8);

    memory = Consume(memory, &len, word, sizeof word);
    bitmap->height = word[0] | ((unsigned short)word[1] << 8);

    bitmap->data = malloc(bitmap->width * bitmap->height *
    				sizeof *bitmap->data);

    if (!bitmap->data)
    {
    	return eGFX_AllocFailed;
    }

    uint8_t *dest = bitmap->data;
    uint8_t *end = bitmap->data + bitmap->width * bitmap->height;

    while(len && dest < end)
    {
	uint8_t pixel;

	memory = Consume(memory, &len, &pixel, 1);

	*dest++ = pixel;

	int first_run = 1;

	while(len && dest < end && *memory > 0x0f)
	{
	    uint8_t run;

	    memory = Consume(memory, &len, &run, 1);
	    run -= COUNT_BASE;

	    for(int f = first_run; f < run; f++)
	    {
	    	*dest++ = pixel;
	    }

	    first_run = 0;
	}
    }

    if (len || dest < end)
    {
	free(bitmap->data);
	return eGFX_InvalidFile;
    }

    return eGFX_Ok;
}

GFX_Bitmap_Status GFX_Bitmap_Encode(const GFX_Bitmap *bitmap,
				    uint8_t **memory, size_t *len)
{
    Stream stream = {0};

    PUSH(&stream, GFX_MAGIC, strlen(GFX_MAGIC));

    uint8_t word[2];

    word[0] = bitmap->width & 0xff;
    word[1] = bitmap->width >> 8;

    PUSH(&stream, word, sizeof word);

    word[0] = bitmap->height & 0xff;
    word[1] = bitmap->height >> 8;

    PUSH(&stream, word, sizeof word);

    uint8_t *source = bitmap->data;
    uint8_t *end = bitmap->data + bitmap->width * bitmap->height;

    while(source < end)
    {
    	size_t run = 1;
	uint8_t pixel = *source++;

	PUSH(&stream, &pixel, 1);

	while (source < end && *source == pixel)
	{
	    run++;
	    source++;
	}

	if (run > 1)
	{
	    while (run)
	    {
	    	uint8_t chunk = run > COUNT_MAX ? COUNT_MAX : run;
	    	uint8_t value = chunk + COUNT_BASE;

		PUSH(&stream, &value, 1);

		run -= chunk;
	    }
	}
    }

    *memory = stream.data;
    *len = stream.len;

    return eGFX_Ok;
}
