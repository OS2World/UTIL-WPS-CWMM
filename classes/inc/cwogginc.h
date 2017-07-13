/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 */          
/*
 * If you need another license for your project/product (commercial,
 * noncommercial, whatever) contact me at
 * 
 * http://www.os2world.com/cdwriting
 * http://www.geocities.com/SiliconValley/Sector/5785/
 *
 */

#ifndef _CWOGGINC_H_INCLUDED
#define _CWOGGINC_H_INCLUDED

#include "common.h"

#define OGGFILE_HELP_PANEL  20
#define OGGFILE_HELP_LIBRARY  "cwmm.hlp"

/* For querying bitrate of OGG-files. */
#define FOURCC_Vorbis mmioFOURCC('O', 'G', 'G', 'S')
#define VORBIS_COOKIE 0x99E59D59

typedef struct _VORBISOPTIONS {
int cookie;
long nominal_bitrate;
long max_bitrate;
long min_bitrate;
} VORBISOPTIONS;

typedef VORBISOPTIONS *PVORBISOPTIONS;

#endif       /*  */
