#ifndef _COMMON_H_INCLUDED
#define _COMMON_H_INCLUDED

#define FIELDOFFSETUL(type, field)    ((ULONG)&(((type *)0)->field))

/* ID tag defines */
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

#define IDINFO_LASTINFO          10

#define MRFALSE (MRESULT) FALSE
#define MRTRUE (MRESULT) TRUE

/* Fonts to use in dialogs */
#define DEFAULT_DIALOG_FONT   "9.WarpSans"
#define DEFAULT_DIALOG_FONT_WARP3   "8.Helv"

#endif
