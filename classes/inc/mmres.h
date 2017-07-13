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

#define ID_ICONVOLUME 1

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
#define ID_ICONFLACFILE     34

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
#define ID_ICONPNGFILE     33
/* Lighttable folder */
#define ID_ICONLIGHTTABLE  29
#define ID_ICONLIGHTTABLE2 30
/* Audio stream icon */
#define ID_ICONAUDIOSTREAM 31
/* M3U icon*/
#define ID_ICONM3UFILE     32
/* Last icon id: 34 */



#define IDSTR_CONVERTCHANNELS     1614
#define IDSTR_CONVERTSAMPLERATE   1615
#define IDSTR_CONVERTPLAYTIME     1616
#define IDSTR_CONVERTDIALOGERROR  1617
#define IDSTR_CONVERTNOSPACETITLE 1618
#define IDSTR_CONVERTNOSPACETEXT  1619
#define IDSTR_INSTALLERRORTITLE   1620
#define IDSTR_AUDIOBROWSETITLE   1621
#define IDSTR_IMAGEBROWSETITLE   1622



/* Menu IDs */
#define ID_MENU_VOLUME      0x6501
#define ID_MENU_START       0xbbef
#define ID_MENU_STOP        0xbbf9
#define ID_MENU_PAUSE       0xbbfa /* Chosen by CW */
#define ID_MENU_RESUME      0xbbfb /* Chosen by CW */
#define ID_MENU_EDITOR      0xbc0d
#define ID_MENU_PLAYER      0xbbe5
#define ID_MENU_CONVERT     0xbc03
#define ID_ITEM_CONVERT     0xb4dd
#define ID_ITEM_FIRSTCONVERT     0xb4dd
#define ID_MENU_IMGEDITOR   0xbc2b
#define ID_MENU_M3UEDITOR   0xbc2c
#define ID_MENU_M3UPLAYER   0xbc2d

#define ID_MENUICONSIZE     0xbc00
#define ID_NORMALICONSITEM  0xbc01
#define ID_50X50ITEM        0xbc02
#define ID_100X100ITEM      0xbc03
#define ID_150X150ITEM      0xbc04
#define ID_200X200ITEM      0xbc05
#define ID_250X250ITEM      0xbc06
/* Static menu item with image info for popup menu */
#define ID_MENU_IMAGEINFO   0xbc10

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
#define IDSTR_FLACTITLE      72

/* Titles video classes */
#define IDSTR_CWVIDEOTITLE   25
#define IDSTR_MPGTITLE       26
#define IDSTR_MOVTITLE       27
#define IDSTR_FLITITLE       28
#define IDSTR_AVITITLE       29
#define IDSTR_VIDPLAYERTITLE     75

#define IDSTR_CWAUDIOTITLE   30
#define IDSTR_CWMMDATAFILETITLE   31
#define IDSTR_HELPLIBRARY    32

/* Convert menu item string */
#define IDSTR_MENU_CONVERT  33
#define IDSTR_MENU_IMAGEINFO 73

/* Titles image classes */
#define IDSTR_AVCTITLE      34
#define IDSTR_DIBTITLE      35
#define IDSTR_GIFTITLE      36
#define IDSTR_JPGTITLE      37
#define IDSTR_PCDTITLE      38
#define IDSTR_PCXTITLE      39
#define IDSTR_TGATITLE      40
#define IDSTR_TIFTITLE      41
#define IDSTR_PNGTITLE      71
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
#define IDSTR_ICONSIZEMENU       53 /* Menu icon size */

/* Column titles for image details view */
#define IDSTR_IMAGEDETAIL_WIDTH  54
#define IDSTR_IMAGEDETAIL_HEIGHT 55
#define IDSTR_IMAGEDETAIL_DEPTH  56
#define IDSTR_IMAGEDETAIL_FORMAT 57

#define ID_TRACKNAMEPAGENAME     58 /* Page name for the track name settings */
#define IDSTR_TRACKNAME          59 /* Name for the groupbox for the track name dialog */
#define IDSTR_STARTINGTRACK      60 /* String to be shown with the title of an audio object
                                     when started from the WPS. */
#define IDSTR_AUDIOSCANNING      61 /* Audio scanning window title */
#define IDSTR_AUDIOSTREAMTITLE   62 /* Class title for audio stream object */
#define IDSTR_STREAMURLPAGENAME  63
#define IDSTR_CDTRACKTITLE       64 /* Class title for CD tracks */
#define IDSTR_M3UTITLE           65 /* Class title for M3U playlists */
/* Titles used when starting player and editor applications */
#define IDSTR_AUDPLAYERTITLE     66
#define IDSTR_AUDEDITORTITLE     67
#define IDSTR_M3UEDITORTITLE     68
#define IDSTR_IMGEDITORTITLE     69
#define IDSTR_VOLUMETITLE        74
#define IDSTR_TRACKPAUSED        70
/* Last used id: 75 */

#define IDSTR_MASTERVOLUMEITEM    106

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

/* Dialog for audio stream class */
#define IDDLG_STREAMURLPAGE         400
#define IDPB_STREAMHELP                401
#define IDGB_STREAMURL              411
#define IDEF_URL                    412
#define IDST_URLTEXT                413

/* The main volume object */
#define IDDLG_VOLUME                500
#define IDCS_VOLUME                 501
#define IDPB_MUTE                   502
#define IDGB_VOLUME                 503
/* Menu items for the system menu of the volume object */
#define IDM_POPUP                   200
#define IDM_ITEMTITLEBAR            201
#define IDM_ITEMEXIT                202

#define IDDLG_CONVERT               600
#define IDGB_CONVERT                601
#define IDST_CONVERTNAMELABEL       602
#define IDST_CONVERTNAME            603
#define IDST_CONVERTSAMPLERATE      604
#define IDST_CONVERTCHANNELS        605
#define IDST_CONVERTCDBITS          606
#define IDST_CONVERTCDSAMPLELABEL   607
#define IDST_CONVERTCDCHANNELSLABEL 608
#define IDST_CONVERTPLAYTIME        609
#define IDEF_CONVERTTARGETNAME      610
#define IDPB_CONVERTBROWSE          611
#define IDBAR_CONVERTPROGRESS       612
#define IDPB_CONVERTOK              613
#define IDPB_CONVERTABORT           614
#define IDPB_CONVERTCLOSE           615
#define IDDD_IOPROC                 616
#define IDGB_CONVERT2               618

/* The image converter app */
#define IDDLG_IMGCONVERT               700
#define IDGB_IMGCONVERT                701
#define IDST_IMGCONVERTNAMELABEL       702
#define IDST_IMGCONVERTNAME            703
#define IDSR_BMP                    704
#define IDEF_IMGCONVERTTARGETNAME      710
#define IDPB_IMGCONVERTBROWSE          711
#define IDBAR_IMGCONVERTPROGRESS       712
#define IDPB_IMGCONVERTOK              713
#define IDPB_IMGCONVERTABORT           714
#define IDPB_IMGCONVERTCLOSE           715
#define IDDD_IMGIOPROC                 716
#define IDST_IMGCONVERTTO              717

/* The MM files find application (mmfind.exe) */
#define IDDLG_MMFIND                   800
#define IDPB_NEWFILE                   801
#define IDST_TYPE                      802
#define IDCB_TYPEOFFILE                803
#define IDGB_PREVIEW                   804
#define IDSR_IMGPREVIEW                805
#define IDST_NOPREVIEWAVAILABLE        806
#define IDST_AUDIOINFO                 807
#define IDGB_AUDIOINFO                 808
#define IDSTR_TYPEAUDIO                820
#define IDSTR_TYPEIMAGE                821
#define IDSTR_TYPEVIDEO                822
#define IDSTR_AUDIOINFOTEXT            823
#define IDSTR_NOMMTEMPLATEFLDRTITLE    824
#define IDSTR_NOMMTEMPLATEFLDR         825

/* The following IDs are predefined for the OS/2 file dialog */

/* Small volume app started from context menu */
#define IDDLG_CLASSVOLUME          1000
#define ID_CLASSVOLCLIENT           1001

/* Audio player (mmplayer.exe) */
#define IDDLG_MMPLAYER               2100
#define ID_CLIENT                 2101
#define IDPB_MMPLAYERPLAY                   2103
#define IDPB_MMPLAYERPAUSE                  2104
#define IDPB_MMPLAYERSTOP                   2105
#define IDST_MMPLAYERPLAYTIME               2107
#define IDSL_MMPLAYERVOLUMEARM              2108
#define IDSL_MMPLAYERVOLUME                 2109
#define IDPB_MMPLAYERPOSBAR                 2110
#define IDPB_MMPLAYERSLIDERARM              2111
#define IDSL_MMPLAYERPOSBAR                 2112

#define ID_MENUTIMECONTEXT        1
#define ID_MENUITEMELAPSED        2
#define ID_MENUITEMREMAINING      3
#define ID_MENUITEMID3            4
#define ID_MENUITEMMASTERVOLUME 5

