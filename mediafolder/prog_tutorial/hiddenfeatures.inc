:h2 res=410
x=50% width=50%
group=2
.Replace audio player
:p.
The default audio player coming with MMOS2 may be replaced by adding the
following key to OS2.INI:
:p.
:hp2.CWMM->audioplayer-><path to new player>:ehp2.
:nt.You don't have to provide the full path if the program is in your PATH. The program must be a PM program!
:ent.
:p.
Even if a new program isn't specified in the INI file the classes improve the default
behaviour by not starting the player as part of the WPS process. So if the player crashes
the WPS doesn't crash anymore . 



:h2 res=411
x=50% width=50%
group=2
.Replace audio editor
:p.
The default audio editor coming with MMOS2 may be replaced by adding the
following key to OS2.INI:
:p.
:hp2.CWMM->audioeditor-><path to new editor>:ehp2.
:nt.You don't have to provide the full path if the program is in your PATH. The program must be a PM program!
:ent.
:p.
Even if a new program isn't specified in the INI file the classes improve the default
behaviour by not starting the editor as part of the WPS process. So if the editor crashes
the WPS doesn't crash anymore. 


:h2 res=412
x=50% width=50%
group=2
.Replace AVI player
:p.
The default AVI player coming with MMOS2 may be replaced by adding the
following key to OS2.INI:
:p.
:hp2.CWMM->aviplayer-><path to new player>:ehp2.
:nt.You don't have to provide the full path if the program is in your PATH. The program must be a PM program!
:ent.
:p.
Even if a new program isn't specified in the INI file the classes improve the default
behaviour by not starting the player as part of the WPS process. So if the player crashes
the WPS doesn't crash anymore . 


:h2 res=413
x=50% width=50%
group=2
.Replace MPG player
:p.
The default MPG player coming with MMOS2 may be replaced by adding the
following key to OS2.INI:
:p.
:hp2.CWMM->mpgplayer-><path to new player>:ehp2.
:nt.You don't have to provide the full path if the program is in your PATH. The program must be a PM program!
:ent.
:p.
Even if a new program isn't specified in the INI file the classes improve the default
behaviour by not starting the player as part of the WPS process. So if the player crashes
the WPS doesn't crash anymore . 


:h2 res=419
x=50% width=50%
group=2
.Replace MP3 player
:p.
The default MP3 player may be replaced by adding the
following key to OS2.INI:
:p.
:hp2.CWMM->mp3player-><path to new player>:ehp2.
:nt.You don't have to provide the full path if the program is in your PATH.
:ent.
:p.
Even if a new program isn't specified in the INI file the classes improve the default
behaviour by not starting the player as part of the WPS process. So if the default player crashes
the WPS doesn't crash, too. 

:h2 res=422
x=50% width=50%
group=2
.Replace M3U editor
:p.
The default M3U editor may be replaced by adding the
following key to OS2.INI:
:p.
:hp2.CWMM->m3ueditor-><path to new editor>:ehp2.
:nt.You don't have to provide the full path if the program is in your PATH.
:ent.

:h2 res=423
x=50% width=50%
group=2
.Replace M3U olayer
:p.
The default M3U player may be replaced by adding the
following key to OS2.INI:
:p.
:hp2.CWMM->m3uplayer-><path to new player>:ehp2.
:nt.You don't have to provide the full path if the program is in your PATH.
:ent.
:p.
Default behaviour is to create a new MEdia folder and fill it with the tracks specified in the
M3U file. Using this setting it's possible to use another MP3 player. 



:h2 res=414
x=50% width=50%
group=2
.Replace image editor
:p.
The default image editor coming with MMOS2 may be replaced by adding the
following key to OS2.INI:
:p.
:hp2.CWMM->imageeditor-><path to new editor>:ehp2.
:nt.You don't have to provide the full path if the program is in your PATH. The program must be a PM program!
:ent.
:p.
Even if a new program isn't specified in the INI file the classes improve the default
behaviour by not starting the editor as part of the WPS process. So if the editor crashes
the WPS doesn't crash anymore. 



:h2 res=415
x=50% width=50%
group=2
.Enable details for images on CD
:p.
The default setting is not to support details for images on CD. The reason is that
with slow drives the gathering of the information needs a huge amount of time. You may
enable the new class features for images on CD by adding the following key to OS2.INI.
:p.
:hp2.CWMM->useNewImgClsFeaturesOnCD->1:ehp2.

:nt.Be sure to keep the uppercase characters!
:ent.
:nt.You have to restart the WPS so the setting takes effect.
:ent.


:h2 res=416
x=50% width=50%
group=2
.Disable time display in icon titles
:p.
When starting audio files with a doubleclick the elapsed time is displayed in the icon
title. To disable this new feature set the following key in OS2.INI.
:p.
:hp2.CWMM->mmClsNoExtendedIconTitles->1:ehp2.

:nt.Be sure to keep the uppercase characters!
:ent.
:nt.You have to restart the WPS so the setting takes effect.
:ent.

:h2 res=417
x=50% width=50%
group=2
.Disable play control blinking while playing
:p.
As a visual feedback play controls (PLAY, PAUSE) blink while a track is playing.
To disable this feature set the following key in OS2.INI.
:p.
:hp2.CWMM->bNoPlayCtrlBlinking->1:ehp2.

:nt.Be sure to keep the uppercase characters!
:ent.
:nt.You have to restart the WPS so the setting takes effect.
:ent.

:h2 res=418
x=50% width=50%
group=2
.Prevent creation of a shadow to the Media Player template
:p.
A shadow to the Media Player template in the Multimedia Templates Folder is automatically
created in the Multimedia Folder for easier access. By setting this key this feature is disabled.

:p.
:hp2.CWMM->bNoMediaFolderTemplateShadow->1:ehp2.

:nt.Be sure to keep the uppercase characters!
:ent.

:nt.You may move the created shadow to another place. It will only be recreated in the Multimedia Folder
if it's deleted from the system.
:ent.


:h2 res=420
x=50% width=50%
group=2
.Disable audio file checking
:p.
You may disable checking of audio files for certain drives by adding the drive letter to this
entry in OS2.INI.
:p.
:hp2.CWMM->mmClsDontCheckDrives-><drive list>:ehp2.
:nt.Be sure to keep the uppercase characters!
:ent.

This may be useful for network drives on slow connections or other slow drives. Normally
the gathered information is stored in the extended attributes of the audio file so the checking only
happens once. If a particular filesystem doesn't support EAs you may chose to add that drive to the list too,
to prevent the scanning.
:nt.
Be aware that no audio information will be shown by the WPS for audio objects located on a drive specified in
the list.
:ent.
:nt.
The WPS must be restarted after changing this setting.
:ent.

:p.
Example:
:xmp.
CWMM->mmClsDontCheckDrives->abcghzy
:exmp.

:nt.
This setting is mostly obsolete now because the MMIOMP3 IO procedure contained in
:hp2.MMAudio Pak 2:ehp2. (released 30.08.2003) no longer scans MP3 files but reads the Xing header
for VBR encoded files.
:ent.


:h2 res=421
x=50% width=50%
group=2
.Disable ID3 tag reading
:p.
You may disable reading of ID3 tags for certain drives by adding the drive letter to this
entry in OS2.INI.
:p.
:hp2.CWMM->mmClsDontReadID3OnDrives-><drive list>:ehp2.
:nt.Be sure to keep the uppercase characters!
:ent.

This may be useful for network drives on slow connections or other slow drives.

:nt.
Be aware that no ID3 tag information will be shown by the WPS for MP3 files located on a drive specified in
the list.
:ent.
:nt.
The WPS must be restarted after changing this setting.
:ent.

:p.
Example:
:xmp.
CWMM->mmClsDontReadID3OnDrives->abcghzy
:exmp.

