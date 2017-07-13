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

#ifndef _COMMON_H_INCLUDED
#define _COMMON_H_INCLUDED

#define FIELDOFFSETUL(type, field)    ((ULONG)&(((type *)0)->field))

/* ID tag defines and defines for info querying */
#define IDINFO_NAME              1
#define IDINFO_ARTIST            2
#define IDINFO_ALBUM             3
#define IDINFO_YEAR              4
#define IDINFO_COMMENT           5
#define IDINFO_GENRE             6

#define IDINFO_PLAYTIME          7
#define IDINFO_BPS               8
#define IDINFO_SAMPLERATE        9
#define IDINFO_CHANNELS          10
#define IDINFO_BITRATE           11 /* New with 0.2.7 */

#define IDINFO_LASTINFO          11

#define IDINFO_WIDTH             1
#define IDINFO_HEIGHT            2
#define IDINFO_BPP               3
#define IDINFO_FORMAT            4


#define MRFALSE (MRESULT) FALSE
#define MRTRUE (MRESULT) TRUE

/* Fonts to use in dialogs */
#define DEFAULT_DIALOG_FONT   "9.WarpSans"
#define DEFAULT_DIALOG_FONT_WARP3   "8.Helv"

/* Retore keys from Warp 4 */
#ifndef IDKEY_FDRGRIDINFO
#define IDKEY_FDRGRIDINFO 2938
#endif

#ifndef IDKEY_FDRLONGARRAY
#define IDKEY_FDRLONGARRAY       2931
#endif

#endif
