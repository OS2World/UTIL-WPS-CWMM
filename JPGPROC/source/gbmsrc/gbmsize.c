/*

gbmsize.c - Change size of General Bitmap

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
#include "gbmscale.h"

/*...vgbm\46\h:0:*/
/*...vgbmscale\46\h:0:*/
/*...e*/

static char progname[] = "gbmsize";

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

	fprintf(stderr, "usage: %s [-w w] [-h h] [-a] [--] fn1.ext{,opt} [fn2.ext{,opt}]\n", progname);
	fprintf(stderr, "flags: -w w           new width of bitmap (default width of bitmap)\n");
	fprintf(stderr, "       -h h           new height of bitmap (default height of bitmap)\n");
	fprintf(stderr, "       -a             preserve aspect ratio when only one of -w or -h given\n");
	fprintf(stderr, "       fn1.ext{,opt}  input filename (with any format specific options)\n");
	fprintf(stderr, "       fn2.ext{,opt}  optional output filename (or will use fn1 if not present)\n");
	fprintf(stderr, "                      ext's are used to deduce desired bitmap file formats\n");

	gbm_init();
	gbm_query_n_filetypes(&n_ft);
	for ( ft = 0; ft < n_ft; ft++ )
		{
		GBMFT gbmft;

		gbm_query_filetype(ft, &gbmft);
		fprintf(stderr, "                      %s when ext in [%s]\n",
			gbmft.short_name, gbmft.extensions);
		}
	gbm_deinit();

	fprintf(stderr, "       opt's          bitmap format specific options\n");

	exit(1);
	}
/*...e*/
/*...sget_opt_value:0:*/
static int get_opt_value(const char *s, const char *name)
	{
	int v;

	if ( s == NULL )
		fatal("missing %s argument", name);
	if ( !isdigit(s[0]) )
		fatal("bad %s argument", name);
	if ( s[0] == '0' && tolower(s[1]) == 'x' )
		sscanf(s + 2, "%x", &v);
	else
		v = atoi(s);

	return v;
	}
/*...e*/
/*...sget_opt_value_pos:0:*/
static int get_opt_value_pos(const char *s, const char *name)
	{
	int n;

	if ( (n = get_opt_value(s, name)) < 0 )
		fatal("%s should not be negative", name);
	return n;
	}
/*...e*/
/*...smain:0:*/
int main(int argc, char *argv[])
	{
	int	w = -1, h = -1;
	char	fn_src[500+1], fn_dst[500+1], *opt_src, *opt_dst;
	int	fd, ft_src, ft_dst, i, stride, stride2, flag;
	GBM_ERR	rc;
	GBMFT	gbmft;
	GBM	gbm;
	GBMRGB	gbmrgb[0x100];
	BOOLEAN	aspect = FALSE;
	byte	*data, *data2;

	for ( i = 1; i < argc; i++ )
		{
		if ( argv[i][0] != '-' )
			break;
		else if ( argv[i][1] == '-' )
			{ ++i; break; }
		switch ( argv[i][1] )
			{
			case 'w':
				if ( ++i == argc ) usage();
				w = get_opt_value_pos(argv[i], "w");
				break;
			case 'h':
				if ( ++i == argc ) usage();
				h = get_opt_value_pos(argv[i], "h");
				break;
			case 'a':
				aspect = TRUE;
				break;
			default:
				usage();
				break;
			}
		}

	if ( aspect && w == -1 && h == -1 )
		{
		gbm_io_close(fd);
		fatal("-a can't be used if neither -w or -h is given");
		}

	if ( aspect && w != -1 && h != -1 )
		{
		gbm_io_close(fd);
		fatal("-a can't be used if both -w and -h are given");
		}

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

	if ( w == -1 )
		{
		if ( aspect )
			w = ( gbm.w * h ) / gbm.h;
		else
			w = gbm.w;
		}

	if ( h == -1 )
		{
		if ( aspect )
			h = ( gbm.h * w ) / gbm.w;
		else
			h = gbm.h;
		}

	if ( w == 0 || w > 10000 || h == 0 || h > 10000 )
		{
		gbm_io_close(fd);
		fatal("desired bitmap size of %dx%d, is silly", w, h);
		}

	gbm_query_filetype(ft_dst, &gbmft);
	switch ( gbm.bpp )
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
			gbmft.short_name, gbm.bpp);
		}

	if ( (rc = gbm_read_palette(fd, ft_src, &gbm, gbmrgb)) != GBM_ERR_OK )
		{
		gbm_io_close(fd);
		fatal("can't read palette of %s: %s", fn_src, gbm_err(rc));
		}

	stride = ( ((gbm.w * gbm.bpp + 31)/32) * 4 );
	if ( (data = malloc((size_t) (stride * gbm.h))) == NULL )
		{
		gbm_io_close(fd);
		fatal("out of memory allocating %d bytes for input bitmap", stride * gbm.h);
		}

	stride2 = ( ((w * gbm.bpp + 31)/32) * 4 );
	if ( (data2 = malloc((size_t) (stride2 * h))) == NULL )
		{
		free(data);
		gbm_io_close(fd);
		fatal("out of memory allocating %d bytes for output bitmap", stride2 * h);
		}

	if ( (rc = gbm_read_data(fd, ft_src, &gbm, data)) != GBM_ERR_OK )
		{
		free(data2);
		free(data);
		gbm_io_close(fd);
		fatal("can't read bitmap data of %s: %s", fn_src, gbm_err(rc));
		}

	gbm_io_close(fd);

	if ( (rc = gbm_simple_scale(data, gbm.w, gbm.h, data2, w, h, gbm.bpp)) != GBM_ERR_OK )
		{
		free(data);
		free(data2);
		fatal("can't scale: %s", gbm_err(rc));
		}

	free(data);

	if ( (fd = gbm_io_create(fn_dst, O_WRONLY|O_BINARY)) == -1 )
		{
		free(data2);
		fatal("can't create %s", fn_dst);
		}

	gbm.w = w;
	gbm.h = h;

	if ( (rc = gbm_write(fn_dst, fd, ft_dst, &gbm, gbmrgb, data2, opt_dst)) != GBM_ERR_OK )
		{
		gbm_io_close(fd);
		remove(fn_dst);
		free(data2);
		fatal("can't write %s: %s", fn_dst, gbm_err(rc));
		}

	gbm_io_close(fd);
	free(data2);

	gbm_deinit();

	return 0;
	}
/*...e*/
