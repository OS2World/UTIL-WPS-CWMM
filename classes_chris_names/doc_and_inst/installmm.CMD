/* rexx - Install the CW-MM classes */


deinstalling = '-R'

call RxFuncAdd 'SysLoadFuncs', 'RexxUtil', 'SysLoadFuncs'
call SysLoadFuncs


curDir=DIRECTORY()


call SysCls
SAY ''
SAY 'This skript will install the CW-MM classes.'
SAY 'Do you want to continue?'
SAY ''
SAY 'Enter Y for yes and N for no.'
SAY ''
SAY '> '
parse value SysCurPos() with aktzeile aktspalte
newposition=SysCurPos(aktzeile-1,2)

resp=LINEIN()
IF TRANSLATE(resp)='N' THEN EXIT

/* Load all the Rexx Util functions */
call RxFuncAdd 'SysLoadFuncs', 'RexxUtil', 'SysLoadFuncs'
call SysLoadFuncs
call RxFuncAdd 'RxWinReplaceObjectClass', 'mmparts', 'RxWinReplaceObjectClass'

/* First uninstall the standard MM-stuff */
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


/* Now install the new MM classes */
classDLL=DIRECTORY()||'\cwmm.dll'

SAY 'Registering CWMMDataFile-Class...'
rc=SysRegisterObjectClass('CWMMDataFile', classDLL)
IF rc=0 THEN DO
	SAY 'Error while registering the class.'
	'@PAUSE'
	Exit
END

SAY 'Registering CWAudio-Class...'
rc=SysRegisterObjectClass('CWAudio', classDLL)
IF rc=0 THEN DO
	SAY 'Error while registering the class.'
	'@PAUSE'
	Exit
END

SAY 'Registering MP3-Class...'
rc=SysRegisterObjectClass('CWMP3', classDLL)
IF rc=0 THEN DO
	SAY 'Error while registering the class.'
END


SAY 'Registering Ogg-Class...'
rc=SysRegisterObjectClass('CWOGG', classDLL)
IF rc=0 THEN DO
	SAY 'Error while registering the class.'
END

SAY 'Registering WAV-Class...'
rc=SysRegisterObjectClass('CWWAV', classDLL)
IF rc=0 THEN DO
	SAY 'Error while registering the class.'
END

SAY 'Registering AIF-Class...'
rc=SysRegisterObjectClass('CWAIF', classDLL)
IF rc=0 THEN DO
	SAY 'Error while registering the class.'
END

SAY 'Registering AU-Class...'
rc=SysRegisterObjectClass('CWAU', classDLL)
IF rc=0 THEN DO
	SAY 'Error while registering the class.'
END

SAY 'Registering _AU-Class...'
rc=SysRegisterObjectClass('CW_AU', classDLL)
IF rc=0 THEN DO
	SAY 'Error while registering the class.'
END

SAY 'Registering IFF-Class...'
rc=SysRegisterObjectClass('CWIFF', classDLL)
IF rc=0 THEN DO
	SAY 'Error while registering the class.'
END

SAY 'Registering SNDF-Class...'
rc=SysRegisterObjectClass('CWSNDF', classDLL)
IF rc=0 THEN DO
	SAY 'Error while registering the class.'
END

SAY 'Registering VOC-Class...'
rc=SysRegisterObjectClass('CWVOC', classDLL)
IF rc=0 THEN DO
	SAY 'Error while registering the class.'
END

SAY 'Registering CDTrack-Class...'
rc=SysRegisterObjectClass('CWCDTrack', classDLL)
IF rc=0 THEN DO
	SAY 'Error while registering the class.'
END

SAY 'Registering MIDI-Class...'
rc=SysRegisterObjectClass('CWMIDI', classDLL)
IF rc=0 THEN DO
	SAY 'Error while registering the class.'
END

SAY 'Registering CWVideo-Class...'
rc=SysRegisterObjectClass('CWVideo', classDLL)
IF rc=0 THEN DO
	SAY 'Error while registering the class.'
END

SAY 'Registering CWAVI-Class...'
rc=SysRegisterObjectClass('CWAVI', classDLL)
IF rc=0 THEN DO
	SAY 'Error while registering the class.'
END

SAY 'Registering CWFLI-Class...'
rc=SysRegisterObjectClass('CWFLI', classDLL)
IF rc=0 THEN DO
	SAY 'Error while registering the class.'
END

SAY 'Registering CWMOV-Class...'
rc=SysRegisterObjectClass('CWMOV', classDLL)
IF rc=0 THEN DO
	SAY 'Error while registering the class.'
END

SAY 'Registering CWMPG-Class...'
rc=SysRegisterObjectClass('CWMPG', classDLL)
IF rc=0 THEN DO
	SAY 'Error while registering the class.'
END

rc=SysRegisterObjectClass('CWImage', classDLL)
SAY 'Registering CWImage base class...'
IF rc=0 THEN DO
	SAY 'Error while registering the class.'
	'@PAUSE'
	Exit
END

SAY 'Registering AVC-Class...'
rc=SysRegisterObjectClass('CWAVC', classDLL)
IF rc=0 THEN DO
	SAY 'Error while registering the class.'
END


SAY 'Registering DIB-Class...'
rc=SysRegisterObjectClass('CWDIB', classDLL)
IF rc=0 THEN DO
	SAY 'Error while registering the class.'
END

SAY 'Registering GIF-Class...'
rc=SysRegisterObjectClass('CWGIF', classDLL)
IF rc=0 THEN DO
	SAY 'Error while registering the class.'
END

SAY 'Registering JPG-Class...'
rc=SysRegisterObjectClass('CWJPG', classDLL)
IF rc=0 THEN DO
	SAY 'Error while registering the class.'
END

SAY 'Registering PhotoCD-Class...'
rc=SysRegisterObjectClass('CWPhotoCD', classDLL)
IF rc=0 THEN DO
	SAY 'Error while registering the class.'
END

SAY 'Registering PCX-Class...'
rc=SysRegisterObjectClass('CWPCX', classDLL)
IF rc=0 THEN DO
	SAY 'Error while registering the class.'
END

SAY 'Registering Targa-Class...'
rc=SysRegisterObjectClass('CWTarga', classDLL)
IF rc=0 THEN DO
	SAY 'Error while registering the class.'
END

SAY 'Registering TIFF-Class...'
rc=SysRegisterObjectClass('CWTIFF', classDLL)
IF rc=0 THEN DO
	SAY 'Error while registering the class.'
END

SAY 'Registering MMotion-Class...'
rc=SysRegisterObjectClass('CWMMotion', classDLL)
IF rc=0 THEN DO
	SAY 'Error while registering the class.'
END

/* Do CWBitmap Replacement Class */

      if SysRegisterObjectClass('CWBitmap', classDLL) then
         say 'CWBitmap class registration successful'
      else
         say 'CWBitmap class registration failed'

      res = SysIni(, 'PM_InstallClassReplacement', 'WPBitmap', 'CWBitmap'x2c('0') )
      say 'Replacing WPBitmap with CWBitmap successful (res=' res ')'

/* Registering MMFolder */
SAY 'Registering MMFolder-Class...'
rc=SysRegisterObjectClass('MMFolder', classDLL)
IF rc=0 THEN DO
	SAY 'Error while registering the class.'
END

/* Do CWMMDisk Replacement Class */

      if SysRegisterObjectClass('CWMMDisk', classDLL) then
         say 'CWMMDisk class registration successful'
      else
         say 'CWMMDisk class registration failed'

      res = SysIni(, 'PM_InstallClassReplacement', 'WPDisk', 'CWMMDisk'x2c('0') )
      say 'Replacing WPBitmap with CWBitmap successful (res=' res ')'

/* Adding INI entries */
rc=SysIni('USER','CWMM','audioconverter', DIRECTORY()||'\bin\audconv.exe')
rc=SysIni('USER','CWMM','imageconverter',DIRECTORY()||'\bin\imgconv.exe')

/* Creating volume object */
rc=SysDestroyObject('<MMPM2_MMVOLUME>')
rc=SysCreateObject('WPProgram','Volume', "<MMPM2_FOLDER>","EXENAME="||DIRECTORY()||"\bin\volume.exe;OBJECTID=<MMPM2_MMVOLUME>;")

/* Create CD-Player object */
rc=SysCreateObject('WPProgram','CD-Player', "<MMPM2_FOLDER>","EXENAME=cdpm.exe;OBJECTID=<MMPM2_CDPLAYER>;")

SAY ''
SAY 'You MUST restart the WPS now!'
SAY ''
'@PAUSE'
EXIT
