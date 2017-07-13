/*

gbmtrunc.c - Truncate to lower bits per pixel

*/

/*...sincludes:0:*/
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "gbm.h"

/*...vgbm\46\h:0:*/
/*...e*/
/*...svars:0:*/
static BOOLEAN inited = FALSE;

/*
For 6Rx6Gx6B, 7Rx8Gx4B palettes etc.
*/

static byte index4[0x100];
static byte index6[0x100];
static byte index7[0x100];
static byte index8[0x100];
static byte index16[0x100];
static byte scale4[] = { 0, 85, 170, 255 };
static byte scale6[] = { 0, 51, 102, 153, 204, 255 };
static byte scale7[] = { 0, 43, 85, 128, 170, 213, 255 };
static byte scale8[] = { 0, 36, 73, 109, 146, 182, 219, 255 };
static byte scale16[] = { 0, 17, 34, 51, 68, 85, 102, 119, 136,
			   153, 170, 187, 204, 221, 238, 255 };
/*...e*/
/*...sinit:0:*/
/*
This function initialises this module.
*/

/*...snearest_inx:0:*/
#ifndef abs
#define	abs(x)	(((x)>=0)?(x):-(x))
#endif

static byte nearest_inx(byte value, const byte ab[], unsigned short cb)
	{
	byte b, inx, inx_min;
	short diff, diff_min;

	b = ab[0];
	diff_min = abs((short) value - (short) b);
	inx_min = 0;
	for ( inx = 1; (unsigned short) inx < cb; inx++ )
		{
		b = ab[inx];
		diff = abs((short) value - (short) b);
		if ( diff < diff_min )
			{
			diff_min = diff;
			inx_min = inx;
			}
		}
	return inx_min;
	}
/*...e*/

static void init(void)
	{
	int i;

	if ( inited )
		return;

	/* For 7 Red x 8 Green x 4 Blue palettes etc. */

	for ( i = 0; i < 0x100; i++ )
		{
		index4 [i] = nearest_inx((byte) i, scale4 , sizeof(scale4 ));
		index6 [i] = nearest_inx((byte) i, scale6 , sizeof(scale6 ));
		index7 [i] = nearest_inx((byte) i, scale7 , sizeof(scale7 ));
		index8 [i] = nearest_inx((byte) i, scale8 , sizeof(scale8 ));
		index16[i] = nearest_inx((byte) i, scale16, sizeof(scale16));
		}

	inited = TRUE;
	}
/*...e*/
/*...strunc:0:*/
static void trunc(
	const GBM *gbm, const byte *src, byte *dest,
	int dest_bpp,
	void (*trunc_line)(const byte *src, byte *dest, int cx)
	)
	{
	int stride_src = ((gbm->w * 3 + 3) & ~3);
	int stride_dest = ((gbm->w * dest_bpp + 31) / 32) * 4;
	int y;

	for ( y = 0; y < gbm->h; y++ )
		(*trunc_line)(src + y * stride_src, dest + y * stride_dest, gbm->w);
	}
/*...e*/
/*...snearest_color:0:*/
static byte nearest_color(byte r, byte g, byte b, GBMRGB *gbmrgb, int n_gbmrgb)
	{
	int i, i_min, dist_min = 0x30000;
	for ( i = 0; i < n_gbmrgb; i++ )
		{
		int dr = (int) ( (unsigned)r - (unsigned)gbmrgb[i].r );
		int dg = (int) ( (unsigned)g - (unsigned)gbmrgb[i].g );
		int db = (int) ( (unsigned)b - (unsigned)gbmrgb[i].b );
		int dist = dr*dr+dg*dg+db*db;
		if ( dist < dist_min )
			{ dist_min = dist; i_min = i; }
		}
	return (byte) i_min;
	}
/*...e*/
/*...sMAP:0:*/
/* A map is a structure used to accelerate the conversion from r,g,b tuple
   to palette index. A map divides RGB space into a cube. Each cube either
   maps all its RGB space to a single index, or 0xffff is stored. */

typedef struct
	{
	word inx[0x20][0x20][0x20]; /* 64KB */
	} MAP;

static void build_map(GBMRGB *gbmrgb, int n_gbmrgb, MAP *map)
	{
	int r, g, b;
	for ( r = 0; r < 0x100; r += 8 )
		for ( g = 0; g < 0x100; g += 8 )
			for ( b = 0; b < 0x100; b += 8 )
				{
				byte i  = nearest_color((byte)  r   , (byte)  g   , (byte)  b   , gbmrgb, n_gbmrgb);
				if ( i == nearest_color((byte)  r   , (byte)  g   , (byte) (b+7), gbmrgb, n_gbmrgb) &&
				     i == nearest_color((byte)  r   , (byte) (g+7), (byte)  b   , gbmrgb, n_gbmrgb) &&
				     i == nearest_color((byte)  r   , (byte) (g+7), (byte) (b+7), gbmrgb, n_gbmrgb) &&
				     i == nearest_color((byte) (r+7), (byte)  g   , (byte)  b   , gbmrgb, n_gbmrgb) &&
				     i == nearest_color((byte) (r+7), (byte)  g   , (byte) (b+7), gbmrgb, n_gbmrgb) &&
				     i == nearest_color((byte) (r+7), (byte) (g+7), (byte)  b   , gbmrgb, n_gbmrgb) &&
				     i == nearest_color((byte) (r+7), (byte) (g+7), (byte) (b+7), gbmrgb, n_gbmrgb) )
					map->inx[r/8][g/8][b/8] = i;
				else
					map->inx[r/8][g/8][b/8] = (word) 0xffff;
				}
	}

static byte nearest_color_via_map(byte r, byte g, byte b, MAP *map, GBMRGB *gbmrgb, int n_gbmrgb)
	{
	word i = map->inx[r/8][g/8][b/8];
	return ( i != (word) 0xffff )
		? (byte) i
		: nearest_color(r, g, b, gbmrgb, n_gbmrgb);
	}
/*...e*/

/*...sgbm_trunc_line_24     \45\ truncate to fewer bits per pixel one line:0:*/
void gbm_trunc_line_24(const byte *src, byte *dest, int cx, byte rm, byte gm, byte bm)
	{
	int x;

	for ( x = 0; x < cx; x++ )
		{
		*dest++ = (*src++ & bm);
		*dest++ = (*src++ & gm);
		*dest++ = (*src++ & rm);
		}
	}
/*...e*/
/*...sgbm_trunc_24          \45\ truncate to fewer bits per pixel:0:*/
void gbm_trunc_24(const GBM *gbm, const byte *data24, byte *data8, byte rm, byte gm, byte bm)
	{
	int	stride = ((gbm->w * 3 + 3) & ~3);
	int	y;

	for ( y = 0; y < gbm->h; y++ )
		gbm_trunc_line_24(data24 + y * stride, data8 + y * stride, gbm->w, rm, gm, bm);
	}
/*...e*/

/*...sgbm_trunc_pal_6R6G6B  \45\ return 6Rx6Gx6B palette:0:*/
/*
This function makes the palette for the 6 red x 6 green x 6 blue palette.
216 palette entrys used. Remaining 40 left blank.
*/

void gbm_trunc_pal_6R6G6B(GBMRGB *gbmrgb)
	{
	byte volatile r;	/* C-Set/2 optimiser fix */
	byte volatile g;
	byte volatile b;

	init();
	memset(gbmrgb, 0x80, 0x100 * sizeof(GBMRGB));
	for ( r = 0; r < 6; r++ )
		for ( g = 0; g < 6; g++ )
			for ( b = 0; b < 6; b++ )
				{
				gbmrgb->r = scale6[r];
				gbmrgb->g = scale6[g];
				gbmrgb->b = scale6[b];
				gbmrgb++;
				}
	}
/*...e*/
/*...sgbm_trunc_line_6R6G6B \45\ truncate to 6Rx6Gx6B one line:0:*/
void gbm_trunc_line_6R6G6B(const byte *src, byte *dest, int cx)
	{
	int x;

	init();

	for ( x = 0; x < cx; x++ )
		{
		byte bi = index6[*src++];
		byte gi = index6[*src++];
		byte ri = index6[*src++];

		*dest++ = (byte) (6 * (6 * ri + gi) + bi);
		}
	}
/*...e*/
/*...sgbm_trunc_6R6G6B      \45\ truncate to 6Rx6Gx6B:0:*/
void gbm_trunc_6R6G6B(const GBM *gbm, const byte *data24, byte *data8)
	{
	trunc(gbm, data24, data8, 8, gbm_trunc_line_6R6G6B);
	}
/*...e*/

/*...sgbm_trunc_pal_7R8G4B  \45\ return 7Rx8Gx4B palette:0:*/
/*
This function makes the palette for the 7 red x 8 green x 4 blue palette.
224 palette entrys used. Remaining 32 left blank.
Colours calculated to match those used by 8514/A PM driver.
*/

void gbm_trunc_pal_7R8G4B(GBMRGB *gbmrgb)
	{
	byte volatile r;	/* C-Set/2 optimiser fix */
	byte volatile g;
	byte volatile b;

	init();

	memset(gbmrgb, 0x80, 0x100 * sizeof(GBMRGB));
	for ( r = 0; r < 7; r++ )
		for ( g = 0; g < 8; g++ )
			for ( b = 0; b < 4; b++ )
				{
				gbmrgb->r = scale7[r];
				gbmrgb->g = scale8[g];
				gbmrgb->b = scale4[b];
				gbmrgb++;
				}
	}
/*...e*/
/*...sgbm_trunc_line_7R8G4B \45\ truncate to 7Rx8Gx4B one line:0:*/
void gbm_trunc_line_7R8G4B(const byte *src, byte *dest, int cx)
	{
	int x;

	init();

	for ( x = 0; x < cx; x++ )
		{
		byte bi = index4[*src++];
		byte gi = index8[*src++];
		byte ri = index7[*src++];

		*dest++ = (byte) (4 * (8 * ri + gi) + bi);
		}
	}
/*...e*/
/*...sgbm_trunc_7R8G4B      \45\ truncate to 7Rx8Gx4B:0:*/
void gbm_trunc_7R8G4B(const GBM *gbm, const byte *data24, byte *data8)
	{
	trunc(gbm, data24, data8, 8, gbm_trunc_line_7R8G4B);
	}
/*...e*/

/*...sgbm_trunc_pal_VGA     \45\ return default VGA palette:0:*/
/*
This function makes the palette for the 16 colour VGA palette.
*/

static GBMRGB gbmrgb_vga[] =
	{
	  0,  0,  0,
	128,  0,  0,
	  0,128,  0,
	128,128,  0,
	  0,  0,128,
	128,  0,128,
	  0,128,128,
	128,128,128,
	204,204,204,
	255,  0,  0,
	  0,255,  0,
	255,255,  0,
	  0,  0,255,
	255,  0,255,
	  0,255,255,
	255,255,255,
	};

void gbm_trunc_pal_VGA(GBMRGB *gbmrgb)
	{
	init();
	memcpy((char *) gbmrgb, (char *) gbmrgb_vga, sizeof(gbmrgb_vga));
	}
/*...e*/
/*...sgbm_trunc_line_VGA    \45\ truncate to default VGA palette:0:*/
/*...scalc_nearest:0:*/
/*
This function, when given am RGB colour, finds the VGA palette entry closest
to it. We deliberately bias away from the two grey palette entries.
*/

static byte calc_nearest(byte r, byte g, byte b)
	{
	long min_dist = 3L * 256L * 256L * 10L;
	byte bi, bi_min;

	for ( bi = 0; bi < 0x10; bi++ )
		{
		long b_dist = ((long) b - (long) gbmrgb_vga[bi].b);
		long g_dist = ((long) g - (long) gbmrgb_vga[bi].g);
		long r_dist = ((long) r - (long) gbmrgb_vga[bi].r);
		long dist = r_dist * r_dist + g_dist * g_dist + b_dist * b_dist;

		if ( dist < min_dist )
			{
			min_dist = dist;
			bi_min = bi;
			}
		}
	return bi_min;
	}
/*...e*/
/*...snearest_colour:0:*/
/*
This function finds the closest VGA palette colour to a given RGB value.
It uses a lookup table to avoid performing distance calculations to the
16 palette entrys. The table is pre-calculated.
*/

/*...squick lookup table:0:*/
/*...v_gbmtrun\46\c \45\ used to make quick_tab:0:*/

static byte quick_tab[16][16][16] =
	{
	0,0,0,0,255,4,4,4,4,4,4,4,12,12,12,12,
	0,0,0,0,255,4,4,4,4,4,4,4,12,12,12,12,
	0,0,0,0,255,4,4,4,4,4,4,4,12,12,12,12,
	0,0,0,0,255,4,4,4,4,4,4,4,12,12,12,12,
	255,255,255,255,255,255,255,255,255,255,255,255,255,12,12,12,
	2,2,2,2,255,6,6,6,6,6,6,6,6,255,12,12,
	2,2,2,2,255,6,6,6,6,6,6,6,6,6,255,12,
	2,2,2,2,255,6,6,6,6,6,6,6,6,6,6,255,
	2,2,2,2,255,6,6,6,6,6,6,6,6,6,6,255,
	2,2,2,2,255,6,6,6,6,6,6,6,6,6,255,14,
	2,2,2,2,255,6,6,6,6,6,6,6,6,255,14,14,
	2,2,2,2,255,6,6,6,6,6,6,6,255,14,14,14,
	10,10,10,10,255,6,6,6,6,6,6,255,14,14,14,14,
	10,10,10,10,10,255,6,6,6,6,255,14,14,14,14,14,
	10,10,10,10,10,10,255,6,6,255,14,14,14,14,14,14,
	10,10,10,10,10,10,10,255,255,14,14,14,14,14,14,14,
	0,0,0,0,255,4,4,4,4,4,4,4,12,12,12,12,
	0,0,0,0,255,4,4,4,4,4,4,4,12,12,12,12,
	0,0,0,0,255,4,4,4,4,4,4,4,12,12,12,12,
	0,0,0,0,255,4,4,4,4,4,4,4,12,12,12,12,
	255,255,255,255,255,255,255,255,255,255,255,255,255,12,12,12,
	2,2,2,2,255,6,6,6,6,6,6,6,6,255,12,12,
	2,2,2,2,255,6,6,6,6,6,6,6,6,6,255,12,
	2,2,2,2,255,6,6,6,6,6,6,6,6,6,6,255,
	2,2,2,2,255,6,6,6,6,6,6,6,6,6,6,255,
	2,2,2,2,255,6,6,6,6,6,6,6,6,6,255,14,
	2,2,2,2,255,6,6,6,6,6,6,6,6,255,14,14,
	2,2,2,2,255,6,6,6,6,6,6,6,255,14,14,14,
	10,10,10,10,255,6,6,6,6,6,6,255,14,14,14,14,
	10,10,10,10,10,255,6,6,6,6,255,14,14,14,14,14,
	10,10,10,10,10,10,255,6,6,255,14,14,14,14,14,14,
	10,10,10,10,10,10,10,255,255,14,14,14,14,14,14,14,
	0,0,0,0,255,4,4,4,4,4,4,4,12,12,12,12,
	0,0,0,0,255,4,4,4,4,4,4,4,12,12,12,12,
	0,0,0,0,255,4,4,4,4,4,4,4,12,12,12,12,
	0,0,0,0,255,4,4,4,4,4,4,4,12,12,12,12,
	255,255,255,255,255,255,255,255,255,255,255,255,255,12,12,12,
	2,2,2,2,255,6,6,6,6,6,6,6,6,255,12,12,
	2,2,2,2,255,6,6,6,6,6,6,6,6,6,255,12,
	2,2,2,2,255,6,6,6,6,6,6,6,6,6,6,255,
	2,2,2,2,255,6,6,6,6,6,6,6,6,6,6,255,
	2,2,2,2,255,6,6,6,6,6,6,6,6,6,255,14,
	2,2,2,2,255,6,6,6,6,6,6,6,6,255,14,14,
	2,2,2,2,255,6,6,6,6,6,6,6,255,14,14,14,
	10,10,10,10,255,6,6,6,6,6,6,255,14,14,14,14,
	10,10,10,10,10,255,6,6,6,6,255,14,14,14,14,14,
	10,10,10,10,10,10,255,6,6,255,14,14,14,14,14,14,
	10,10,10,10,10,10,10,255,255,14,14,14,14,14,14,14,
	0,0,0,0,255,4,4,4,4,4,4,4,12,12,12,12,
	0,0,0,0,255,4,4,4,4,4,4,4,12,12,12,12,
	0,0,0,0,255,4,4,4,4,4,4,4,12,12,12,12,
	0,0,0,0,255,4,4,4,4,4,4,4,12,12,12,12,
	255,255,255,255,255,255,255,255,255,255,255,255,255,12,12,12,
	2,2,2,2,255,6,6,6,6,6,6,6,6,255,12,12,
	2,2,2,2,255,6,6,6,6,6,6,6,6,6,255,12,
	2,2,2,2,255,6,6,6,6,6,6,6,6,6,6,255,
	2,2,2,2,255,6,6,6,6,6,6,6,6,6,6,255,
	2,2,2,2,255,6,6,6,6,6,6,6,6,6,255,14,
	2,2,2,2,255,6,6,6,6,6,6,6,6,255,14,14,
	2,2,2,2,255,6,6,6,6,6,6,6,255,14,14,14,
	10,10,10,10,255,6,6,6,6,6,6,255,14,14,14,14,
	10,10,10,10,10,255,6,6,6,6,255,14,14,14,14,14,
	10,10,10,10,10,10,255,6,6,255,14,14,14,14,14,14,
	10,10,10,10,10,10,10,255,255,14,14,14,14,14,14,14,
	255,255,255,255,255,255,255,255,255,255,255,255,255,12,12,12,
	255,255,255,255,255,255,255,255,255,255,255,255,255,12,12,12,
	255,255,255,255,255,255,255,255,255,255,255,255,255,12,12,12,
	255,255,255,255,255,255,255,255,255,255,255,255,255,12,12,12,
	255,255,255,255,255,255,255,255,255,255,255,255,255,255,12,12,
	255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,12,
	255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
	255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
	255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
	255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
	255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,14,
	255,255,255,255,255,255,255,255,255,255,255,255,255,255,14,14,
	255,255,255,255,255,255,255,255,255,255,255,255,255,14,14,14,
	10,10,10,10,255,255,255,255,255,255,255,255,14,14,14,14,
	10,10,10,10,10,255,255,255,255,255,255,14,14,14,14,14,
	10,10,10,10,10,10,255,255,255,255,14,14,14,14,14,14,
	1,1,1,1,255,5,5,5,5,5,5,5,5,255,12,12,
	1,1,1,1,255,5,5,5,5,5,5,5,5,255,12,12,
	1,1,1,1,255,5,5,5,5,5,5,5,5,255,12,12,
	1,1,1,1,255,5,5,5,5,5,5,5,5,255,12,12,
	255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,12,
	3,3,3,3,255,7,7,7,7,7,7,7,7,7,255,255,
	3,3,3,3,255,7,7,7,7,7,7,7,7,7,7,255,
	3,3,3,3,255,7,7,7,7,7,7,7,7,7,7,7,
	3,3,3,3,255,7,7,7,7,7,7,7,7,7,7,7,
	3,3,3,3,255,7,7,7,7,7,7,7,7,7,7,255,
	3,3,3,3,255,7,7,7,7,7,7,7,7,7,255,255,
	3,3,3,3,255,7,7,7,7,7,7,7,7,255,255,255,
	3,3,3,3,255,7,7,7,7,7,7,7,255,255,255,14,
	255,255,255,255,255,7,7,7,7,7,7,255,255,255,14,14,
	10,10,10,10,255,255,7,7,7,7,255,255,255,14,14,14,
	10,10,10,10,10,255,255,7,7,255,255,255,14,14,14,14,
	1,1,1,1,255,5,5,5,5,5,5,5,5,5,255,12,
	1,1,1,1,255,5,5,5,5,5,5,5,5,5,255,12,
	1,1,1,1,255,5,5,5,5,5,5,5,5,5,255,12,
	1,1,1,1,255,5,5,5,5,5,5,5,5,5,255,12,
	255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
	3,3,3,3,255,7,7,7,7,7,7,7,7,7,7,255,
	3,3,3,3,255,7,7,7,7,7,7,7,7,7,7,7,
	3,3,3,3,255,7,7,7,7,7,7,7,7,7,7,7,
	3,3,3,3,255,7,7,7,7,7,7,7,7,7,7,255,
	3,3,3,3,255,7,7,7,7,7,7,7,7,7,255,255,
	3,3,3,3,255,7,7,7,7,7,7,7,7,255,255,255,
	3,3,3,3,255,7,7,7,7,7,7,7,255,255,255,255,
	3,3,3,3,255,7,7,7,7,7,7,255,255,255,255,255,
	3,3,3,3,255,7,7,7,7,7,255,255,255,255,255,255,
	255,255,255,255,255,7,7,7,7,255,255,255,255,255,255,255,
	10,10,10,10,255,255,7,7,255,255,255,255,255,255,255,255,
	1,1,1,1,255,5,5,5,5,5,5,5,5,5,5,255,
	1,1,1,1,255,5,5,5,5,5,5,5,5,5,5,255,
	1,1,1,1,255,5,5,5,5,5,5,5,5,5,5,255,
	1,1,1,1,255,5,5,5,5,5,5,5,5,5,5,255,
	255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
	3,3,3,3,255,7,7,7,7,7,7,7,7,7,7,7,
	3,3,3,3,255,7,7,7,7,7,7,7,7,7,7,7,
	3,3,3,3,255,7,7,7,7,7,7,7,7,7,7,255,
	3,3,3,3,255,7,7,7,7,7,7,7,7,7,255,255,
	3,3,3,3,255,7,7,7,7,7,7,7,7,255,255,255,
	3,3,3,3,255,7,7,7,7,7,7,7,255,255,255,8,
	3,3,3,3,255,7,7,7,7,7,7,255,255,255,8,8,
	3,3,3,3,255,7,7,7,7,7,255,255,255,8,8,8,
	3,3,3,3,255,7,7,7,7,255,255,255,8,8,8,8,
	3,3,3,3,255,7,7,7,255,255,255,8,8,8,8,8,
	255,255,255,255,255,7,7,255,255,255,8,8,8,8,8,255,
	1,1,1,1,255,5,5,5,5,5,5,5,5,5,5,255,
	1,1,1,1,255,5,5,5,5,5,5,5,5,5,5,255,
	1,1,1,1,255,5,5,5,5,5,5,5,5,5,5,255,
	1,1,1,1,255,5,5,5,5,5,5,5,5,5,5,255,
	255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
	3,3,3,3,255,7,7,7,7,7,7,7,7,7,7,7,
	3,3,3,3,255,7,7,7,7,7,7,7,7,7,7,255,
	3,3,3,3,255,7,7,7,7,7,7,7,7,7,255,255,
	3,3,3,3,255,7,7,7,7,7,7,7,7,255,255,255,
	3,3,3,3,255,7,7,7,7,7,7,7,255,255,255,8,
	3,3,3,3,255,7,7,7,7,7,7,255,255,255,8,8,
	3,3,3,3,255,7,7,7,7,7,255,255,255,8,8,8,
	3,3,3,3,255,7,7,7,7,255,255,255,8,8,8,8,
	3,3,3,3,255,7,7,7,255,255,255,8,8,8,8,8,
	3,3,3,3,255,7,7,255,255,255,8,8,8,8,8,8,
	255,255,255,255,255,7,255,255,255,8,8,8,8,8,8,8,
	1,1,1,1,255,5,5,5,5,5,5,5,5,5,255,13,
	1,1,1,1,255,5,5,5,5,5,5,5,5,5,255,13,
	1,1,1,1,255,5,5,5,5,5,5,5,5,5,255,13,
	1,1,1,1,255,5,5,5,5,5,5,5,5,5,255,13,
	255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
	3,3,3,3,255,7,7,7,7,7,7,7,7,7,7,255,
	3,3,3,3,255,7,7,7,7,7,7,7,7,7,255,255,
	3,3,3,3,255,7,7,7,7,7,7,7,7,255,255,255,
	3,3,3,3,255,7,7,7,7,7,7,7,255,255,255,8,
	3,3,3,3,255,7,7,7,7,7,7,255,255,255,8,8,
	3,3,3,3,255,7,7,7,7,7,255,255,255,8,8,8,
	3,3,3,3,255,7,7,7,7,255,255,255,8,8,8,8,
	3,3,3,3,255,7,7,7,255,255,255,8,8,8,8,8,
	3,3,3,3,255,7,7,255,255,255,8,8,8,8,8,8,
	255,255,255,255,255,7,255,255,255,8,8,8,8,8,8,8,
	11,11,11,11,255,255,255,255,8,8,8,8,8,8,8,8,
	1,1,1,1,255,5,5,5,5,5,5,5,5,255,13,13,
	1,1,1,1,255,5,5,5,5,5,5,5,5,255,13,13,
	1,1,1,1,255,5,5,5,5,5,5,5,5,255,13,13,
	1,1,1,1,255,5,5,5,5,5,5,5,5,255,13,13,
	255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,13,
	3,3,3,3,255,7,7,7,7,7,7,7,7,7,255,255,
	3,3,3,3,255,7,7,7,7,7,7,7,7,255,255,255,
	3,3,3,3,255,7,7,7,7,7,7,7,255,255,255,8,
	3,3,3,3,255,7,7,7,7,7,7,255,255,255,8,8,
	3,3,3,3,255,7,7,7,7,7,255,255,255,8,8,8,
	3,3,3,3,255,7,7,7,7,255,255,255,8,8,8,8,
	3,3,3,3,255,7,7,7,255,255,255,8,8,8,8,8,
	3,3,3,3,255,7,7,255,255,255,8,8,8,8,8,8,
	255,255,255,255,255,7,255,255,255,8,8,8,8,8,8,8,
	11,11,11,11,255,255,255,255,8,8,8,8,8,8,8,8,
	11,11,11,11,11,255,255,8,8,8,8,8,8,8,8,8,
	1,1,1,1,255,5,5,5,5,5,5,5,255,13,13,13,
	1,1,1,1,255,5,5,5,5,5,5,5,255,13,13,13,
	1,1,1,1,255,5,5,5,5,5,5,5,255,13,13,13,
	1,1,1,1,255,5,5,5,5,5,5,5,255,13,13,13,
	255,255,255,255,255,255,255,255,255,255,255,255,255,255,13,13,
	3,3,3,3,255,7,7,7,7,7,7,7,7,255,255,255,
	3,3,3,3,255,7,7,7,7,7,7,7,255,255,255,255,
	3,3,3,3,255,7,7,7,7,7,7,255,255,255,8,8,
	3,3,3,3,255,7,7,7,7,7,255,255,255,8,8,8,
	3,3,3,3,255,7,7,7,7,255,255,255,8,8,8,8,
	3,3,3,3,255,7,7,7,255,255,255,8,8,8,8,8,
	3,3,3,3,255,7,7,255,255,255,8,8,8,8,8,8,
	255,255,255,255,255,7,255,255,255,8,8,8,8,8,8,8,
	11,11,11,11,255,255,255,255,8,8,8,8,8,8,8,8,
	11,11,11,11,11,255,255,8,8,8,8,8,8,8,8,8,
	11,11,11,11,11,255,255,8,8,8,8,8,8,8,8,255,
	9,9,9,9,255,5,5,5,5,5,5,255,13,13,13,13,
	9,9,9,9,255,5,5,5,5,5,5,255,13,13,13,13,
	9,9,9,9,255,5,5,5,5,5,5,255,13,13,13,13,
	9,9,9,9,255,5,5,5,5,5,5,255,13,13,13,13,
	255,255,255,255,255,255,255,255,255,255,255,255,255,13,13,13,
	3,3,3,3,255,7,7,7,7,7,7,7,255,255,255,13,
	3,3,3,3,255,7,7,7,7,7,7,255,255,255,255,255,
	3,3,3,3,255,7,7,7,7,7,255,255,255,8,8,8,
	3,3,3,3,255,7,7,7,7,255,255,255,8,8,8,8,
	3,3,3,3,255,7,7,7,255,255,255,8,8,8,8,8,
	3,3,3,3,255,7,7,255,255,255,8,8,8,8,8,8,
	255,255,255,255,255,7,255,255,255,8,8,8,8,8,8,8,
	11,11,11,11,255,255,255,255,8,8,8,8,8,8,8,8,
	11,11,11,11,11,255,255,8,8,8,8,8,8,8,8,8,
	11,11,11,11,11,255,255,8,8,8,8,8,8,8,8,255,
	11,11,11,11,11,11,255,8,8,8,8,8,8,8,255,255,
	9,9,9,9,9,255,5,5,5,5,255,13,13,13,13,13,
	9,9,9,9,9,255,5,5,5,5,255,13,13,13,13,13,
	9,9,9,9,9,255,5,5,5,5,255,13,13,13,13,13,
	9,9,9,9,9,255,5,5,5,5,255,13,13,13,13,13,
	9,9,9,9,255,255,255,255,255,255,255,255,13,13,13,13,
	255,255,255,255,255,7,7,7,7,7,7,255,255,255,13,13,
	3,3,3,3,255,7,7,7,7,7,255,255,255,255,255,255,
	3,3,3,3,255,7,7,7,7,255,255,255,8,8,8,8,
	3,3,3,3,255,7,7,7,255,255,255,8,8,8,8,8,
	3,3,3,3,255,7,7,255,255,255,8,8,8,8,8,8,
	255,255,255,255,255,7,255,255,255,8,8,8,8,8,8,8,
	11,11,11,11,255,255,255,255,8,8,8,8,8,8,8,8,
	11,11,11,11,11,255,255,8,8,8,8,8,8,8,8,8,
	11,11,11,11,11,255,255,8,8,8,8,8,8,8,8,255,
	11,11,11,11,11,11,255,8,8,8,8,8,8,8,255,255,
	11,11,11,11,11,11,255,8,8,8,8,8,8,255,255,255,
	9,9,9,9,9,9,255,5,5,255,13,13,13,13,13,13,
	9,9,9,9,9,9,255,5,5,255,13,13,13,13,13,13,
	9,9,9,9,9,9,255,5,5,255,13,13,13,13,13,13,
	9,9,9,9,9,9,255,5,5,255,13,13,13,13,13,13,
	9,9,9,9,9,255,255,255,255,255,255,13,13,13,13,13,
	9,9,9,9,255,255,7,7,7,7,255,255,255,13,13,13,
	255,255,255,255,255,7,7,7,7,255,255,255,255,255,255,255,
	3,3,3,3,255,7,7,7,255,255,255,8,8,8,8,8,
	3,3,3,3,255,7,7,255,255,255,8,8,8,8,8,8,
	255,255,255,255,255,7,255,255,255,8,8,8,8,8,8,8,
	11,11,11,11,255,255,255,255,8,8,8,8,8,8,8,8,
	11,11,11,11,11,255,255,8,8,8,8,8,8,8,8,8,
	11,11,11,11,11,255,255,8,8,8,8,8,8,8,8,255,
	11,11,11,11,11,11,255,8,8,8,8,8,8,8,255,255,
	11,11,11,11,11,11,255,8,8,8,8,8,8,255,255,255,
	11,11,11,11,11,11,255,8,8,8,8,8,255,255,255,15,
	9,9,9,9,9,9,9,255,255,13,13,13,13,13,13,13,
	9,9,9,9,9,9,9,255,255,13,13,13,13,13,13,13,
	9,9,9,9,9,9,9,255,255,13,13,13,13,13,13,13,
	9,9,9,9,9,9,9,255,255,13,13,13,13,13,13,13,
	9,9,9,9,9,9,255,255,255,255,13,13,13,13,13,13,
	9,9,9,9,9,255,255,7,7,255,255,255,13,13,13,13,
	9,9,9,9,255,255,7,7,255,255,255,255,255,255,255,255,
	255,255,255,255,255,7,7,255,255,255,8,8,8,8,8,255,
	255,255,255,255,255,7,255,255,255,8,8,8,8,8,8,8,
	11,11,11,11,255,255,255,255,8,8,8,8,8,8,8,8,
	11,11,11,11,11,255,255,8,8,8,8,8,8,8,8,8,
	11,11,11,11,11,255,255,8,8,8,8,8,8,8,8,255,
	11,11,11,11,11,11,255,8,8,8,8,8,8,8,255,255,
	11,11,11,11,11,11,255,8,8,8,8,8,8,255,255,255,
	11,11,11,11,11,11,255,8,8,8,8,8,255,255,255,15,
	11,11,11,11,11,11,255,255,8,8,8,255,255,255,15,15,
	};
/*...e*/

static byte nearest_colour(byte r, byte g, byte b)
	{
	byte i;

	if ( (i = quick_tab[r >> 4][g >> 4][b >> 4]) != (byte) 0xff )
		return i;

	return calc_nearest(r, g, b);
	}
/*...e*/

void gbm_trunc_line_VGA(const byte *src, byte *dest, int cx)
	{
	BOOLEAN left = TRUE;
	int x;

	init();

	for ( x = 0; x < cx; x++ )
		{
		byte b   = *src++;
		byte g   = *src++;
		byte r   = *src++;
		byte inx = nearest_colour(r, g, b); 

		if ( left )
			*dest = (byte) (inx << 4);
		else
			*dest++ |= inx;

		left = !left;
		}
	}
/*...e*/
/*...sgbm_trunc_VGA         \45\ truncate to default VGA palette:0:*/
void gbm_trunc_VGA(const GBM *gbm, const byte *data24, byte *data4)
	{
	trunc(gbm, data24, data4, 4, gbm_trunc_line_VGA);
	}
/*...e*/

/*...sgbm_trunc_pal_8       \45\ return default 8 colour palette:0:*/
/*
This function makes the palette for the 16 colour VGA palette.
*/

static GBMRGB gbmrgb_8[] =
	{
	  0,  0,  0,
	  0,  0,255,
	  0,255,  0,
	  0,255,255,
	255,  0,  0,
	255,  0,255,
	255,255,  0,
	255,255,255,
	  0,  0,  0,
	  0,  0,  0,
	  0,  0,  0,
	  0,  0,  0,
	  0,  0,  0,
	  0,  0,  0,
	  0,  0,  0,
	  0,  0,  0,
	};

void gbm_trunc_pal_8(GBMRGB *gbmrgb)
	{
	init();
	memcpy((char *) gbmrgb, (char *) gbmrgb_8, sizeof(gbmrgb_8));
	}
/*...e*/
/*...sgbm_trunc_line_8      \45\ truncate to default 8 colour palette:0:*/
void gbm_trunc_line_8(const byte *src, byte *dest, int cx)
	{
	BOOLEAN left = TRUE;
	int x;

	init();

	for ( x = 0; x < cx; x++ )
		{
		byte b   = ((*src++ & 0x80U) >> 7);
		byte g   = ((*src++ & 0x80U) >> 6);
		byte r   = ((*src++ & 0x80U) >> 5);
		byte inx = r|g|b;

		if ( left )
			*dest = (byte) (inx << 4);
		else
			*dest++ |= inx;

		left = !left;
		}
	}
/*...e*/
/*...sgbm_trunc_8           \45\ truncate to default 8 colour palette:0:*/
void gbm_trunc_8(const GBM *gbm, const byte *data24, byte *data4)
	{
	trunc(gbm, data24, data4, 4, gbm_trunc_line_8);
	}
/*...e*/

/*...sgbm_trunc_pal_4G      \45\ return 4 bit greyscale palette:0:*/
/*
This function makes the palette for the 16 colour VGA palette.
*/

void gbm_trunc_pal_4G(GBMRGB *gbmrgb)
	{
	int i;

	init();
	for ( i = 0; i < 0x10; i++ )
		{
		gbmrgb[i].r = scale16[i];
		gbmrgb[i].g = scale16[i];
		gbmrgb[i].b = scale16[i];
		}
	}
/*...e*/
/*...sgbm_trunc_line_4G     \45\ truncate to 4 bit greyscale palette:0:*/
void gbm_trunc_line_4G(const byte *src, byte *dest, int cx)
	{
	BOOLEAN left = TRUE;
	int x;

	init();

	for ( x = 0; x < cx; x++ )
		{
		byte b   = *src++;
		byte g   = *src++;
		byte r   = *src++;
		byte k   = (byte) (((word) r * 77U + (word) g * 150U + (word) b * 29U) >> 8);
		byte inx = index16[k];

		if ( left )
			*dest = (byte) (inx << 4);
		else
			*dest++ |= inx;

		left = !left;
		}
	}
/*...e*/
/*...sgbm_trunc_4G          \45\ truncate to 4 bit greyscale palette:0:*/
void gbm_trunc_4G(const GBM *gbm, const byte *data24, byte *data4)
	{
	trunc(gbm, data24, data4, 4, gbm_trunc_line_4G);
	}
/*...e*/

/*...sgbm_trunc_pal_BW      \45\ return black and white palette:0:*/
/*
This function makes the black and white palette.
(We consider the image to be black on white, hence the ordering).
*/

static GBMRGB gbmrgb_bw[] =
	{
	255,255,255,
	0,0,0,
	};

void gbm_trunc_pal_BW(GBMRGB *gbmrgb)
	{
	init();
	memcpy((char *) gbmrgb, (char *) gbmrgb_bw, sizeof(gbmrgb_bw));
	}
/*...e*/
/*...sgbm_trunc_line_BW     \45\ truncate to black and white:0:*/
void gbm_trunc_line_BW(const byte *src, byte *dest, int cx)
	{
	int x, bit = 0;
	byte v = 0xff;

	init();

	for ( x = 0; x < cx; x++ )
		{
		byte b = *src++;
		byte g = *src++;
		byte r = *src++;
		word k = (word) ((word) r * 77 + (word) g * 150 + (word) b * 29);

		if ( k >= 0x8000U )
			v &= ~(0x80U >> bit);

		if ( ++bit == 8 )
			{
			bit = 0;
			*dest++ = v;
			v = 0xff;
			}
		}
	if ( bit )
		*dest  = v;
	}
/*...e*/
/*...sgbm_trunc_BW          \45\ truncate to black and white:0:*/
void gbm_trunc_BW(const GBM *gbm, const byte *data24, byte *data1)
	{
	trunc(gbm, data24, data1, 1, gbm_trunc_line_BW);
	}
/*...e*/

/*...sgbm_trunc_1bpp        \45\ map to user specified 1bpp palette:0:*/
void gbm_trunc_1bpp(
	const GBM *gbm, const byte *data24, byte *data1,
	GBMRGB *gbmrgb, int n_gbmrgb
	)
	{
	int stride_src  = ((gbm->w * 3 + 3) & ~3);
	int step_src    = stride_src - gbm->w * 3;
	int stride_dest = ((gbm->w + 31) / 32) * 4;
	int y;
	MAP map;
	build_map(gbmrgb, n_gbmrgb, &map);
	memset(data1, 0, gbm->h * stride_dest);
	for ( y = 0; y < gbm->h; y++ )
		{
		int x;
		for ( x = 0; x < gbm->w; x++ )
			{
			byte b = *data24++;
			byte g = *data24++;
			byte r = *data24++;
			byte i = nearest_color_via_map(r, g, b, &map, gbmrgb, n_gbmrgb);
			if ( i )
				data1[x>>3] |= ( 0x80 >> (x & 7) );
			}
		data24 += step_src;
		data1  += stride_dest;
		}
	}
/*...e*/
/*...sgbm_trunc_4bpp        \45\ map to user specified 4bpp palette:0:*/
void gbm_trunc_4bpp(
	const GBM *gbm, const byte *data24, byte *data4,
	GBMRGB *gbmrgb, int n_gbmrgb
	)
	{
	int stride_src  = ((gbm->w * 3 + 3) & ~3);
	int step_src    = stride_src - gbm->w * 3;
	int stride_dest = ((gbm->w * 4 + 31) / 32) * 4;
	int step_dest   = stride_dest - ((gbm->w * 4)+7)/8;
	int y;
	MAP map;
	build_map(gbmrgb, n_gbmrgb, &map);
	for ( y = 0; y < gbm->h; y++ )
		{
		int x;
		for ( x = 0; x+1 < gbm->w; x += 2 )
			{
			byte b0 = *data24++;
			byte g0 = *data24++;
			byte r0 = *data24++;
			byte i0 = nearest_color_via_map(r0, g0, b0, &map, gbmrgb, n_gbmrgb);
			byte b1 = *data24++;
			byte g1 = *data24++;
			byte r1 = *data24++;
			byte i1 = nearest_color_via_map(r1, g1, b1, &map, gbmrgb, n_gbmrgb);
			*data4++ = ( ( i0 << 4 ) | i1 );
			}
		if ( gbm->w & 1 )
			{
			byte b0 = *data24++;
			byte g0 = *data24++;
			byte r0 = *data24++;
			byte i0 = nearest_color_via_map(r0, g0, b0, &map, gbmrgb, n_gbmrgb);
			*data4++ = ( i0 << 4 );
			}
		data24 += step_src;
		data4  += step_dest;
		}
	}
/*...e*/
/*...sgbm_trunc_8bpp        \45\ map to user specified 8bpp palette:0:*/
void gbm_trunc_8bpp(
	const GBM *gbm, const byte *data24, byte *data8,
	GBMRGB *gbmrgb, int n_gbmrgb
	)
	{
	int stride_src  = ((gbm->w * 3 + 3) & ~3);
	int step_src    = stride_src - gbm->w * 3;
	int stride_dest = ((gbm->w * 8 + 31) / 32) * 4;
	int step_dest   = stride_dest - gbm->w;
	int y;
	MAP map;
	build_map(gbmrgb, n_gbmrgb, &map);
	for ( y = 0; y < gbm->h; y++ )
		{
		int x;
		for ( x = 0; x < gbm->w; x++ )
			{
			byte b = *data24++;
			byte g = *data24++;
			byte r = *data24++;
			*data8++ = nearest_color_via_map(r, g, b, &map, gbmrgb, n_gbmrgb);
			}
		data24 += step_src;
		data8  += step_dest;
		}
	}
/*...e*/
