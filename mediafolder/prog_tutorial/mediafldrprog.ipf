:userdoc.

:docprof.

:title.Programming tutorial for Multimedia classes 

:h1 res=100.Introduction
:p.
The Media-Folder offers two interfaces for controlling and programming. You
may use :link reftype=hd res=200.setup strings:elink. for basic tasks like starting and stopping the playing
or a :link reftype=hd res=300.REXX interface:elink. with access to internal class methods.
:p.
For use of the REXX interface
:p.
:link reftype=launch
object='netscape.exe'
data='http://www.os2world.com/cdwriting'.
WPS-Wizard V0.2.0:elink. (http&colon.//www.os2world.com/cdwriting)
:p.
or above must be installed. 

:h1 res=200
x=left y=bottom width=50%.Setup strings
:p.
The following additional setup strings are supported with every media folder.

:ul.
:li.:link reftype=hd res=210.MEDIAFLDRNEXT:elink.   Play the next track in the folder
:li.:link reftype=hd res=211.MEDIAFLDRPLAY:elink.   Start playing
:li.:link reftype=hd res=215.MEDIAFLDRPLAYFIRST:elink.   Start first track
:li.:link reftype=hd res=212.MEDIAFLDRPREV:elink.   Skip to previus track
:li.:link reftype=hd res=216.MEDIAFLDRREPEAT:elink. Set repeat.
:li.:link reftype=hd res=213.MEDIAFLDRSTOP:elink.   Stop playing
:li.:link reftype=hd res=214.MEDIAFLDRVOLUME:elink. Set volume
:eul.
:p.
The folder must be open for these setup strings to work.
:p.
The MM audio classes support the following strings:
:ul.
:li.:link reftype=hd res=220.MMAUDIOTRACKNAME:elink.   Set the track name of the audio file.
:li.:link reftype=hd res=221.MMAUDIOARTIST:elink.   Set the artist name for the audio file.
:li.:link reftype=hd res=222.MMAUDIOALBUM:elink.    Set the album name.
:li.:link reftype=hd res=223.MMAUDIOCOMMENT:elink.  Set the comment for the audio file.
:li.:link reftype=hd res=224.MMAUDIOYEAR:elink.     Set the year of the audio file.
:li.:link reftype=hd res=225.MMAUDIOGENRE:elink.    Set the audio file genre.
:li.:link reftype=hd res=226.MMREFRESH:elink.       Reread the infos of the audio file.
:li.:link reftype=hd res=227.MMAUDIOCOPYTAGTO:elink.    Copy all tags of the audio file.

:eul.


:h2 res=210
x=50% width=50%
group=2
.MEDIAFLDRNEXT
:p.
Skip to next track while playing. If not yet playing the first track will be started.
:xmp.
/* REXX - This command skips to the next track */

rc=SysSetObjectData('x&colon.\path_to_folder','MEDIAFLDRNEXT=1')
:exmp.

:h2 res=211
x=50% width=50%
group=2
.MEDIAFLDRPLAY
:p.
Start playing with the first track. If any track is selected playing will start with the first selected track.
:xmp.
/* REXX - This command starts playing */

rc=SysSetObjectData('x&colon.\path_to_folder','MEDIAFLDRPLAY=1')
:exmp.

:h2 res=215
x=50% width=50%
group=2
.MEDIAFLDRPLAYFIRST
:p.
Start playing with the first track no matter if any other track is selected.

:xmp.
/* REXX - This command starts playing */

rc=SysSetObjectData('x&colon.\path_to_folder','MEDIAFLDRPLAYFIRST=1')
:exmp.


:h2 res=212
x=50% width=50%
group=2
.MEDIAFLDRPREV
:p.
Skip to the previous track while playing.
:xmp.
/* REXX - This command skips to the previous track */

rc=SysSetObjectData('x&colon.\path_to_folder','MEDIAFLDRPREV=1')
:exmp.

:h2 res=216
x=50% width=50%
group=2
.MEDIAFLDRREPEAT
:p.
Set the repeat attribute. If set to 1 the tracks in the folder are
repeated. If set to 0 repeating is disabled.
:xmp.
/* REXX - This command sets repeat to yes */

rc=SysSetObjectData('x&colon.\path_to_folder','MEDIAFLDRREPEAT=1')
:exmp.

:h2 res=213
x=50% width=50%
group=2
.MEDIAFLDRSTOP
:p.
Stop playing.
:xmp.
/* REXX - This command stops playing */

rc=SysSetObjectData('x&colon.\path_to_folder','MEDIAFLDRSTOP=1')
:exmp.

:h2 res=214
x=50% width=50%
group=2
.MEDIAFLDRVOLUME
:p.
Change the volume between 0 and 100%.
:xmp.
/* REXX - This command sets the volume to 35% */

rc=SysSetObjectData('x&colon.\path_to_folder','MEDIAFLDRVOLUME=35')
:exmp.

.********************************************

:h2 res=220
x=50% width=50%
group=2
.MMAUDIOTRACKNAME
:p.
Set the track name of the audio file. This information is saved in the EA of the file. For
MP3 files this information id also saved in the ID3 tag.
:xmp.
/* REXX - This command adds the track name to the audio file */

rc=SysSetObjectData('x&colon.\path_to_folder\audio_file.ext','MMAUDIOTRACKNAME=The track name')
:exmp.

:h2 res=221
x=50% width=50%
group=2
.MMAUDIOARTIST
:p.
Set the artist name of the audio file. This information is saved in the EA of the file. For
MP3 files this information is also saved in the ID3 tag.
:xmp.
/* REXX - This command adds the artist name to the audio file */

rc=SysSetObjectData('x&colon.\path_to_folder\audio_file.ext','MMAUDIOARTIST=Artist name')
:exmp.

:h2 res=222
x=50% width=50%
group=2
.MMAUDIOALBUM
:p.
Set the album name of the audio file. This information is saved in the EA of the file. For
MP3 files this information is also saved in the ID3 tag.
:xmp.
/* REXX - This command adds the album name to the audio file */

rc=SysSetObjectData('x&colon.\path_to_folder\audio_file.ext','MMAUDIOALBUM=Album name')
:exmp.

:h2 res=223
x=50% width=50%
group=2
.MMAUDIOCOMMENT
:p.
Set the comment of the audio file. This information is saved in the EA of the file. For
MP3 files this information is also saved in the ID3 tag.
:xmp.
/* REXX - This command adds a comment to the audio file */

rc=SysSetObjectData('x&colon.\path_to_folder\audio_file.ext','MMAUDIOCOMMENT=Track comment')
:exmp.

:h2 res=224
x=50% width=50%
group=2
.MMAUDIOYEAR
:p.
Set the year of the audio file. This information is saved in the EA of the file. For
MP3 files this information is also saved in the ID3 tag.
:xmp.
/* REXX - This command adds the year to the audio file */

rc=SysSetObjectData('x&colon.\path_to_folder\audio_file.ext','MMAUDIOYEAR=2003')
:exmp.

:h2 res=225
x=50% width=50%
group=2
.MMAUDIOGENRE
:p.
Set the genre of the audio file. This information is saved in the EA of the file. For
MP3 files this information is also saved in the ID3 tag.
:xmp.
/* REXX - This command adds the genre of the audio file */

rc=SysSetObjectData('x&colon.\path_to_folder\audio_file.ext','MMAUDIOGENRE=Blues')
:exmp.

:h2 res=226
x=50% width=50%
group=2
.MMREFRESH
:p.
Reread the audio information from the file and update internal object data.
:xmp.
/* REXX - This command adds the genre of the audio file */

rc=SysSetObjectData('x&colon.\path_to_folder\audio_file.ext','MMREFRESH=1')
:exmp.

:h2 res=227
x=50% width=50%
group=2
.MMAUDIOCOPYTAGTO
:p.
Reread the audio information from the file and update internal object data.
:xmp.
/* REXX - This command adds the genre of the audio file */

rc=SysSetObjectData('x&colon.\path_to_folder\audio_file.ext','MMAUDIOCOPYTAGTO=x&colon.\path_to_folder\another_file.ext')
:exmp.


.********************************************

:h1 res=300.REXX interface
:p.
When using the REXX interface skripts have access to the class methods of the media folder.
Support for the most important methods of the WPS, helper functions and some PM window functions is provided by
WPS-Wizard.

:p.
The media folder methods are called using a wrapper function.

:table cols='20 35'.
:row.
:c.:link reftype=hd res=320.MFldrCallFunc:elink.
:c.Call one of the methods introduced by the media folder.
:etable.
:p.
Methods introduced by the multimedia classes are called using the following function.
:table cols='20 35'.
:row.
:c.:link reftype=hd res=330.MMCallFunc:elink.
:c.Call one of the methods introduced by multimedia classes (CWAudio, CWMP3, CWOGG).
:etable.

:p.
The skript runs in the context of the WPS which is a PM program on a separate thread. You may
use additional REXX dlls available. The name of the REXX skript does not matter but the file
must as usual start with a comment.

.*************************************

.***********************************************

:h2 res=320
width=30%
.MFldrCallFunc
:link reftype=hd res=321
auto dependent group=2.
:p.
Use this function to call one of the new media folder methods or a
helper method.
:p.
:link reftype=hd res=321 dependent.Syntax:elink.
.*.br
.*:link reftype=hd res=322.Parameters:elink.
.br
:link reftype=hd res=323 dependent.Returns:elink.
.br
:link reftype=hd res=324 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=325.Usage:elink.
.br
:link reftype=hd res=326.Example:elink.

:h2 res=321
x=30%
width=70%
group=2
hide
.MFldrCallFunc - Syntax
:xmp.
/* Call a method from REXX */

methodName  /* The name of a method */
Object         /* Object on which the method is invoked */
param1         /* First method parameter */
param2         /* Second method parameter */
:p.
rc=MFldrCallFunc(methodName, Object, 
                 param1, param2, ...);
:exmp.


:h2 res=322
x=30%
width=70%
group=2
hide
.MFldrCallFunc - Parameters
:p.

:h2 res=323
x=30%
width=70% height=35%
group=3
hide
.MFldrCallFunc - Return value
:p.
The return value depends on the method being called. It may be a string or a
numeric value. See the method description for more info.
:p.
If the method can't be found the string :hp2.ERROR_METHOD&colon.:ehp2. is returned.

:h2 res=324
x=30%
width=70%
group=2
hide
.MFldrCallFunc - Remarks
:p.
The method to be called must be a member of the object :hp2.Object:ehp2. An error occurs if
the object does not support the requested method. For example a CWMediaFolder object does
not support the same methods as a CWAudio object.

:h2 res=325
x=30%
width=70%
group=2
hide
.MFldrCallFunc - Usage
:p.

:h2 res=326
x=30%
width=70%
group=2
hide
.MFldrCallFunc - Example
:xmp.
/* This example queries the first object in  */
/* a CWMediaFolder, gets the linked file and */
/* queries the name of the artist from the   */
/* ID3 tag.                                  */

/* Get the first object of folder theFolder */
theObject=MFldrCallWPSFunc("wpQueryContent", theFolder, 0, QC_FIRST)

/* Get the linked file system object from the shadow.
   cwGetFileSystemObject is a member of the
   CWMediaFolder class. */
theFileObj=MFldrCallFunc("cwGetFileSystemObject", theFolder, theObject)

/* Get the artist name. cwmmQueryTrackInfo is a member of the
   CWAudio class. Every object in the CWMediaFolder is usually
   a shadow to an object instance of a subclass of CWAudio */
theName=MFldrCallFunc("cwmmQueryTrackInfo", theFileObj, 2)

/* Display the name */
rc=RxMessageBox("The artist name : "||theName)
:exmp.

.*********************************************
.***********************************************

:h2 res=330
width=30%
.MMCallFunc
:link reftype=hd res=331
auto dependent group=2.
:p.
Use this function to call one of the new multi media class methods introduced with :hp2.CWAudio/MMAudio:ehp2..
:p.
:link reftype=hd res=331 dependent.Syntax:elink.
.*.br
.*:link reftype=hd res=332.Parameters:elink.
.br
:link reftype=hd res=333 dependent.Returns:elink.
.br
:link reftype=hd res=334 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=335.Usage:elink.
.br
:link reftype=hd res=336.Example:elink.

:h2 res=331
x=30%
width=70%
group=2
hide
.MMCallFunc - Syntax
:xmp.
/* Call a method from REXX */

methodName  /* The name of a method */
Object         /* Object on which the method is invoked */
param1         /* First method parameter */
param2         /* Second method parameter */
:p.
rc=MMCallFunc(methodName, Object, 
                 param1, param2, ...);
:exmp.


:h2 res=332
x=30%
width=70%
group=2
hide
.MMCallFunc - Parameters
:p.

:h2 res=333
x=30%
width=70% height=35%
group=3
hide
.MMCallFunc - Return value
:p.
The return value depends on the method being called. It may be a string or a
numeric value. See the method description for more info.
:p.
If the method can't be found the string :hp2.ERROR_METHOD&colon.:ehp2. is returned.

:h2 res=334
x=30%
width=70%
group=2
hide
.MMCallFunc - Remarks
:p.
The method to be called must be a member of the object :hp2.Object:ehp2. An error occurs if
the object does not support the requested method. For example a CWMediaFolder object does
not support the same methods as a CWAudio object.
:p.
In previous releases of the classes this function had the name :hp2.MMClsCallFunc:ehp2.. This
name is still supported but depracted.

:h2 res=335
x=30%
width=70%
group=2
hide
.MMCallFunc - Usage
:p.

:h2 res=336
x=30%
width=70%
group=2
hide
.MMCallFunc - Example
:xmp.
/* This example queries the first object in  */
/* a CWMediaFolder, gets the linked file and */
/* queries the name of the artist from the   */
/* ID3 tag.                                  */

/* Get the first object of folder theFolder */
theObject=WPSWizCallWPSFunc("wpQueryContent", theFolder, 0, QC_FIRST)

/* Get the linked file system object from the shadow.
   cwGetFileSystemObject is a member of the
   CWMediaFolder class. */
theFileObj=MFldrCallFunc("cwGetFileSystemObject", theFolder, theObject)

/* Get the artist name. cwmmQueryTrackInfo is a member of the
   CWAudio class. Every object in the CWMediaFolder is usually
   a shadow to an object instance of a subclass of CWAudio */
theName=MMCallFunc("cwmmQueryTrackInfo", theFileObj, 2)

/* Display the name */
rc=RxMessageBox("The artist name : "||theName)
:exmp.


.im wpsmethods.inc

:h1 res=400
x=left y=bottom width=50%.Hidden features
:p.
The following features of the CW-multimedia classes may be enabled by setting values in :hp2.OS2.INI:ehp2..
:ul.
:li.:link reftype=hd res=410.Replace audio player:elink.
:li.:link reftype=hd res=411.Replace audio editor:elink.
:li.:link reftype=hd res=412.Replace AVI player:elink.
:li.:link reftype=hd res=413.Replace MPG player:elink.
:li.:link reftype=hd res=419.Replace MP3 player:elink.
:li.:link reftype=hd res=414.Replace image editor:elink.
:li.:link reftype=hd res=415.Enable details for images on CD:elink.
:li.:link reftype=hd res=416.Disable time display in icon titles:elink.
:li.:link reftype=hd res=417.Disable blinking of play controls (PLAY, PAUSE):elink.
:li.:link reftype=hd res=418.Disable creating of a shadow to the Media Player template:elink.
:li.:link reftype=hd res=420.Disable audio file checking:elink.
:li.:link reftype=hd res=421.Disable ID3 tag reading:elink.
:eul.

.im hiddenfeatures.inc

:h1 res=500.Extended attributes
:p.
Information about audio files is stored in the extended attributes of the file for use by other
programs or REXX. The following EAs are currently defined.

:table cols='20 35'.
:row.
:c.MMALBUM
:c.The album this track belongs to
:row.
:c.MMARTIST
:c.Name of the artist
:row.
:c.MMBPS
:c.Bits per sample
:row.
:c.MMCHANNELS
:c.Number of channels
:row.
:c.MMCOMMENT
:c.The comment for this file
:row.
:c.MMGENRE
:c.The genre of this audio file
:row.
:c.MMNUMAUDIOBYTES
:c.The number of audio bytes in this audio file
:row.
:c.MMPLAYTIME
:c.Playtime in seconds
:row.
:c.MMPLAYTIMEMS
:c.Playtime in milliseconds
:row.
:c.MMSAMPLERATE
:c.Samplerate
:row.
:c.MMTRACKNAME
:c.Name of the track
:row.
:c.MMYEAR
:c.Year
:etable.

:nt.
The information is stored in ASCII format.
:ent.
:euserdoc.








