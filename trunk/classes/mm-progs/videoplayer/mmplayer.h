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


/* Used for sending a click event from the time control to the play control */
#define NOTIFY_CLICK       1 /* For WM_APPTERMINATENOTIFY */
/* Same for context menu */
#define NOTIFY_CONTEXT     2

/* Which display to show */
#define DISPLAY_TIMEELAPSED    0
#define DISPLAY_TIMEREMAINING  1
#define DISPLAY_TIMEID3        2

/* Which track to start */
#define PLAY_FIRST    1L
#define PLAY_NEXT     2L
#define PLAY_PREV     3L
#define PAUSE_TRACK   4L
#define STOP_TRACK    5L
#define SEEK_TRACK    6L

#define  MAIN_BMP_IDX              0
#define  CTRLIDX_PLAYBUTTON        1
#define  CTRLIDX_PLAYBUTTONSEL     2
#define  CTRLIDX_STOPBUTTON        3
#define  CTRLIDX_STOPBUTTONSEL     4
#define  CTRLIDX_PAUSEBUTTON       5
#define  CTRLIDX_PAUSEBUTTONSEL    6
#define  CTRLIDX_POSSLIDER         7
#define  CTRLIDX_POSSLIDERARM      8
#define  CTRLIDX_PLAYTIME          9
#define  CTRLIDX_VOLSLIDER         10
#define  CTRLIDX_VOLSLIDERARM      11
#define  CTRLIDX_VOLSLIDERARMSEL   12

#if 0
#define  CTRLIDX_TOPLEFT           7
#define  CTRLIDX_TOPRIGHT          8
#define  CTRLIDX_BOTTOM            19
#define  CTRLIDX_CHECK             20
#define  CTRLIDX_UNCHECK           21
#define  CTRLIDX_CHECKSEL          22
#define  CTRLIDX_UNCHECKSEL        23
#define  CTRLIDX_RANDOMCHECK             24
#define  CTRLIDX_RANDOMUNCHECK           25
#define  CTRLIDX_RANDOMCHECKSEL          26
#define  CTRLIDX_RANDOMUNCHECKSEL        27
#endif

#define  NUM_CTRL_IDX              13

/* Slider window class name */
#define SLIDER_CLASS     "CWSLIDER"

/* New slider window class            */
/* Data positions in the window words */
#define SLIDERARMWIDTH    0  /* USHORT */
#define SLIDERARMHEIGHT  2  /* USHORT */
#define PTRPOSINSLARM    4  /* USHORT */
#define SLINCREMENTS     6  /* USHORT */
#define SLIDERCX         8  /* ULONG */
#define SLIDERCY         12 /* ULONG */
#define SLIDERX          16 /* ULONG */
#define SLIDERY          20 /* ULONG */
#define SLIDERARMPOS     24 /* LONG  */
#define SLDRAGGING       28 /* BOOL  */
#define SLADDBYTES       32 /* num additional window bytes for slider */

typedef struct
{
  INT id;
  RECTL rclSource;
  HBITMAP hbmSource;
  RECTL rclDest;
}CONTROLINFO;

typedef struct
{
  BITMAPINFOHEADER bmpInfoHdr;
  HBITMAP hbm;
}LOADEDBITMAP;

/* The IDs for the playtime display */
#define IDTIMER_PLAY  5L
#define PLAYTIMER_DELAY 1000

#define IDTIMER_START_PLAYING 6L
#define START_TIMEOUT 500
