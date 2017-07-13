static void 

GBM_ERR gbm_simple_scale(
	const byte *s, int sw, int sh,
	      byte *d, int dw, int dh,
	int bpp
	)
	{
	int sst = ( (sw * bpp + 31) / 32 ) * 4;
	int dst = ( (dw * bpp + 31) / 32 ) * 4;
	int *xs, *ys, i;
	void (*scaler)(const byte *s, byte *d, int dw, int xs[]);

	/* Allocate memory for step arrays */

	if ( (xs = malloc((size_t) ((dw+1+dh+1)*sizeof(int)))) == NULL )
		return GBM_ERR_MEM;
	ys = xs + (dw+1);

	/* Make mapping to 0..dx from 0..sx (and same for y) */

	for ( i = 0; i <= dw; i++ )
		xs[i] = (i * sw) / dw;

	for ( i = 0; i <= dh; i++ )
		ys[i] = (i * sh) / dh;

	/* Compute step coefficients */

	for ( i = 0; i < dw; i++ )
		xs[i] = xs[i+1] - xs[i];
	
	for ( i = 0; i < dh; i++ )
		ys[i] = ys[i+1] - ys[i];

	/* Pick a scaling routine. Special optimisation to prevent
	   excessive work scaling horizontally if widths are the same.
	   Effectively reduces this code to a memcpy. */

	if ( dw == sw )
		switch ( bpp )
			{
			case 1 : scaler = fast_simple_scale_1 ; break;
			case 4 : scaler = fast_simple_scale_4 ; break;
			case 8 : scaler = fast_simple_scale_8 ; break;
			case 24: scaler = fast_simple_scale_24; break;
			}
	else
		switch ( bpp )
			{
			case 1 : scaler = simple_scale_1 ; break;
			case 4 : scaler = simple_scale_4 ; break;
			case 8 : scaler = simple_scale_8 ; break;
			case 24: scaler = simple_scale_24; break;
			}

	/* Now do guts of scaling */

	while ( dh-- > 0 )
		{
		(*scaler)(s, d, dw, xs);
		d += dst;
		s += (sst * *ys++);
		}

	free(xs);
	return GBM_ERR_OK;
	}

static void expand(const byte *s, int sw, byte *d, int dw)
	{
	int n = 0, col = 0;
	for ( ;; )
		{
		n += dw;
		while ( n >= sw )
			{
			n -= sw;
			*d++ = 

		col += *s++ * dw;
		while ( 


	for ( --

	int i = 0, c = 0;
	while ( i < sw * dw )
		{
		i += sw


	int i = 0, col = 0;
	while ( sw > 0 )
		{
		i += dx;
		while ( i >= sx )
			{
			i -= sx;
			col += *s++ * sx;
			*d++ = (col / dx);
			}
		}

	int i = 0, col = 0;
	while ( dw > 0 )
		{


		if ( i < 

GBM_ERR gbm_scale_8(
	const byte *s, int sw, int sh,
	      byte *d, int dw, int dh
	)
	{
	int sst = ( (sw * 8 + 31) / 32 ) * 4;
	int dst = ( (dw * 8 + 31) / 32 ) * 4;






	int *xs, *ys, i;
	void (*scaler)(const byte *s, byte *d, int dw, const int xs[]);

	/* Allocate memory for step arrays */

	if ( (xs = malloc((size_t) ((dw+dh)*sizeof(int)))) == NULL )
		return GBM_ERR_MEM;
	ys = xs + dw;

	/* Make mapping to 0..dx-1 from 0..sx-1 (and same for y) */

	for ( i = 0; i < dw; i++ )
		xs[i] = (i * (sw-1) * 0x100) / (dw-1);

	for ( i = 0; i < dh; i++ )
		ys[i] = (i * (sh-1) * 0x100) / (dh-1);

	/* Compute step coefficients */

	for ( i = 0; i < dw; i++ )
		xs[i] = xs[i+1] - xs[i];
	
	for ( i = 0; i < dh; i++ )
		ys[i] = ys[i+1] - ys[i];



static void simple_scale_8(
	const byte *s,
	byte *d, int dw,
	int xs[]
	)
	{
	while ( dw-- > 0 )
		{
		*d++ = *s;
		s += *xs++;
		}
	}
