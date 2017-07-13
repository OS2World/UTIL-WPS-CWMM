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

#ifndef _CWMP3INC_H_INCLUDED
#define _CWMP3INC_H_INCLUDED

#include "common.h"

#define MP3FILE_HELP_PANEL  10
#define MP3FILE_HELP_LIBRARY  "cwmm.hlp"
#define MP3FILE_SETTINGS_PANEL 11
#define MP3FILE_SETTINGS_PANEL_CD 13

/* For getting the bitrate using MMIOMP3 0.4.2 or above */
#define FOURCC_MP3 mmioFOURCC('M', 'P', '3', ' ')
#define MP3_COOKIE 0x072D6337

typedef struct _MP3OPTIONS {
int cookie;
long bitrate;
} MP3OPTIONS;

typedef MP3OPTIONS *PMP3OPTIONS;

typedef struct _ID3TAG
{
  char id3Tag[3];
  char id3Name[30];
  char id3Artist[30];
  char id3Album[30];
  char id3Year[4];
  char id3Comment[30];
  char id3Genre[1];

}ID3TAG;

typedef ID3TAG *PID3TAG;

/* Setup strings for setting the ID3 tags */
#if 0
#define SETUP_ID3TRACKNAME    "ID3TRACKNAME"
#define SETUP_ID3ARTIST       "ID3ARTIST"
#define SETUP_ID3ALBUM        "ID3ALBUM"
#define SETUP_ID3COMMENT      "ID3COMMENT"
#define SETUP_ID3YEAR         "ID3YEAR"
#define SETUP_ID3GENRE        "ID3GENRE"
#endif

#define SETUP_ID3TRACKNAME    SETUP_AUDIOTRACKNAME
#define SETUP_ID3ARTIST       SETUP_AUDIOARTIST
#define SETUP_ID3ALBUM        SETUP_AUDIOALBUM
#define SETUP_ID3COMMENT      SETUP_AUDIOCOMMENT
#define SETUP_ID3YEAR         SETUP_AUDIOYEAR
#define SETUP_ID3GENRE        SETUP_AUDIOGENRE


#endif /* */

