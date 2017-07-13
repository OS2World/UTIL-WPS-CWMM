/* */
 call RxFuncAdd 'SysLoadFuncs', 'RexxUtil', 'SysLoadFuncs'
 call SysLoadFuncs

/* Delete INI keys */
rc=SysIni("USER", "CWMM", "ib",'DELETE:')
rc=SysIni("USER", "CWMM", "ab",'DELETE:')

/* Copy backup of MMPARTS.DLL to MMOS2\DLL */
curDir=DIRECTORY()
targetDir=FILESPEC('drive',curdir)||FILESPEC('path', curdir)


'@copy 'curDir||'\MMPARTS.DLL 'targetdir
'@del 'targetdir'MMPARTS.cwm'

'@'curdir||'\unlock 1>NUL 2>NUL'
'@'curdir||'\unlock 'targetdir'JPEGIO.DLL'
'@attrib -r -s 'targetdir'JPEGIO.DLL'
'@del 'targetdir'JPEGIO.DLL'
'@copy 'curDir||'\JPEGIO.DLL 'targetdir

call RxFuncAdd 'RxWinReplaceObjectClass', 'mmparts', 'RxWinReplaceObjectClass'

deinstalling=""

MMClasses.   = ''
MMClasses.1  = 'MMDataFile'
MMClasses.2  = 'MMImage'
MMClasses.3  = 'MMAudio'
MMClasses.4  = 'MMVideo'
MMClasses.5  = 'MMMIDI'
MMClasses.6  = 'MMTarga'
MMClasses.7  = 'MMTIFF'
MMClasses.8  = 'MMGIF'
MMClasses.9  = 'MMPCX'
MMClasses.10 = 'MMPhotoCD'
MMClasses.11 = 'MMAVC'
MMClasses.12 = 'MMMMotion'
MMClasses.13 = 'MMDIB'
MMClasses.14 = 'MMDevice'
MMClasses.15 = 'MMCDDevice'
MMClasses.16 = 'MMLVDDevice'
MMClasses.17 = 'MMVolume'
MMClasses.18 = 'MMVOC'
MMClasses.19 = 'MM_AU'
MMClasses.20 = 'MMAU'
MMClasses.21 = 'MMAIF'
MMClasses.22 = 'MMIFF'
MMClasses.23 = 'MMSNDF'
MMClasses.24 = 'MMAVI'
MMClasses.25 = 'MMFLI'
MMClasses.26 = 'MMMPG'
MMClasses.27 = 'MMJPG'
MMClasses.28 = 'MMWAV'
MMClasses.29 = 'MMMOV'
MMClasses.30 = 'MMFolder'
MMClasses.31 = 'MMFinder'

x = 1;

do while MMClasses.x <> ''   /* loop until no more classes */
   if deinstalling = '-R' then
      if SysDeregisterObjectClass(MMClasses.x) then
         say MMClasses.x 'class deregistration successful'
      else
         say MMClasses.x 'class deregistration failed'
   else
      if SysRegisterObjectClass(MMClasses.x, "mmparts") then
         say MMClasses.x 'class registration successful'
      else
         say MMClasses.x 'class registration failed'
   x = x + 1
end

/* Register MMShadow class */
      IF SysRegisterObjectClass("MMShadow", "MMSHD") then DO
	say 'MMShadow class registration successful'
	/* Do replacement */
	res = SysIni(, 'PM_InstallClassReplacement', 'WPShadow', 'MMShadow'x2c('0') )
     END
/* Do MMBitmap Replacement Class */

if deinstalling = '-R' then
   do
      if RxWinReplaceObjectClass('WPBitmap','MMBitmap', '0') then
         say 'UnReplace MMBitmap successful'
      else
         say 'UnReplace MMBitmap failed'

      if SysDeregisterObjectClass('MMBitmap') then
         say 'MMBitmap class deregistration successful'
      else
         say 'MMBitmap class deregistration failed'

   end
else
   do
      if SysRegisterObjectClass('MMBitmap', 'MMBIT') then
         say 'MMBitmap class registration successful'
      else
         say 'MMBitmap class registration failed'

      res = SysIni(, 'PM_InstallClassReplacement', 'WPBitmap', 'MMBitmap'x2c('0') )
      say 'Replacing WPBitmap with MMBitmap successful (res=' res ')'
   end

/* Do MMShadow Replacement Class */

if deinstalling = '-R' then
   do
      if RxWinReplaceObjectClass('WPShadow','MMShadow', '0') then
         say 'UnReplace MMShadow successful'
      else
         say 'UnReplace MMShadow failed'

      if SysDeregisterObjectClass('MMShadow') then
         say 'MMShadow class deregistration successful'
      else
         say 'MMShadow class deregistration failed'

   end
else
   do
      if SysRegisterObjectClass('MMShadow', 'MMSHD') then
         say 'MMShadow class registration successful'
      else
         say 'MMShadow class registration failed'

      res = SysIni(, 'PM_InstallClassReplacement', 'WPShadow', 'MMShadow'x2c('0') )
      say 'Replacing WPShadow with MMShadow successful (res=' res ')'
   end


MMIpl.=''
MMIpl.1 = 'MMAU'
MMIpl.2 = 'MMAIF'
MMIpl.3  = 'MMAudio'
MMIpl.4 = 'MMAVC'
MMIpl.5 = 'MMAVI'
MMIpl.6 = 'MMDIB'
MMIpl.7 = 'MMFLI'
MMIpl.8  = 'MMGIF'
MMIpl.9 = 'MMIFF'
MMIpl.10 = 'MMJPG'
MMIpl.11  = 'MMMIDI'
MMIpl.12 = 'MMMMotion'
MMIpl.13 = 'MMMOV'
MMIpl.14 = 'MMMPG'
MMIpl.15  = 'MMPCX'
MMIpl.16 = 'MMPhotoCD'
MMIpl.17 = 'MMSNDF'
MMIpl.18  = 'MMTarga'
MMIpl.19  = 'MMTIFF'
MMIpl.20  = 'MMVideo'
MMIpl.21 = 'MMVOC'
MMIpl.22 = 'MMWAV'
MMIpl.23 = 'MM_AU'

MMIpl.0=23

DO a=1 to MMIpl.0
	rc= SysIni('USER', 'PM_Workplace:IplLoad', MMIpl.a , X2C(2000))
END


/* Find MMOS2 directory */
thePath=VALUE("MMBASE", , "OS2ENVIRONMENT")
	thePath=STRIP(thePath)
	IF LENGTH(thePATH)\=0 THEN DO
		if RIGHT(thePath, 1)=";" THEN thePath=LEFT(thePAth, LENGTH(thePath)-1)
	END
	IF LENGTH(thePATH)\=0 THEN DO
		if RIGHT(thePath, 1)="\" THEN thePath=LEFT(thePAth, LENGTH(thePath)-1)
	END

/* Correct settings of some MM objects */
/* Digital Audio Player */
rc=SysSetObjectData("<MMPM_DAPLAYER1>", "EXENAME=WEPM.EXE;STARTUPDIR="||thePath||"\SOUNDS;")
rc=SysSetObjectData("<MMPM_DAPLAYER2>", "EXENAME=WEPM.EXE;STARTUPDIR="||thePath||"\SOUNDS;")

/* Midi Player */
rc=SysSetObjectData("<MMPM_MIDIPLAYER1>", "EXENAME=MPPM.EXE;STARTUPDIR="||thePath||"\SOUNDS;")
rc=SysSetObjectData("<MMPM_MIDIPLAYER2>", "EXENAME=MPPM.EXE;STARTUPDIR="||thePath||"\SOUNDS;")

/* Video Player */
rc=SysSetObjectData("<MMPM2_SOFTWARE_MOTION_VIDEO1>", "EXENAME=MPPM.EXE;STARTUPDIR="||thePath||"\MOVIES;")
rc=SysSetObjectData("<MMPM2_SOFTWARE_MOTION_VIDEO2>", "EXENAME=MPPM.EXE;STARTUPDIR="||thePath||"\MOVIES;")

/* CD Player */
rc=SysSetObjectData("<MMPM_CDPLAYER1>", "EXENAME=CDPM.EXE;STARTUPDIR="||thePath||"\SOUNDS;")
rc=SysSetObjectData("<MMPM_CDPLAYER2>", "EXENAME=CDPM.EXE;STARTUPDIR="||thePath||"\SOUNDS;")
rc=SysSetObjectData("<MMPM_CDPLAYER3>", "EXENAME=CDPM.EXE;STARTUPDIR="||thePath||"\SOUNDS;")



exit
