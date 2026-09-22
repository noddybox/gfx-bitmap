# gfx-bitmap - Basic graphics file format
#
# Copyright (C) 2026  Ian Cowburn (deathstation9000@gmail.com)
# 
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
# 
# -------------------------------------------------------------------------
# 


CFLAGS	=	-g -I.. 

gfx-test: gfx-test.o gfx-bitmap.o
	$(CC) -o gfx-test gfx-test.o gfx-bitmap.o

gfx-test.o: gfx-test.c gfx-bitmap.h
	$(CC) $(CFLAGS) -c gfx-test.c

gfx-bitmap.o: gfx-bitmap.c gfx-bitmap.h
	$(CC) $(CFLAGS) -c gfx-bitmap.c

clean:
	rm -f gfx-test *.o core
