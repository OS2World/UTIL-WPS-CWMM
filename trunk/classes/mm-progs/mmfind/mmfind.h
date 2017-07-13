/* Bitmap IDs for the play controls */
#define IDBMP_PLAY 1
#define IDBMP_STOP 2

/* Fallbacks if strings can't be loaded */
#define TEXT_TYPEAUDIO "Audio"
#define TEXT_TYPEIMAGE "Image"
#define TEXT_TYPEVIDEO "Video"
#define TEXT_INFOTEXT  "Play time:\t%02d:%02d\n\nSamplerate:\t%d\n\nBits per sample:\t%d\n\nChannels:\t%d"
/* Max. length of the string holding all our extensions */
#define EXT_STRING_LENGTH 300

/* Media types we support for searching */
#define NUMMEDIATYPES 3
#define MT_AUDIO 0
#define MT_IMAGE 1
#define MT_VIDEO 2
#define MT_UNKNOWN 100

#define MMAUDIO_PLAYER_CREATED 0x0001
#define PLAY_FILE   0x0001
#define STOP_TRACK   0x0002
#define PAUSE_TRACK   0x0003

#ifndef OPEN_DEFAULT 
/*  Open views for the wpOpen() method */
      #define OPEN_UNKNOWN      -1
      #define OPEN_DEFAULT       0
      #define OPEN_CONTENTS      1
      #define OPEN_SETTINGS      2
      #define OPEN_HELP          3
      #define OPEN_RUNNING       4
      #define OPEN_PROMPTDLG     5
      #define OPEN_PALETTE       121                                   /*SPLIT*/
      #define OPEN_USER          0x6500
#endif

/* Timer IDs */
#define TIMERID_SELECTION 1
#define IDTIMER_PLAY      2

/* Defines used for audio playing */
#define MMAUDIO_PLAYER_CREATED 0x0001
#define PLAY_FILE   0x0001
#define STOP_TRACK   0x0002
#define PAUSE_TRACK   0x0003

/* Function prototypes */
#ifdef DEBUG
void HlpWriteToTrapLog(const char* chrFormat, ...);
#endif

BOOL HlpBuildMMProgIniFileName(char* chrProgname, char * chrBuffer, ULONG ulBufferSize);
void freeResHandle();
HMODULE queryResModuleHandle(char *chrExePath);
void pmUsage();
VOID DrawBitmap ( HWND hwnd );
void showImageControls(HWND hwnd, BOOL bShow);

MRESULT EXPENTRY bmpPreviewProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
