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

#ifndef _CWAUDIOINC_H_INCLUDED
#define _CWAUDIOINC_H_INCLUDED

#include "common.h"

/* Target folder for MM class templates */
#define MMTEMPLATE_FOLDER_ID      "<MMPM2_MMTEMPLATEFOLDER>"

/* Default class titles if name can't be loaded from res dll */
#define CLSTITLE_DIGITALAUDIO    "Digital Audio"
#define CLSTITLE_MP3             "MP3Audio.MP3"
#define CLSTITLE_WAV             "WAVAudio.WAV"
#define CLSTITLE_OGG             "OGGAudio.OGG"
#define CLSTITLE_AIF             "AIFAudio.AIF"
#define CLSTITLE_AU              "AUAudio.AU"
#define CLSTITLE_IFF             "IFFAudio.IFF"
#define CLSTITLE_MIDI            "MIDI.MID"
#define CLSTITLE_SNDF            "SNDAudio.SND"
#define CLSTITLE_VOC             "VOCAudio.VOC"
#define CLSTITLE__AU             "_AUAudio._AU"
#define CLSTITLE_AUDIOSTREAM     "Audio-Stream"
#define CLSTITLE_CDTRACK         "CD-Track"
#define CLSTITLE_M3U             "M3U-Playlist"
#define CLSTITLE_FLAC            "FLACAudio.FLA"

/* TYPES returned by the classes */
#define TYPE_DIGITALAUDIO    "Digital Audio"
#define TYPE_MP3             "MP3"
#define TYPE_OGG             "OGGS"
#define TYPE_WAV             "WAV"
#define TYPE_AIF             "AIF"
#define TYPE_AU              "AU"
#define TYPE_IFF             "IFF"
#define TYPE_MIDI            "MIDI"
#define TYPE_SNDF            "SND"
#define TYPE_VOC             "VOC"
#define TYPE__AU             "_AU"
#define TYPE_AUDIOSTREAM     "Audio-Stream"
#define TYPE_CDTRACK         "CD-Track"
#define TYPE_M3U             "M3U"
#define TYPE_FLAC            "FLAC"

/* Setup strings for MMAudio (was CWAudio). These strings will
   be removed in the future. Use the MM* instead */
#define SETUP_AUDIOTRACKNAME    "CWAUDIOTRACKNAME"
#define SETUP_AUDIOARTIST       "CWAUDIOARTIST"
#define SETUP_AUDIOALBUM        "CWAUDIOALBUM"
#define SETUP_AUDIOCOMMENT      "CWAUDIOCOMMENT"
#define SETUP_AUDIOYEAR         "CWAUDIOYEAR"
#define SETUP_AUDIOGENRE        "CWAUDIOGENRE"
#define SETUP_AUDIOPLAYTIME     "CWAUDIOPLAYTIME"
#define SETUP_REFRESH           "CWAUDIOREFRESH"
#define SETUP_COPYTAGTO         "CWAUDIOCOPYTAGTO"

#define SETUP_MMAUDIOTRACKNAME    "MMAUDIOTRACKNAME"
#define SETUP_MMAUDIOARTIST       "MMAUDIOARTIST"
#define SETUP_MMAUDIOALBUM        "MMAUDIOALBUM"
#define SETUP_MMAUDIOCOMMENT      "MMAUDIOCOMMENT"
#define SETUP_MMAUDIOYEAR         "MMAUDIOYEAR"
#define SETUP_MMAUDIOGENRE        "MMAUDIOGENRE"
#define SETUP_MMAUDIOPLAYTIME     "MMAUDIOPLAYTIME"
#define SETUP_MMREFRESH           "MMREFRESH"
#define SETUP_MMCOPYTAGTO         "MMAUDIOCOPYTAGTO"
/* For setting volume during playing */
#define SETUP_MMAUDIOVOLUME         "MMAUDIOVOLUME"
/* Lighttable */
#define SETUP_MMTHUMBSIZE         "MMTHUMBSIZE"

/* Restore keys for audio info from instance data */
#define  KEY_TIME         4001
#define  KEY_CHANNELS     4002
#define  KEY_SAMPLERATE   4003
#define  KEY_BPS          4004
#define  KEY_FILESIZE     4005 /* also used for image files */
/* #define  KEY_TRACKNAME    4006
   #define  KEY_ARTIST       4007
   #define  KEY_ALBUM        4008
   #define  KEY_COMMENT      4009
   #define  KEY_YEAR         4010
   #define  KEY_GENRE        4011 */
#define  KEY_TIME_MS      4012
#define  KEY_NUM_AUDIO_BYTES  4013

#define TRACKNAME_SETTINGS_PANEL 12

/* Restore keys for image info from instance data */
#define  KEY_WIDTH         4001
#define  KEY_HEIGHT        4002
#define  KEY_DEPTH         4003


/* The max length of any new column title */
#define SIZE_TITLE       30
#define NUM_AUDIO_FIELDS 11 /* Num of additional details columns */

typedef struct _AUDIODETAILS
{
  PSZ pszPlayTime;
  PSZ pszBitRate;     /* New with 0.2.7 */
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

#define CWAUDIO_VIEW_PLAY ID_MENU_START

/* Parameters for WM_APPTERMINATENOTIFY for controlling playing */
#define ACKEY_START  1
#define ACKEY_STOP   2
#define ACKEY_PAUSE  3
#define ACKEY_RESUME 4
#define ACKEY_NEXT   5
#define ACKEY_VOLUME 6

/* returned by cwmmQueryTrackStatus()*/
#define TRACK_STATUS_STOPPED 0
#define TRACK_STATUS_PLAYING 1
#define TRACK_STATUS_PAUSED  2



/* MMImage */
#define NUM_IMAGE_DETAILS_FIELDS 4 /* Num of additional details columns */

char chrWidth[SIZE_TITLE];
char chrHeight[SIZE_TITLE];
char chrDepth[SIZE_TITLE];
char chrFormat[SIZE_TITLE];

typedef struct _IMAGEDETAILS
{
  ULONG ulWidth;
  ULONG ulHeight;
  ULONG ulDepth;
  PSZ pszFormat;
}IMAGEDETAILS;
typedef IMAGEDETAILS *PIMAGEDETAILS;


/* The maximum size of returned small bitmaps for lighttable folder. Used if someone tries to get
   a wider BMP. */
#define MAX_SIZE_SMALLBITMAP 250

/* Thread parameters used when requesting a new small bitmap from the create bitmap thread. */
typedef struct _CREATEBMPINFO
{
  WPObject * wpObject;
  PMINIRECORDCORE pmrcToRefresh; /* Object record in container. Will be refreshed to show image instead of icon. */
  HWND hwndCnr;                  /* Container containing the object. Will be refreshed to show image instead of icon. */
  ULONG ulWidth;
  ULONG ulHeight;
}CREATEBMPINFO;
typedef CREATEBMPINFO *PCREATEBMPINFO;

/* Tell create bitmap thread to create a new small bitmap */
#define ACKEY_CREATE_SMALLBITMAP  0
/* Tell create bitmap thread to read in the image info so cached image size will be
   updated. */
#define ACKEY_QUERY_BITMAPINFO    1

/*
  Parameter block given to the play thread when audio objects are started using method
  cwmmPlayTrack().
  */
typedef struct _OPENPARAMS
{
  WPObject* wpObject; /* Object being started. */
  HWND hwndCnr;      /* Container from which the object is started. */
  ULONG ulReserved1;
  ULONG ulReserved2;
  ULONG ulReserved3;
  //PID pid;
  //  SWCNTRL swCtrl;
  USEITEM useItem;   /* Tell the WPS the object is in use when playing so inuse emphasis
                        is drawn. An OPEN_VIEW item is added to the inuse list. The information
                        is in the following VIEWITEM. These two structures are used together! */
  VIEWITEM viewItem; /* Used to register the open view with the WPS. */
}OPENPARAMS;

typedef struct _MMDATAFILEVIEW
{
  USEITEM useItem;
  VIEWITEM viewItem;
  WPObject* thisPtr;
  PID pid;
  SWCNTRL swCtrl;
  WPObject* wpObject;
}MMDATAFILEVIEW;


#define DEFAULT_AUDIOSTREAM_SERVER "Icecast-Server:port"

/* Ini key for enabling new image class features on CD */
#define IMAGECLS_FEATURES_ON_CD_KEY    "useNewImgClsFeaturesOnCD"
/* Ini key to disable extended icon titles for audio files */
#define MM_NO_EXTENDED_ICONTITLE    "mmClsNoExtendedIconTitles"

/* Help panels */
#define M3UFILE_HELP_PANEL  40

/* Maximum # of startable audio files at the same time. This
   prevents that a user starts a whole folder full of audio objects.
   see CWAudio */
#define MAX_AUDIOFILE_STARTS 1

/* Default volume for audio file if no entry in INI file. Used only for double click
   starting. */
#define DEFVOLUME_AUDIO_FILE  70
#define CLASS_VOLUME_APP   "classvolume"
#define CLASS_VOLUME_KEY   "volume"

#endif










