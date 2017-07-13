/*

gbmbpp.c - Change bits per pixel in a General Bitmap

*/

/*...sincludes:0:*/
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdarg.h>
#if defined(AIX) || defined(LINUX) || defined(MAC)
#include <unistd.h>
#else
#include <io.h>
#endif
#include <fcntl.h>
#ifdef MAC
#include <types.h>
#include <stat.h>
#else
#include <sys/types.h>
#include <sys/stat.h>
#endif
#ifndef O_BINARY
#define	O_BINARY	0
#endif
#include "gbm.h"
#include "gbmerr.h"
#include "gbmtrunc.h"
#include "gbmht.h"
#include "gbmhist.h"
#include "gbmmcut.h"

/*...vgbm\46\h:0:*/
/*...vgbmerr\46\h:0:*/
/*...vgbmtrunc\46\h:0:*/
/*...vgbmht\46\h:0:*/
/*...vgbmhist\46\h:0:*/
/*...vgbmmcut\46\h:0:*/
/*...e*/

static char progname[] = "gbmbpp";

/*...sfatal:0:*/
static void fatal(const char *fmt, ...)
	{
	va_list	vars;
	char s[256+1];

	va_start(vars, fmt);
	vsprintf(s, fmt, vars);
	va_end(vars);
	fprintf(stderr, "%s: %s\n", progname, s);
	exit(1);
	}
/*...e*/
/*...susage:0:*/
static void usage(void)
	{
	int ft, n_ft;

	fprintf(stderr, "usage: %s [-m map] [-e] [-hN] [-p fnp.ext{,opt}]\n", progname);
	fprintf(stderr, "              [--] fn1.ext{,opt} [fn2.ext{,opt}]\n");
	fprintf(stderr, "flags: -m map            mapping to perform (default 7x8x4)\n");
	fprintf(stderr, "                         bw           black and white\n");
	fprintf(stderr, "                         vga          16 colour VGA\n");
	fprintf(stderr, "                         8            8 colour (in 4 bit file)\n");
	fprintf(stderr, "                         4g           4 bit greyscale\n");
	fprintf(stderr, "                         7x8x4        7 levels red, 8 green, 4 blue 8514/A\n");
	fprintf(stderr, "                         6x6x6        6 levels red, 6 green, 6 blue\n");
	fprintf(stderr, "                         8g           8 bit greyscale\n");
	fprintf(stderr, "                         tripel       64 reds, 64 greens, 64 blues tripel\n");
	fprintf(stderr, "                         pal1bpp      map to 1bpp, palette specified via -p\n");
	fprintf(stderr, "                         pal4bpp      map to 4bpp, palette specified via -p\n");
	fprintf(stderr, "                         pal8bpp      map to 8bpp, palette specified via -p\n");
	fprintf(stderr, "                         freqR:G:B:N  keep R red, G green, b blue bits, and map\n");
	fprintf(stderr, "                                      to N most used colours in 8 bit palette\n");
	fprintf(stderr, "                         mcutN        median cut to N colours\n");
	fprintf(stderr, "                         R:G:B        keep R red, G green, B blue bits\n");
	fprintf(stderr, "       -e                enable error-diffusion (default is to truncate)\n");
	fprintf(stderr, "                         -e only with -m bw, vga, 4g, 7x8x4, 6x6x6, no -h\n");
	fprintf(stderr, "       -h                enable halftoning (default is to truncate)\n");
	fprintf(stderr, "                         -h only with -m 7x8x4, 6x6x6, 8, vga or R:G:B, no -e\n");
	fprintf(stderr, "                         N is a halftoning algorithm number (default 0)\n");
	fprintf(stderr, "       -p fnp.ext{,opt}  palette is taken from this bitmap\n");
	fprintf(stderr, "       fn1.ext{,opt}     input filename (with any format specific options)\n");
	fprintf(stderr, "       fn2.ext{,opt}     optional output filename (or use fn1 if not present)\n");
	fprintf(stderr, "                         ext's are used to deduce desired bitmap file formats\n");

	gbm_init();
	gbm_query_n_filetypes(&n_ft);
	for ( ft = 0; ft < n_ft; ft++ )
		{
		GBMFT gbmft;

		gbm_query_filetype(ft, &gbmft);
		fprintf(stderr, "                         %s when ext in [%s]\n",
			gbmft.short_name, gbmft.extensions);
		}
	gbm_deinit();

	fprintf(stderr, "       opt's             bitmap format specific options\n");

	exit(1);
	}
/*...e*/
/*...ssame:0:*/
static BOOLEAN same(const char *s1, const char *s2, int n)
	{
	for ( ; n--; s1++, s2++ )
		if ( tolower(*s1) != tolower(*s2) )
			return FALSE;
	return TRUE;
	}
/*...e*/
/*...smain:0:*/
/*...smapinfos:0:*/
#define	CVT_BW		0
#define	CVT_VGA		1
#define	CVT_8		2
#define	CVT_4G		3
#define	CVT_784		4
#define	CVT_666		5
#define	CVT_8G		6
#define	CVT_TRIPEL	7
#define	CVT_PAL1BPP	8
#define	CVT_PAL4BPP	9
#define	CVT_PAL8BPP	10
#define	CVT_RGB		11
#define	CVT_FREQ	12
#define	CVT_MCUT	13
#define	CVT_ERRDIFF	0x4000
#define	CVT_HALFTONE	0x2000

typedef struct { char *name; int m; int dest_bpp; } MAPINFO;

static MAPINFO mapinfos[] =
	{
	"bw",		CVT_BW,		1,
	"vga",		CVT_VGA,	4,
	"8",		CVT_8,		4,
	"4g",		CVT_4G,		4,
	"7x8x4",	CVT_784,	8,
	"6x6x6",	CVT_666,	8,
	"8g",		CVT_8G,		8,
	"tripel",	CVT_TRIPEL,	8,
	"pal1bpp",	CVT_PAL1BPP,	1,
	"pal4bpp",	CVT_PAL4BPP,	4,
	"pal8bpp",	CVT_PAL8BPP,	8,
	};

#define	N_MAPINFOS	(sizeof(mapinfos)/sizeof(mapinfos[0]))
/*...e*/
/*...sget_masks:0:*/
/*
Returns TRUE if a set of masks given at map.
Also sets *rm, *gm, *bm from these.
Else returns FALSE.
*/

static byte mask[] = { 0x00, 0x80, 0xc0, 0xe0, 0xf0, 0xf8, 0xfc, 0xfe, 0xff };

static BOOLEAN get_masks(char *map, byte *rm, byte *gm, byte *bm)
	{
	if ( map[0] <  '0' || map[0] > '8' ||
	     map[1] != ':' ||
	     map[2] <  '0' || map[2] > '8' ||
	     map[3] != ':' ||
	     map[4] <  '0' || map[4] > '8' )
		return FALSE;

	*rm = mask[map[0] - '0'];
	*gm = mask[map[2] - '0'];
	*bm = mask[map[4] - '0'];
	return TRUE;
	}
/*...e*/
/*...sget_pal:0:*/
static void get_pal(const char *pal, GBMRGB *gbmrgb, int bpp, int *ncols)
	{
	char fn_pal[500+1], *opt_pal;
	int fd, ft_pal, i;
	GBM gbm;
	GBM_ERR	rc;

	if ( pal == NULL )
		fatal("palette file must be specified");

	strcpy(fn_pal, pal);
	if ( (opt_pal = strchr(fn_pal, ',')) != NULL )
		*opt_pal++ = '\0';
	else
		opt_pal = "";

	if ( gbm_guess_filetype(fn_pal, &ft_pal) != GBM_ERR_OK )
		fatal("can't guess bitmap file format for %s", fn_pal);

	if ( (fd = gbm_io_open(fn_pal, O_RDONLY|O_BINARY)) == -1 )
		fatal("can't open %s", fn_pal);

	if ( (rc = gbm_read_header(fn_pal, fd, ft_pal, &gbm, opt_pal)) != GBM_ERR_OK )
		{
		gbm_io_close(fd);
		fatal("can't read header of %s: %s", fn_pal, gbm_err(rc));
		}

	if ( gbm.bpp > 8 )
		{
		gbm_io_close(fd);
		fatal("bitmap %s should contain a palette", fn_pal);
		}

	if ( gbm.bpp > bpp )
		{
		gbm_io_close(fd);
		fatal("bitmap %s has too many colours in palette for chosen mapping", fn_pal);
		}

	/* Pre-initialise palette with medium grey */
	for ( i = 0; i < (1<<bpp); i++ )
		gbmrgb[i].r =
		gbmrgb[i].g =
		gbmrgb[i].b = 0x80;

	if ( (rc = gbm_read_palette(fd, ft_pal, &gbm, gbmrgb)) != GBM_ERR_OK )
		{
		gbm_io_close(fd);
		fatal("can't read palette of %s: %s", fn_pal, gbm_err(rc));
		}

	gbm_io_close(fd);

	*ncols = ( 1 << gbm.bpp );
	}
/*...e*/
/*...sexpand_to_24bit:0:*/
static void expand_to_24bit(GBM *gbm, GBMRGB *gbmrgb, byte **data)
	{
	int stride = ((gbm->w * gbm->bpp + 31)/32) * 4;
	int new_stride = ((gbm->w * 3 + 3) & ~3);
	int bytes, y;
	byte *new_data;

	if ( gbm->bpp == 24 )
		return;

	bytes = new_stride * gbm->h;
	if ( (new_data = malloc((size_t) bytes)) == NULL )
		fatal("out of memory allocating %d bytes", bytes);

	for ( y = 0; y < gbm->h; y++ )
		{
		byte	*src = *data + y * stride;
		byte	*dest = new_data + y * new_stride;
		int	x;

		switch ( gbm->bpp )
			{
/*...s1:24:*/
case 1:
	{
	byte	c;

	for ( x = 0; x < gbm->w; x++ )
		{
		if ( (x & 7) == 0 )
			c = *src++;
		else
			c <<= 1;

		*dest++ = gbmrgb[c >> 7].b;
		*dest++ = gbmrgb[c >> 7].g;
		*dest++ = gbmrgb[c >> 7].r;
		}
	}
	break;
/*...e*/
/*...s4:24:*/
case 4:
	for ( x = 0; x + 1 < gbm->w; x += 2 )
		{
		byte	c = *src++;

		*dest++ = gbmrgb[c >> 4].b;
		*dest++ = gbmrgb[c >> 4].g;
		*dest++ = gbmrgb[c >> 4].r;
		*dest++ = gbmrgb[c & 15].b;
		*dest++ = gbmrgb[c & 15].g;
		*dest++ = gbmrgb[c & 15].r;
		}

	if ( x < gbm->w )
		{
		byte	c = *src;

		*dest++ = gbmrgb[c >> 4].b;
		*dest++ = gbmrgb[c >> 4].g;
		*dest++ = gbmrgb[c >> 4].r;
		}
	break;
/*...e*/
/*...s8:24:*/
case 8:
	for ( x = 0; x < gbm->w; x++ )
		{
		byte	c = *src++;

		*dest++ = gbmrgb[c].b;
		*dest++ = gbmrgb[c].g;
		*dest++ = gbmrgb[c].r;
		}
	break;
/*...e*/
			}
		}
	free(*data);
	*data = new_data;
	gbm->bpp = 24;
	}
/*...e*/
/*...sto_grey_pal:0:*/
static void to_grey_pal(GBMRGB *gbmrgb)
	{
	int i;

	for ( i = 0; i < 0x100; i++ )
		gbmrgb[i].r =
		gbmrgb[i].g =
		gbmrgb[i].b = (byte) i;
	}
/*...e*/
/*...sto_grey:0:*/
static void to_grey(GBM *gbm, const byte *src_data, byte *dest_data)
	{
	int src_stride  = ((gbm->w * 3 + 3) & ~3);
	int dest_stride = ((gbm->w     + 3) & ~3);
	int y;

	for ( y = 0; y < gbm->h; y++ )
		{
		const byte *src  = src_data;
		      byte *dest = dest_data;
		int x;

		for ( x = 0; x < gbm->w; x++ )
			{
			byte b = *src++;
			byte g = *src++;
			byte r = *src++;

			*dest++ = (byte) (((word) r * 77U + (word) g * 150U + (word) b * 29U) >> 8);
			}

		src_data  += src_stride;
		dest_data += dest_stride;
		}
	gbm->bpp = 8;
	}
/*...e*/
/*...stripel_pal:0:*/
static void tripel_pal(GBMRGB *gbmrgb)
	{
	int	i;

	memset(gbmrgb, 0, 0x100 * sizeof(GBMRGB));

	for ( i = 0; i < 0x40; i++ )
		{
		gbmrgb[i       ].r = (byte) (i << 2);
		gbmrgb[i + 0x40].g = (byte) (i << 2);
		gbmrgb[i + 0x80].b = (byte) (i << 2);
		}
	}
/*...e*/
/*...stripel:0:*/
static void tripel(GBM *gbm, const byte *src_data, byte *dest_data)
	{
	int src_stride  = ((gbm->w * 3 + 3) & ~3);
	int dest_stride = ((gbm->w     + 3) & ~3);
	int y;

	for ( y = 0; y < gbm->h; y++ )
		{
		const byte *src  = src_data;
		      byte *dest = dest_data;
		int x;

		for ( x = 0; x < gbm->w; x++ )
			{
			byte b = *src++;
			byte g = *src++;
			byte r = *src++;

			switch ( (x+y)%3 )
				{
				case 0:	*dest++ = (byte)         (r >> 2) ;	break;
				case 1:	*dest++ = (byte) (0x40 + (g >> 2));	break;
				case 2:	*dest++ = (byte) (0x80 + (b >> 2));	break;
				}
			}

		src_data  += src_stride;
		dest_data += dest_stride;
		}
	gbm->bpp = 8;
	}
/*...e*/

int main(int argc, char *argv[])
	{
	BOOLEAN	errdiff = FALSE, halftone = FALSE, ok = TRUE;
	int	htmode = 0;
	char	*map = "7x8x4";
	char	*pal = NULL;
	char	fn_src[500+1], fn_dst[500+1], *opt_src, *opt_dst;
	int	fd, ft_src, ft_dst, i, stride, bytes, flag, m, dest_bpp;
	byte	rm, gm, bm;
	int	ncols;
	GBM_ERR	rc;
	GBMFT	gbmft;
	GBM	gbm;
	GBMRGB	gbmrgb[0x100];
	byte	*data;

/*...sprocess command line options:8:*/
for ( i = 1; i < argc; i++ )
	{
	if ( argv[i][0] != '-' )
		break;
	else if ( argv[i][1] == '-' )
		{ ++i; break; }
	switch ( argv[i][1] )
		{
		case 'e':	errdiff = TRUE;
				break;
		case 'h':	halftone = TRUE;
				if ( argv[i][2] != '\0' && isdigit(argv[i][2]) )
					htmode = argv[i][2] - '0';
				break;
		case 'm':	if ( ++i == argc )
					fatal("expected map argument");
				map = argv[i];
				break;
		case 'p':	if ( ++i == argc )
					fatal("expected palette argument");
				pal = argv[i];
				break;
		default:	usage();
				break;
		}
	}

if ( errdiff && halftone )
	fatal("error-diffusion and halftoning can't both be done at once");
/*...e*/
/*...sdeduce mapping and bits per pixel etc\46\:8:*/
if ( get_masks(map, &rm, &gm, &bm) && map[5] == '\0' )
	{
	m = CVT_RGB;
	dest_bpp = 24;
	}
else if ( same(map, "freq", 4) )
	{
	m = CVT_FREQ;
	dest_bpp = 8;
	if ( !get_masks(map+4, &rm, &gm, &bm) )
		fatal("freqR:G:B:N has bad/missing R:G:B");
	if ( map[9] != ':' )
		fatal("freqR:G:B:N has bad/missing :N");
	sscanf(map+10, "%i", &ncols);
	if ( ncols < 1 || ncols > 256 )
		fatal("freqR:G:B:N N number between 1 and 256 required");
	}
else if ( same(map, "mcut", 4) )
	{
	m = CVT_MCUT;
	dest_bpp = 8;
	sscanf(map+4, "%i", &ncols);
	if ( ncols < 1 || ncols > 256 )
		fatal("mcutN N number between 1 and 256 required");
	}
else
	{
	int j;

	for ( j = 0; j < N_MAPINFOS; j++ )
		if ( same(map, mapinfos[j].name, strlen(map) + 1) )
			break;
	if ( j == N_MAPINFOS )
		fatal("unrecognised mapping %s", map);
	m        = mapinfos[j].m;
	dest_bpp = mapinfos[j].dest_bpp;
	}
/*...e*/

	if ( i == argc )
		usage();
	strcpy(fn_src, argv[i++]);
	strcpy(fn_dst, ( i == argc ) ? fn_src : argv[i++]);
	if ( i < argc )
		usage();

	if ( (opt_src = strchr(fn_src, ',')) != NULL )
		*opt_src++ = '\0';
	else
		opt_src = "";

	if ( (opt_dst = strchr(fn_dst, ',')) != NULL )
		*opt_dst++ = '\0';
	else
		opt_dst = "";

	gbm_init();

	if ( gbm_guess_filetype(fn_src, &ft_src) != GBM_ERR_OK )
		fatal("can't guess bitmap file format for %s", fn_src);

	if ( gbm_guess_filetype(fn_dst, &ft_dst) != GBM_ERR_OK )
		fatal("can't guess bitmap file format for %s", fn_dst);

	if ( (fd = gbm_io_open(fn_src, O_RDONLY|O_BINARY)) == -1 )
		fatal("can't open %s", fn_src);

	if ( (rc = gbm_read_header(fn_src, fd, ft_src, &gbm, opt_src)) != GBM_ERR_OK )
		{
		gbm_io_close(fd);
		fatal("can't read header of %s: %s", fn_src, gbm_err(rc));
		}

	gbm_query_filetype(ft_dst, &gbmft);
	switch ( dest_bpp )
		{
		case 24:	flag = GBM_FT_W24;	break;
		case 8:		flag = GBM_FT_W8;	break;
		case 4:		flag = GBM_FT_W4;	break;
		case 1:		flag = GBM_FT_W1;	break;
		}

	if ( (gbmft.flags & flag) == 0 )
		{
		gbm_io_close(fd);
		fatal("output bitmap format %s does not support writing %d bpp data",
			gbmft.short_name, dest_bpp);
		}

	if ( (rc = gbm_read_palette(fd, ft_src, &gbm, gbmrgb)) != GBM_ERR_OK )
		{
		gbm_io_close(fd);
		fatal("can't read palette of %s: %s", fn_src, gbm_err(rc));
		}

	stride = ( ((gbm.w * gbm.bpp + 31)/32) * 4 );
	bytes = stride * gbm.h;
	if ( (data = malloc((size_t) bytes)) == NULL )
		{
		gbm_io_close(fd);
		fatal("out of memory allocating %d bytes", bytes);
		}

	if ( (rc = gbm_read_data(fd, ft_src, &gbm, data)) != GBM_ERR_OK )
		{
		free(data);
		gbm_io_close(fd);
		fatal("can't read bitmap data of %s: %s", fn_src, gbm_err(rc));
		}

	gbm_io_close(fd);

	/* Now expand bits per pixel if necessary */

	expand_to_24bit(&gbm, gbmrgb, &data);

	if ( errdiff )
		m |= CVT_ERRDIFF;

	if ( halftone )
		m |= CVT_HALFTONE;

	switch ( m )
		{
		case CVT_BW:
			gbm_trunc_pal_BW(gbmrgb);
			gbm_trunc_BW(&gbm, data, data);
			break;
		case CVT_4G:
			gbm_trunc_pal_4G(gbmrgb);
			gbm_trunc_4G(&gbm, data, data);
			break;
		case CVT_8:
			gbm_trunc_pal_8(gbmrgb);
			gbm_trunc_8(&gbm, data, data);
			break;
		case CVT_VGA:
			gbm_trunc_pal_VGA(gbmrgb);
			gbm_trunc_VGA(&gbm, data, data);
			break;
		case CVT_784:
			gbm_trunc_pal_7R8G4B(gbmrgb);
			gbm_trunc_7R8G4B(&gbm, data, data);
			break;
		case CVT_666:
			gbm_trunc_pal_6R6G6B(gbmrgb);
			gbm_trunc_6R6G6B(&gbm, data, data);
			break;
		case CVT_8G:
			to_grey_pal(gbmrgb);
			to_grey(&gbm, data, data);
			break;
		case CVT_TRIPEL:
			tripel_pal(gbmrgb);
			tripel(&gbm, data, data);
			break;
		case CVT_PAL1BPP:
			get_pal(pal, gbmrgb, 1, &ncols);
			gbm_trunc_1bpp(&gbm, data, data, gbmrgb, ncols);
			break;
		case CVT_PAL4BPP:
			get_pal(pal, gbmrgb, 4, &ncols);
			gbm_trunc_4bpp(&gbm, data, data, gbmrgb, ncols);
			break;
		case CVT_PAL8BPP:
			get_pal(pal, gbmrgb, 8, &ncols);
			gbm_trunc_8bpp(&gbm, data, data, gbmrgb, ncols);
			break;
		case CVT_FREQ:
			memset(gbmrgb, 0, sizeof(gbmrgb));
			ok = gbm_hist(&gbm, data, gbmrgb, data, ncols, rm, gm, bm);
			break;
		case CVT_RGB:
			gbm_trunc_24(&gbm, data, data, rm, gm, bm);
			break;
		case CVT_MCUT:
			ok = gbm_mcut(&gbm, data, gbmrgb, data, ncols);
			break;
		case CVT_BW | CVT_ERRDIFF:
			gbm_errdiff_pal_BW(gbmrgb);
			ok = gbm_errdiff_BW(&gbm, data, data);
			break;
		case CVT_4G | CVT_ERRDIFF:
			gbm_errdiff_pal_4G(gbmrgb);
			ok = gbm_errdiff_4G(&gbm, data, data);
			break;
		case CVT_8 | CVT_ERRDIFF:
			gbm_errdiff_pal_8(gbmrgb);
			ok = gbm_errdiff_8(&gbm, data, data);
			break;
		case CVT_VGA | CVT_ERRDIFF:
			gbm_errdiff_pal_VGA(gbmrgb);
			ok = gbm_errdiff_VGA(&gbm, data, data);
			break;
		case CVT_784 | CVT_ERRDIFF:
			gbm_errdiff_pal_7R8G4B(gbmrgb);
			ok = gbm_errdiff_7R8G4B(&gbm, data, data);
			break;
		case CVT_666 | CVT_ERRDIFF:
			gbm_errdiff_pal_6R6G6B(gbmrgb);
			ok = gbm_errdiff_6R6G6B(&gbm, data, data);
			break;
		case CVT_RGB | CVT_ERRDIFF:
			ok = gbm_errdiff_24(&gbm, data, data, rm, gm, bm);
			break;
		case CVT_784 | CVT_HALFTONE:
			gbm_ht_pal_7R8G4B(gbmrgb);
			gbm_ht_7R8G4B_2x2(&gbm, data, data);
			break;
		case CVT_666 | CVT_HALFTONE:
			gbm_ht_pal_6R6G6B(gbmrgb);
			gbm_ht_6R6G6B_2x2(&gbm, data, data);
			break;
		case CVT_8 | CVT_HALFTONE:
			gbm_ht_pal_8(gbmrgb);
			switch ( htmode )
				{
				default:
				case 0: gbm_ht_8_3x3(&gbm, data, data); break;
				case 1: gbm_ht_8_2x2(&gbm, data, data); break;
				}
			break;
		case CVT_VGA | CVT_HALFTONE:
			gbm_ht_pal_VGA(gbmrgb);
			switch ( htmode )
				{
				default:
				case 0: gbm_ht_VGA_3x3(&gbm, data, data); break;
				case 1: gbm_ht_VGA_2x2(&gbm, data, data); break;
				}
			break;
		case CVT_RGB | CVT_HALFTONE:
			gbm_ht_24_2x2(&gbm, data, data, rm, gm, bm);
			break;
		default:
			fatal("bad mapping/error-diffusion/halftone combination");
		}

	if ( !ok )
		fatal("unable to perform mapping");

	gbm.bpp = dest_bpp;

	if ( (fd = gbm_io_create(fn_dst, O_WRONLY|O_BINARY)) == -1 )
		fatal("can't create %s", fn_dst);

	if ( (rc = gbm_write(fn_dst, fd, ft_dst, &gbm, gbmrgb, data, opt_dst)) != GBM_ERR_OK )
		{
		gbm_io_close(fd);
		remove(fn_dst);
		fatal("can't write %s: %s", fn_dst, gbm_err(rc));
		}

	gbm_io_close(fd);

	free(data);

	gbm_deinit();

	return 0;
	}
/*...e*/
