
/* Defines for positioning the frame controls */
#define FCTL_LEFT   1
#define FCTL_RIGHT  2
#define FCTL_TOP    4
#define FCTL_BOTTOM 8
#define FCTL_POSABOVE  1
#define FCTL_POSBELOW  2

/* The following is for use with WPS-Wizard if installed  */
/* The first user menuitem ID */
#define FIRSTUSERITEM        0x7200
/* The first skript menuitem ID */
/*#define FIRSTSKRIPTITEM        0x7300*/

/* The view menu. We insert the 'Compact' menu here. */
#define MENUID_VIEW          104

/*#define SKRIPTFOLDER_MEDIAFOLDER   "<SKRIPTS_MEDIAFOLDER>"*/

/* Keys for the instance data */
#define KEY_VOLUME          4100
#define KEY_FLYFOREGROUND   4101
#define KEY_FLYBACKGROUND   4102
#define KEY_FLYFONT         4103
#define KEY_FLYDELAY        4104
#define KEY_FLYENABLED      4105
#define KEY_FLAGS           4106
#define KEY_DISPLAYTYPE     4107
#define KEY_PLAYTIMEFONT    4108

#define KEY_DRIVE           4100

/* Flags saved in the instance data */
/* Make sure we only resize the folder ones */
#define FLAG_ALREADYSIZED      0x00000001
/* Repeat check button */
#define FLAG_REPEAT            0x00000002
/* Random play */
#define FLAG_RANDOM            0x00000004

/* Which display to show */
#define DISPLAY_TIMEELAPSED    0
#define DISPLAY_TIMEREMAINING  1
#define DISPLAY_TIMEID3        2

#define SHADOW_CLASS_NAME   "CWAudioShadow"

/************* Object IDs *******************/
#define MEDIAFLDR_TBID      "<MEDIAFLDR_TOOLBAR>"
#define ID_MEDIAFLDRTEMPLATE       "<MMPM2_MEDIAFLDRTEMPLATE>"
//#define MEDIAFLDR_TEMPLATE_NAME    "Media-Player"
#define ID_MMTEMPLATEFOLDER        "<MMPM2_MMTEMPLATEFOLDER>"
#define ID_MEDIALDRTMPL_SHADOW     "<MFLDRTMPL_SHADOW>"
#define ID_MMFOLDER                "<MMPM2_FOLDER>"
/* The ID for the data configuration folder */
#define USERMENUFOLDER_MEDIAFOLDER "<USERMENU_MEDIAFOLDER>"

#define USERMENUFOLDER_M3UFOLDER "<USERMENU_CWM3UFolder>"

/************ INI file settings *************/
#define APPKEY_CWMM           "CWMM"
/* Setting for ini file to prevent template shadow creation in MM folder */
#define MFLDR_NO_TEMPLATE_SHADOW   "bNoMediaFolderTemplateShadow"
/* If set no blinking of the buttons while playing */
#define MFLDR_NO_VISUAL_FEEDBACK   "bNoPlayCtrlBlinking"
/* FreeDB root directory */
#define KEY_FREEDBCLIENTROOT  "freedbclient"

/*********** FreeDB client ******************/
/* Client for FreeDB query. This path is relative to the 'freedbclient' path specified in os2.ini see above. */
#define FREEDB_CLIENT_NAME    "\\pmcddb.exe"
/* Script sending the tracknames. Path is relative to install dir of cdfolder */
#define FREEDB_REXXSCRIPT     "\\bin\\trknames.cmd"

/* Flyover default settings. Not yet used */
#define MAXDELAY            9999       /* Delay for toolbar flyover */
#define DEFAULTDELAY        250

/* Fonts to use in dialogs */
#define DEFAULT_DIALOG_FONT   "9.WarpSans"
#define DEFAULT_DIALOG_FONT_WARP3   "8.Helv"


/*********** Setup strings ******************/
/* Setup strings for media folders */
#define MEDIAFLDR_PLAY_FIRST    "MEDIAFLDRPLAYFIRST"
#define MEDIAFLDR_PLAY    "MEDIAFLDRPLAY"
#define MEDIAFLDR_STOP    "MEDIAFLDRSTOP"
#define MEDIAFLDR_NEXT    "MEDIAFLDRNEXT"
#define MEDIAFLDR_PREV    "MEDIAFLDRPREV"
#define MEDIAFLDR_VOLUME  "MEDIAFLDRVOLUME"
#define MEDIAFLDR_REPEAT  "MEDIAFLDRREPEAT"
/* Setup strings for CD folder.
   Usage: CDPLAYERDRIVE=O: */
#define CDFLDR_DRIVE      "CDPLAYERDRIVE"
/* The template for creting IDs for CD player objects.
   For example: <CWMM_CDPLAYER_N> */
#define CDFLDR_ID         "<CWMM_CDPLAYER_%c>"
/* Destination for CD player objects */
#define CDFLDR_LOCATION "<MMPM2_FOLDER>"
/* CD folder classname */
#define CDFLDR_CLASSNAME    "CWCDFolder"
/* Setup string to hide the class page of menu folders */
#define SETUP_HIDECLASSPAGE  "HIDECLASSPAGE"

/* Script which creates the CD player objects */
#define SCRIPT_CREATEPLAYERS "creatobj.cmd"

/* The private media folder view when playing a track */
#define OPEN_PLAYING           OPEN_USER+1000

/* Which track to start */
#define PLAY_FIRST    1L
#define PLAY_NEXT     2L
#define PLAY_PREV     3L
#define PAUSE_TRACK   4L
#define STOP_TRACK    5L
#define SEEK_TRACK    6L

/* The IDs for the playtime display */
#define IDTIMER_PLAY  5L
#define PLAYTIMER_DELAY 1000

/* Timer ID for status text */
#define IDTIMER_STATUS 1

/* Slider window class name */
#define SLIDER_CLASS     "CWSLIDER"

/* Data positions in the window words */
#define SLIDERARMWIDTH    0  /* USHORT */
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

/* Useful defines */
#define MRFALSE (MRESULT)FALSE
#define MRTRUE (MRESULT)TRUE

/* Used for sending a click event from the time control to the main control */
#define NOTIFY_CLICK       1 /* For WM_APPTERMINATENOTIFY */
/* Same for context menu */
#define NOTIFY_CONTEXT     2
#define NOTIFY_STARTSTATUSTIMER 3

/* for cwmmQueryTrackInfo() */
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

/* From CWCDTrack */
#define IDINFO_TRACKNUM          100

#ifndef CTXT_CHANGETOICON
#define CTXT_CHANGETOICON   0x4000000
#define CTXT_CHANGETOTREE   0x8000000
#define CTXT_CHANGETODETAILS 0x10000000

/* This shouldn't be static... But there's an overflow check so at least
there's no crash id anyone has more objects in a playlist.  */
#define MAX_RANDOM_OBJECTS   5000  /* Max objects we use when playing random */

/* Internal REXX scripts */
#define RX_INSERT_PLAYLISTTRACKS    "\\bin\\insTrack.rx"
#define RX_INSERT_MEDIAFLDRTRACKS    "\\bin\\insTrk2.rx"
/*#define RX_CREATE_AUDIOCD           "mkaudCD.rx"*/

/* Additional window bytes for all the media folders */
#define MFLDR_WINDOWBYTES  24

/* Name of the DLL conatining the resources. %c%c will be replaced by the language code. */
#define RESDLLNAME "\\mfldr_%c%c.dll"
/* Default resource DLL name if NLV is not available. */
#define DEFRESDLLNAME "\\mfldr_en.dll"

#endif


