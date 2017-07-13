.******************************************************

:h2 res=1010
width=30%
.cwGetFileSystemObject
:link reftype=hd res=1011
auto dependent group=2.
:p.
:link reftype=hd res=1011.Syntax:elink.
.br
:link reftype=hd res=1013 dependent.Returns:elink.
.br
:link reftype=hd res=1014 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=315.Usage:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h2 res=1011
x=30%
width=70%
hide
group=2
.cwGetFileSystemObject - Syntax
:xmp.
/* Get the file system object a shadow links to */

mediaFolder   /* Pointer to a media folder */
theShadow     /* A shadow object  */
:p.
fsObj=MFldrCallFunc("cwGetFileSystemObject", 
                        mediaFolder, theShadow);
:exmp.


:h2 res=1013
x=30%
width=70% height=35%
group=3
hide
.cwGetFileSystemObject - Return value
:p.
:hp2.fsObj:ehp2. numeric
:p.
The return value is a pointer to the file system object the shadow
:hp2.theShadow:ehp2. links to. If the link is broken 0 is returned.


:h2 res=1014
x=30%
width=70%
hide
group=2
.cwGetFileSystemObject - Remarks
:p.
This method is a member of the media folder therefore an object pointer to
a media folder must be provided.
:p.
The method was introduced by the CWProgFolder class which is a parent class
for several new folder classes. So you may also use a pointer to any other
subclass of CWProgFolder. Some of these subclasses are listed below.

:ul.
:li.CWAudioFolder
:li.CWDataFolder
:li.CWMediaFolder
:eul.



.******************************************************
:h2 res=1040
width=30%
.cwPauseAudioFile
:link reftype=hd res=1041
auto dependent group=2.
:p.
:link reftype=hd res=1041.Syntax:elink.
.br
:link reftype=hd res=1043 dependent.Returns:elink.
.br
:link reftype=hd res=1044 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=315.Usage:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h2 res=1041
x=30%
width=70%
hide
group=2
.cwPauseAudioFile - Syntax
:xmp.
/* This method pauses playing */

mediaFolder   /* Pointer to the media folder */
frameHandle   /* Handle of the folder frame  */
:p.
rc=MFldrCallFunc("cwPauseAudioFile", 
                        mediaFolder, frameHandle);
:exmp.


:h2 res=1043
x=30%
width=70% height=35%
group=3
hide
.cwPauseAudioFile - Return value
:p.
:hp2.rc:ehp2. numeric
:p.


:h2 res=1044
x=30%
width=70%
hide
group=2
.cwPauseAudioFile - Remarks
:p.
The frame handle of the media folder is given to the skript as a parameter
when started.

.******************************************************
:h2 res=1050
width=30%
.cwPlayAudioFile
:link reftype=hd res=1051
auto dependent group=2.
:p.
:link reftype=hd res=1051.Syntax:elink.
.br
:link reftype=hd res=1053 dependent.Returns:elink.
.br
:link reftype=hd res=1054 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=315.Usage:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h2 res=1051
x=30%
width=70%
hide
group=2
.cwPlayAudioFile - Syntax
:xmp.
/* This method starts playing the contents 
    of the media folder */

mediaFolder   /* Pointer to the media folder */
frameHandle   /* Handle of the folder frame  */
flag          /* Flag specifying which track to start */
:p.
rc=MFldrCallFunc("cwPlayAudioFile", 
                        mediaFolder, frameHandle, flag);
:exmp.

:h2 res=1053
x=30%
width=70% height=35%
group=3
hide
.cwPlayAudioFile - Return value
:p.
:hp2.rc:ehp2. numeric
:p.


:h2 res=1054
x=30%
width=70%
hide
group=2
.cwPlayAudioFile - Remarks
:p.
The frame handle of the media folder is given to the skript as a parameter
when started. The flag parameter may have one of the following values.

:ul.
:li.PLAY_FIRST    1
.br
Start the first audio track in the media folder. If any track is selected
start with the first selected track.
:li.PLAY_NEXT     2
.br
Play the next track.
:li.PLAY_PREV     3
.br
Play previous track.
:eul.


.******************************************************
:h2 res=1060
width=30%
.cwQueryCurrentlyPlayingObject
:link reftype=hd res=1061
auto dependent group=2.
:p.
:link reftype=hd res=1061.Syntax:elink.
.br
:link reftype=hd res=1063 dependent.Returns:elink.
.*.br
.*:link reftype=hd res=1054 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=315.Usage:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h2 res=1061
x=30%
width=70%
hide
group=2
.cwQueryCurrentlyPlayingObject - Syntax
:xmp.
/* This method queries the pointer to the 
   currently playing object */

mediaFolder   /* Pointer to the media folder */
:p.
rc=MFldrCallFunc("cwQueryCurrentlyPlayingObject", 
                  mediaFolder);
:exmp.

:h2 res=1063
x=30%
width=70% height=35%
group=3
hide
.cwQueryCurrentlyPlayingObject - Return value
:p.
:hp2.rc:ehp2. numeric
:p.
Object pointer to the currently playing object or 0 if no object is
playing.



.******************************************************
:h2 res=1090
width=30%
.cwSetStatusLineText
:link reftype=hd res=1091
auto dependent group=2.
:p.
:link reftype=hd res=1091.Syntax:elink.
.br
:link reftype=hd res=1093 dependent.Returns:elink.
.br
:link reftype=hd res=1094 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=315.Usage:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h2 res=1091
x=30%
width=70%
hide
group=2
.cwSetStatusLineText - Syntax
:xmp.
/* This method sets the status line text of a media folder */

mediaFolder   /* Pointer to a media folder on which the method is invoked */
theText       /* Text for status line or 0 */
:p.
info=MFldrCallFunc("cwSetStatusLineText", 
                    mediaFolder, theText);
:exmp.

:h2 res=1093
x=30%
width=70% height=35%
group=3
hide
.cwSetStatusLineText - Return value
:p.
:hp2.info:ehp2. string
:p.


:h2 res=1094
x=30%
width=70%
hide
group=2
.cwSetStatusLineText - Remarks
:p.
If the text parameter is 0 the folder shows it's standard
status line text.


.******************************************************
:h2 res=1100
width=30%
.cwStopAudioFile
:link reftype=hd res=1101
auto dependent group=2.
:p.
:link reftype=hd res=1101.Syntax:elink.
.br
:link reftype=hd res=1103 dependent.Returns:elink.
.br
:link reftype=hd res=1104 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=315.Usage:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h2 res=1101
x=30%
width=70%
hide
group=2
.cwStopAudioFile - Syntax
:xmp.
/* This method stops playing */

mediaFolder   /* Pointer to the media folder */
frameHandle   /* Handle of the folder frame  */
:p.
rc=MFldrCallFunc("cwStopAudioFile", 
                        mediaFolder, frameHandle);
:exmp.


:h2 res=1103
x=30%
width=70% height=35%
group=3
hide
.cwStopAudioFile - Return value
:p.
:hp2.rc:ehp2. numeric
:p.


:h2 res=1104
x=30%
width=70%
hide
group=2
.cwStopAudioFile - Remarks
:p.
The frame handle of the media folder is given to the skript as a parameter
when started.


.******************************************************
:h2 res=1070
width=30%
.cwmmQueryTrackInfo
:link reftype=hd res=1071
auto dependent group=2.
:p.
:link reftype=hd res=1071.Syntax:elink.
.br
:link reftype=hd res=1073 dependent.Returns:elink.
.br
:link reftype=hd res=1074 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=315.Usage:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h2 res=1071
x=30%
width=70%
hide
group=2
.cwmmQueryTrackInfo - Syntax
:xmp.
/* This method queries information from an
   audio object */

audioObject   /* Pointer to an audio object on which the method is invoked */
flag          /* Flag specifying which info to query */
:p.
info=MMClsCallFunc("cwmmQueryTrackInfo", 
                  audioObject, flag);
:exmp.

:h2 res=1073
x=30%
width=70% height=35%
group=3
hide
.cwmmQueryTrackInfo - Return value
:p.
:hp2.info:ehp2. string
:p.
The requested information is returned as a string. If no information
is available an empty string is returned.
:p.
If an error occurs :hp2.ERROR&colon.:ehp2. is returned.

:h2 res=1074
x=30%
width=70%
hide
group=2
.cwmmQueryTrackInfo - Remarks
:p.
The object must be an instance of :hp2.CWAudio:ehp2. or of one of it's
subclasses (:hp2.CWMP3:ehp2., :hp2.MMWAV:ehp2....). This method was introduced
by :hp2.CWAudio:ehp2.. Since V0.2.5 of the classes the class names are :hp2.MMAudio:ehp2.
:hp2.MMWAV:ehp2. etc. to be compatible with the IBM classes coming with OS/2.
:p.
The flag parameter may have one of the following values.
:ul.
:li.IDINFO_NAME              1
.br
Trackname
:li.IDINFO_ARTIST            2
.br
Name of artist.
:li.IDINFO_ALBUM             3
.br
Name of album.
:li.IDINFO_YEAR              4
.br
Year from ID3 tag
:li.IDINFO_COMMENT           5
.br
Comment.
:li.IDINFO_GENRE             6
.br
The genre as a string.
:li.IDINFO_PLAYTIME          7
.br
Play time in seconds (numeric value).
:li.IDINFO_BPS               8
.br
Bits per sample (numeric value).
:li.IDINFO_SAMPLERATE        9
.br
Samplerate in Hz (numeric value).
:li.IDINFO_CHANNELS          10
.br
Number of channels (numeric value).
:li.IDINFO_BITRATE           11
.br
Bitrate in Hz (numeric value). This flag is only valid for instances of class MMMP3 and
MMOGG. For other classes the return value is undefined. New with V0.2.7.
:eul.


.******************************************************
:h2 res=1080
width=30%
.cwmmQueryImageInfo
:link reftype=hd res=1081
auto dependent group=2.
:p.
:link reftype=hd res=1081.Syntax:elink.
.br
:link reftype=hd res=1082 dependent.Parameters:elink.

.br
:link reftype=hd res=1083 dependent.Returns:elink.
.*.br
.*:link reftype=hd res=1084 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=315.Usage:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h2 res=1081
x=30%
width=70%
hide
group=2
.cwmmQueryImageInfo - Syntax
:xmp.
/* This method queries information from an
   image object */

imageObject   /* Pointer to an image object on which the method is invoked */
flag          /* Flag specifying which info to query */
:p.
info=MMCallFunc("cwmmQueryTrackInfo", 
                  imageObject, flag);
:exmp.

:h2 res=1083
x=30%
width=70% height=35%
group=3
hide
.cwmmQueryImageInfo - Return value
:p.
:hp2.info:ehp2. string
:p.
The requested information is returned as a string. If no information
is available an empty string is returned.
:p.
If an error occurs :hp2.ERROR&colon.:ehp2. is returned.

:h2 res=1082
x=30%
width=70%
hide
group=2
.cwmmQueryImageInfo - Parameters
:p.
:parml tsize=10.
:pt.:hp2.imageObject:ehp2. (numeric)
:pd.Object pointer of the image file.
:pt.:hp2.flag:ehp2. (numeric)
:pd.Determines the information to be queried.
:parml tsize=3.
:pt.IDINFO_WIDTH    0x0001
:pd.The width of the image.
:pt.IDINFO_HEIGHT   0x0002
:pd.The height of the image.
:pt.IDINFO_BPP      0x0003  
:pd.Number of bits per pixel.
:pt.IDINFO_FORMAT   0x0004  
:pd.Image format.
:eparml.
:pt.:hp2.info:ehp2. (numeric)
:pd.The queried information as a string.
:eparml.





