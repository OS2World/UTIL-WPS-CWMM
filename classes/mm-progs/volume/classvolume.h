/*
 * (C) Chris Wohlgemuth 2002-2003
 *
 */
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

#define  MAIN_BMP_IDX                     0
#define  CTRLIDX_VOLSLIDER                1
#define  CTRLIDX_VOLSLIDERARM             2
#define  CTRLIDX_VOLSLIDERARMSEL          3

#define  NUM_CTRL_IDX              4

/* Slider window class name */
#define SLIDER_CLASS     "CWSLIDER"

/* New slider window class            */
/* Data positions in the window words */
#define SLIDERARMWIDTH   0  /* USHORT */
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



