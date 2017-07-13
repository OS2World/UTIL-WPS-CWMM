#ifndef _CWAUDIOINC_H_INCLUDED
#define _CWAUDIOINC_H_INCLUDED

#include "common.h"

#define SETUP_AUDIOTRACKNAME    "CWAUDIOTRACKNAME"
#define SETUP_AUDIOARTIST       "CWAUDIOARTIST"
#define SETUP_AUDIOALBUM        "CWAUDIOALBUM"
#define SETUP_AUDIOCOMMENT      "CWAUDIOCOMMENT"
#define SETUP_AUDIOYEAR         "CWAUDIOYEAR"
#define SETUP_AUDIOGENRE        "CWAUDIOGENRE"
#define SETUP_AUDIOPLAYTIME     "CWAUDIOPLAYTIME"
#define SETUP_REFRESH           "CWAUDIOREFRESH"
#define SETUP_COPYTAGTO         "CWAUDIOCOPYTAGTO"

/* Restore keys for audio info from instance data */
#define  KEY_TIME         4001
#define  KEY_CHANNELS     4002
#define  KEY_SAMPLERATE   4003
#define  KEY_BPS          4004
#define  KEY_FILESIZE     4005
#define  KEY_TRACKNAME    4006
#define  KEY_ARTIST       4007
#define  KEY_ALBUM        4008
#define  KEY_COMMENT      4009
#define  KEY_YEAR         4010
#define  KEY_GENRE        4011
#define  KEY_TIME_MS      4012

#define TRACKNAME_SETTINGS_PANEL 12

/* The max length of any new column title */
#define SIZE_TITLE       30
#define NUM_AUDIO_FIELDS 10 /* Num of additional details columns */

/* Set some default titles */
static PSZ pszAudioColTitles[NUM_AUDIO_FIELDS]= {"Playtime", "Samplerate", "Channels", "Bits per sample",
"Track Name","Artist","Album","Year","Comment","Genre"};

static char chrPlayTime[SIZE_TITLE]="";
static char chrSampleRate[SIZE_TITLE]="";
static char chrChannels[SIZE_TITLE]="";
static char chrBPS[SIZE_TITLE]="";
static char chrName[SIZE_TITLE]="";
static char chrArtist[SIZE_TITLE]="";
static char chrAlbum[SIZE_TITLE]="";
static char chrYear[SIZE_TITLE]="";
static char chrComment[SIZE_TITLE]="";
static char chrGenre[SIZE_TITLE]="";

typedef struct _AUDIODETAILS
{
  PSZ pszPlayTime;
  PSZ pszSampleRate;
  PSZ pszChannels;
  PSZ pszBPS;
  PSZ pszName;
  PSZ pszArtist;
  PSZ pszAlbum;
  PSZ pszYear;
  PSZ pszComment;
  PSZ pszGenre;
}AUDIODETAILS;
typedef AUDIODETAILS *PAUDIODETAILS;

static CLASSFIELDINFO cfiFieldInfo[NUM_AUDIO_FIELDS];

#define CWAUDIO_VIEW_PLAY ID_MENU_START

/* Parameters for WM_APPTERMINATENOTIFY for controlling playing */
#define ACKEY_START  1
#define ACKEY_STOP   2


/* CWImage */
#define NUM_IMAGE_DETAILS_FIELDS 4 /* Num of additional details columns */

/* Set some default titles */
static PSZ pszImageColTitles[NUM_IMAGE_DETAILS_FIELDS]= {"Width", "Height", "Depth", "Format"};

static char chrWidth[SIZE_TITLE]="";
static char chrHeight[SIZE_TITLE]="";
static char chrDepth[SIZE_TITLE]="";
static char chrFormat[SIZE_TITLE]="";

typedef struct _IMAGEDETAILS
{
  ULONG ulWidth;
  ULONG ulHeight;
  ULONG ulDepth;
  PSZ pszFormat;
}IMAGEDETAILS;
typedef IMAGEDETAILS *PIMAGEDETAILS;

static CLASSFIELDINFO cfiImageFieldInfo[NUM_IMAGE_DETAILS_FIELDS];

#define MAX_SIZE_SMALLBITMAP 250


typedef struct _CREATEBMPINFO
{
  WPObject * wpObject;
  PMINIRECORDCORE pmrcToRefresh;
  HWND hwndCnr;
  ULONG ulWidth;
  ULONG ulHeight;
}CREATEBMPINFO;
typedef CREATEBMPINFO *PCREATEBMPINFO;

/* Restore keys for lighttable folder */
#define KEY_ICONSIZE     4001

/* Ini key for enabling new image class features on CD */
#define IMAGECLS_FEATURES_ON_CD_KEY    "useNewImgClsFeaturesOnCD"
/* Ini key to disable extended icon titles for audio files */
#define MM_NO_EXTENDED_ICONTITLE    "mmClsNoExtendedIconTitles"

#endif










