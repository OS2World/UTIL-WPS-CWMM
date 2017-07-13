/* rexx -  Uninstall the CW-Audio classes */

deinstalling = ''

call RxFuncAdd 'SysLoadFuncs', 'RexxUtil', 'SysLoadFuncs'
call SysLoadFuncs

call SysCls
SAY ''
SAY 'This skript will remove the CWMM classes.'
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



rc=SysDeRegisterObjectClass('CWMP3')
IF rc=0 THEN DO
	SAY 'Error while deregistering the MP3-class.'
END
ELSE
	SAY 'MP3-class deregistered.'

rc=SysDeRegisterObjectClass('CWOGG')
IF rc=0 THEN DO
	SAY 'Error while deregistering the Ogg-class.'
END
ELSE
	SAY 'Ogg-class deregistered.'

rc=SysDeRegisterObjectClass('CWWAV')
IF rc=0 THEN DO
	SAY 'Error while deregistering the WAV-class.'
END
ELSE
	SAY 'WAV-class deregistered.'

rc=SysDeRegisterObjectClass('CWAIF')
IF rc=0 THEN DO
	SAY 'Error while deregistering the AIF-class.'
END
ELSE
	SAY 'AIF-class deregistered.'

rc=SysDeRegisterObjectClass('CWAU')
IF rc=0 THEN DO
	SAY 'Error while deregistering the AU-class.'
END
ELSE
	SAY 'AU-class deregistered.'

rc=SysDeRegisterObjectClass('CW_AU')
IF rc=0 THEN DO
	SAY 'Error while deregistering the _AU-class.'
END
ELSE
	SAY '_AU-class deregistered.'

rc=SysDeRegisterObjectClass('CWIFF')
IF rc=0 THEN DO
	SAY 'Error while deregistering the IFF-class.'
END
ELSE
	SAY 'IFF-class deregistered.'

rc=SysDeRegisterObjectClass('CWSNDF')
IF rc=0 THEN DO
	SAY 'Error while deregistering the SNDF-class.'
END
ELSE
	SAY 'SNDF-class deregistered.'

rc=SysDeRegisterObjectClass('CWVOC')
IF rc=0 THEN DO
	SAY 'Error while deregistering the VOC-class.'
END
ELSE
	SAY 'VOC-class deregistered.'

rc=SysDeRegisterObjectClass('CWCDTrack')
IF rc=0 THEN DO
	SAY 'Error while deregistering the CDTrack-class.'
END
ELSE
	SAY 'CDTrack-class deregistered.'

rc=SysDeRegisterObjectClass('CWMIDI')
IF rc=0 THEN DO
	SAY 'Error while deregistering the MIDI-class.'
END
ELSE
	SAY 'MIDI-class deregistered.'

rc=SysDeRegisterObjectClass('CWAudio')
IF rc=0 THEN DO
	SAY 'Error while deregistering the CWAudio class.'
END


rc=SysDeRegisterObjectClass('CWAVI')
IF rc=0 THEN DO
	SAY 'Error while deregistering the CWAVI class.'
END

rc=SysDeRegisterObjectClass('CWFLI')
IF rc=0 THEN DO
	SAY 'Error while deregistering the CWFLI class.'
END

rc=SysDeRegisterObjectClass('CWMOV')
IF rc=0 THEN DO
	SAY 'Error while deregistering the CWMOV class.'
END

rc=SysDeRegisterObjectClass('CWMPG')
IF rc=0 THEN DO
	SAY 'Error while deregistering the CWMPG class.'
END

rc=SysDeRegisterObjectClass('CWVideo')
IF rc=0 THEN DO
	SAY 'Error while deregistering the CWVideo class.'
END

rc=SysDeRegisterObjectClass('CWAVC')
IF rc=0 THEN DO
	SAY 'Error while deregistering the CWAVC class.'
END

rc=SysDeRegisterObjectClass('CWDIB')
IF rc=0 THEN DO
	SAY 'Error while deregistering the CWDIB class.'
END

rc=SysDeRegisterObjectClass('CWGIF')
IF rc=0 THEN DO
	SAY 'Error while deregistering the CWGIF class.'
END

rc=SysDeRegisterObjectClass('CWJPG')
IF rc=0 THEN DO
	SAY 'Error while deregistering the CWJPG class.'
END

rc=SysDeRegisterObjectClass('CWPhotoCD')
IF rc=0 THEN DO
	SAY 'Error while deregistering the CWPhotoCD class.'
END

rc=SysDeRegisterObjectClass('CWPCX')
IF rc=0 THEN DO
	SAY 'Error while deregistering the CWPCX class.'
END

rc=SysDeRegisterObjectClass('CWTarga')
IF rc=0 THEN DO
	SAY 'Error while deregistering the CWTarga class.'
END

rc=SysDeRegisterObjectClass('CWTIFF')
IF rc=0 THEN DO
	SAY 'Error while deregistering the CWTIFF class.'
END

rc=SysDeRegisterObjectClass('CWMMotion')
IF rc=0 THEN DO
	SAY 'Error while deregistering the CWMMotion class.'
END

rc=SysDeRegisterObjectClass('CWImage')
IF rc=0 THEN DO
	SAY 'Error while deregistering the CWImage class.'
END

/* Do CWBitmap Replacement Class */
     if RxWinReplaceObjectClass('WPBitmap','CWBitmap', '0') then
         say 'UnReplace CWBitmap successful'
      else
         say 'UnReplace CWBitmap failed'

      if SysDeregisterObjectClass('CWBitmap') then
         say 'CWBitmap class deregistration successful'
      else
         say 'CWBitmap class deregistration failed'

rc=SysDeRegisterObjectClass('CWMMDataFile')
IF rc=0 THEN DO
	SAY 'Error while deregistering the CWMMDataFile class.'
END

rc=SysDeRegisterObjectClass('MMFolder')
IF rc=0 THEN DO
	SAY 'Error while deregistering the MMFolder class.'
END

/* Do CWMMDisk Replacement Class */
     if RxWinReplaceObjectClass('WPDisk','CWMMDisk', '0') then
         say 'UnReplace CWMMDisk successful'
      else
         say 'UnReplace CWMMDisk failed'

      if SysDeregisterObjectClass('CWMMDisk') then
         say 'CWMMDisk class deregistration successful'
      else
         say 'CWMMDisk class deregistration failed'

/* Removing INI entries */
rc=SysIni('USER','CWMM','audioconverter', 'DELETE:')
rc=SysIni('USER','CWMM','imageconverter', 'DELETE:')

rc=SysDestroyObject('<MMPM2_MMVOLUME>')

/* Register the standard MM classes */

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


SAY ''
SAY 'You MUST restart the WPS now!'
SAY ''

'@PAUSE'
Exit
