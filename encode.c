/*
    Copyright 1990, 1998, 2005, 2006 by Al Williams (alw@al-williams.com)

    This file is part of Binds

    Binds is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    Binds is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Binds; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA


*/
#include <string.h>
#include "config.h"

#if HAVE_MEMFROB != 1
void *memfrob(char *s, size_t len)
{
  char *s0=s;
  while (len--) {
    *s=*s^0x2A;
    s++;
  }
  return s0;
}

#endif


char *encode(char *s, size_t len, size_t *lenout)
{
  if (lenout) *lenout=len;
  memfrob(s,len);
  return s;
}
