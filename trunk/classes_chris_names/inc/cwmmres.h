/* Audio icons */
#define ID_ICONCWAUDIOFILE  2
#define ID_ICONMP3FILE      3
#define ID_ICONOGGFILE      4
#define ID_ICONWAVFILE      5
#define ID_ICONAIFFILE      6
#define ID_ICONAUFILE       7
#define ID_ICON_AUFILE      8
#define ID_ICONIFFFILE      9
#define ID_ICONSNDFFILE     10
#define ID_ICONVOCFILE      11
#define ID_ICONMIDIFILE     12

/* Video icons */
#define ID_ICONCWVIDEOFILE  13
#define ID_ICONMPGFILE      14
#define ID_ICONMOVFILE      15
#define ID_ICONFLIFILE      16
#define ID_ICONAVIFILE      17

/* Image icons */
#define ID_ICONCWIMAGEFILE 18
#define ID_ICONAVCFILE     19
#define ID_ICONDIBFILE     20
#define ID_ICONGIFFILE     21
#define ID_ICONJPGFILE     22
#define ID_ICONPCDFILE     23
#define ID_ICONPCXFILE     24
#define ID_ICONTGAFILE     25
#define ID_ICONTIFFILE     26
#define ID_ICON_IMFILE     27
#define ID_ICONBMPFILE     28

/* Lighttable folder */
#define ID_ICONLIGHTTABLE  29
#define ID_ICONLIGHTTABLE2 30

/* Audio stream */
#define ID_ICONAUDIOSTREAM 31

/* Menu IDs */
#define ID_MENU_VOLUME      0x6501
#define ID_MENU_START       0xbbef /*0x6502*/
#define ID_MENU_STOP        0xbbf9 /*0x6503*/
#define ID_MENU_EDITOR      0xbc0d  /*0x6504*/
#define ID_MENU_PLAYER      0xbbe5  /*0x6505*/
#define ID_MENU_CONVERT     0xbc03
#define ID_ITEM_CONVERT     0xb4dd
#define ID_ITEM_FIRSTCONVERT     0xb4dd
#define ID_MENU_IMGEDITOR   0xbc2b

#define ID_MENUICONSIZE     0xbc00
#define ID_NORMALICONSITEM  0xbc01
#define ID_50X50ITEM        0xbc02
#define ID_100X100ITEM      0xbc03
#define ID_150X150ITEM      0xbc04
#define ID_200X200ITEM      0xbc05
#define ID_250X250ITEM      0xbc06

/* Audio information page dialog */
#define IDDLG_WAVEINFOPAGE 100
#define IDGB_WAVEINFO      110
#define IDST_SAMPLERATELABEL        101
#define IDST_BPSLABEL               102
#define IDST_PLAYTIMELABEL          103
#define IDST_CHANNELSLABEL          104
#define IDST_PLAYTIME               105
#define IDST_SAMPLERATE             106
#define IDST_BPS                    107
#define IDST_CHANNELS               108

/* String IDs */
#define ID_WAVEINFOPAGENAME 1
#define IDSTR_TITLE         2
#define IDSTR_ARTIST        3
#define IDSTR_ALBUM         4
#define IDSTR_YEAR          5
#define IDSTR_COMMENT       6
#define IDSTR_GENRE         7

#define IDSTR_PLAYTIME      8
#define IDSTR_SAMPLERATE    9
#define IDSTR_CHANNELS      10
#define IDSTR_BPS           11
#define ID_ID3TAGSPAGENAME  12

#define IDSTR_BITRATE       13
#define IDSTR_VBR           14

/* Titles audio classes */
#define IDSTR_AIFTITLE       15
#define IDSTR_AUTITLE        16
#define IDSTR__AUTITLE       17
#define IDSTR_IFFTITLE       18
#define IDSTR_SNDFTITLE      19
#define IDSTR_VOCTITLE       20
#define IDSTR_WAVTITLE       21
#define IDSTR_OGGTITLE       22
#define IDSTR_MP3TITLE       23
#define IDSTR_MIDITITLE      24

/* Titles video classes */
#define IDSTR_CWVIDEOTITLE   25
#define IDSTR_MPGTITLE       26
#define IDSTR_MOVTITLE       27
#define IDSTR_FLITITLE       28
#define IDSTR_AVITITLE       29

#define IDSTR_CWAUDIOTITLE   30
#define IDSTR_CWMMDATAFILETITLE   31
#define IDSTR_HELPLIBRARY    32

/* Convert menu item string */
#define IDSTR_MENU_CONVERT  33
/* Titles image classes */
#define IDSTR_AVCTITLE      34
#define IDSTR_DIBTITLE      35
#define IDSTR_GIFTITLE      36
#define IDSTR_JPGTITLE      37
#define IDSTR_PCDTITLE      38
#define IDSTR_PCXTITLE      39
#define IDSTR_TGATITLE      40
#define IDSTR_TIFTITLE      41
#define IDSTR_MMOTIONTITLE  42
#define IDSTR_CWIMAGETITLE  43

#define IDSTR_IMAGEINFOPAGENAME 44
#define IDSTR_SPRINTFPIXEL   45 /* Used with sprintf to build the 'xxx Pixel' string */	
#define IDSTR_SPRINTFBIT     46 /* Used with sprintf to build the 'xx Bit' string */

#define IDSTR_NOIMGIOPROCERROR   47
#define IDSTR_OPENFILEERRORTITLE 48
#define IDSTR_OPENFILEERROR      49
#define IDSTR_NOMEMERROR         50
#define IDSTR_IMGIOPROCCRASH     51

#define IDSTR_LIGHTTABLETITLE    52 /* Ligthtable title */

#define IDSTR_ICONSIZEMENU       53 /* MEnu icon size */

/* Column titles for image details view */
#define IDSTR_IMAGEDETAIL_WIDTH  54
#define IDSTR_IMAGEDETAIL_HEIGHT 55
#define IDSTR_IMAGEDETAIL_DEPTH  56
#define IDSTR_IMAGEDETAIL_FORMAT 57

#define ID_TRACKNAMEPAGENAME     58 /* PAge name for the track name settings */
#define IDSTR_TRACKNAME          59 /* Name for the groupbox for the track name dialog */
#define IDSTR_STARTINGTRACK      60 /* String to be shown with the title of an audio object
                                     when started from the WPS. */
#define IDSTR_AUDIOSCANNING      61 /* Audio scanning window title */
#define IDSTR_AUDIOSTREAMTITLE   62
#define IDSTR_STREAMURLPAGENAME  63

/* ID3 tag dialog */
#define IDDLG_ID3TAGS               200
#define IDPB_ID3SAVE                201
#define IDPB_CANCEL                 202
#define IDPB_ID3HELP                203
#define IDEF_ID3NAME                204
#define IDEF_ID3ARTIST              205
#define IDEF_ID3ALBUM               206
#define IDEF_ID3COMMENT             207
#define IDEF_ID3YEAR                208
#define IDDD_ID3GENRE               209
#define IDGB_ID3TAGS                216

/* Image information dialog */
#define IDDLG_IMAGEINFOPAGE         300
#define IDGB_IMAGEINFO              301
#define IDST_IMAGEWIDTHLABEL        302
#define IDST_IMAGEHEIGHTLABEL       303
#define IDST_COLORDEPTHLABEL        304
#define IDST_IMAGEWIDTH             305
#define IDST_IMAGEHEIGHT            306
#define IDST_COLORDEPTH             307
#define IDST_IMAGEFORMAT            308
#define IDDLG_STREAMURLPAGE         400
#define IDGB_STREAMURL              401
#define IDEF_URL                    402
#define IDST_URLTEXT                403
