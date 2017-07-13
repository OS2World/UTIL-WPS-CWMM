#ifndef _CWMP3INC_H_INCLUDED
#define _CWMP3INC_H_INCLUDED

#include "common.h"

#define MP3FILE_HELP_PANEL  10
#define MP3FILE_HELP_LIBRARY  "cwmm.hlp"
#define MP3FILE_SETTINGS_PANEL 11

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
#define SETUP_ID3TRACKNAME    "ID3TRACKNAME"
#define SETUP_ID3ARTIST       "ID3ARTIST"
#define SETUP_ID3ALBUM        "ID3ALBUM"
#define SETUP_ID3COMMENT      "ID3COMMENT"
#define SETUP_ID3YEAR         "ID3YEAR"
#define SETUP_ID3GENRE        "ID3GENRE"

#if 0
/* The max length of any new column title */
#define SIZE_TITLE       30
#define NUM_MP3_FIELDS 6

static PSZ pszMp3ColTitles[NUM_MP3_FIELDS];//, , "Year","Comment","Genre"};

static char chrBitrate[SIZE_TITLE]="Bitrate";
static char chrVBR[SIZE_TITLE]="VBR";
static char chrTrackName[SIZE_TITLE]="Track name";
static char chrArtist[SIZE_TITLE]="Artist";
static char chrAlbum[SIZE_TITLE]="Album";
static char chrYear[SIZE_TITLE]="Year";
static char chrComment[SIZE_TITLE]="Comment";
static char chrGenre[SIZE_TITLE]="Genre";

typedef struct _MP3DETAILS
{
  PSZ pszBitrate;
  PSZ pszVBR;
  PSZ pszTrackName;
  PSZ pszArtist;
  PSZ pszAlbum;
  PSZ pszYear;
  PSZ pszComment;
  PSZ pszGenre;
}MP3DETAILS;

typedef MP3DETAILS *PMP3DETAILS;

static CLASSFIELDINFO cfiMp3FieldInfo[NUM_MP3_FIELDS];
#endif

#endif

