/*
 * libmondai -- MONTSUQI data access library
 * Copyright (C) 1989-2008 Ogochan.
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef	_INC_MISC_H
#define	_INC_MISC_H

#include	<string.h>
#include	"types.h"

#define CLAMP(x, low, high)			(((x) > (high)) ? (high) : (((x) < (low)) ? (low) : (x)))

extern	FILE	*Fopen(char *name, char *mode);
extern	void	MakeCobolX(char *to, size_t len, char *from);
extern	void	CopyCobol(char *to, char *from);
extern	void	AdjustByteOrder(void *to, void *from, size_t size);
extern	void	PrintFixString(char *s, int len);
#endif
