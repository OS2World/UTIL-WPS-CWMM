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


classDLL=DIRECTORY()||'\cwmm.dll'

SAY 'Degistering new multimedia classes...'

deinstalling = '-R'

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
MMClasses.14 = 'MMMP3'
MMClasses.15 = 'MMOGG'
MMClasses.16 = 'MMAudioStream'
MMClasses.17 = 'MMCDTrack'
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
MMClasses.30 = 'MMM3U'
MMClasses.31 = 'MMFolder'

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

deinstalling = ''

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