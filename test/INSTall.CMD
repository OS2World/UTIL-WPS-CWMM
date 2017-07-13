/**************************************************************************/
/* instmmvw.cmd                                                           */
/*                                                                        */
/* This REXX command file will install the MMParts Heirarchy. Use -R as an */
/* argument to instmmvw to remove the MMView Heirarchy.                   */
/*                                                                        */
/* Example:                                                               */
/*    instmmvw               (installs)                                   */
/*    instmmvw -r            (removes)                                    */
/**************************************************************************/

/**** !!! Adapt this !!! ****/
bootDrv='d:'

theDLL=DIRECTORY()||"\cwmm-0_2_4-eng\cwmm.dll"
SAY theDLL

mfldrDLL=DIRECTORY()||"\CWMFldr-0_2_4-eng\bin\mediafld.dll"
SAY mfldrDLL

progDLL=DIRECTORY()||"\CWMFldr-0_2_4-eng\DLL\progfldr.dll"
SAY progDLL

targetDLL=bootdrv||"\os2\dll\progfldr.dll"
SAY targetDLL

EXIT

arg deinstalling

/* Load all the Rexx Util functions */
call RxFuncAdd 'SysLoadFuncs', 'RexxUtil', 'SysLoadFuncs'
call SysLoadFuncs
call RxFuncAdd 'RxWinReplaceObjectClass', 'mmparts', 'RxWinReplaceObjectClass'

if deinstalling = "-R" then
   say "Removing MM classes"
else DO
   say "Installing MM classes"

   /* uninstall IBM classes */
   call uninstallIBM
END



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
MMClasses.14 = 'MMCDTrack'
MMClasses.15 = 'MMAudioStream'
MMClasses.16 = 'MMVOC'
MMClasses.17 = 'MM_AU'
MMClasses.18 = 'MMAU'
MMClasses.19 = 'MMAIF'
MMClasses.20 = 'MMIFF'
MMClasses.21 = 'MMSNDF'
MMClasses.22 = 'MMAVI'
MMClasses.23 = 'MMFLI'
MMClasses.24 = 'MMMPG'
MMClasses.25 = 'MMJPG'
MMClasses.26 = 'MMWAV'
MMClasses.27 = 'MMMOV'
MMClasses.28 = 'MMFolder'
MMClasses.29 = 'MMM3U'
MMClasses.0=29
x = 1;

do while MMClasses.x <> ''   /* loop until no more classes */
   if deinstalling = '-R' then
      if SysDeregisterObjectClass(MMClasses.x) then
         say MMClasses.x 'class deregistration successful'
      else
         say MMClasses.x 'class deregistration failed'
   else
      if SysRegisterObjectClass(MMClasses.x, "D:\OS2\APPS\CWMMOS2\CWMM\cwmm.dll") then
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
      if SysRegisterObjectClass('MMBitmap', DIRECTORY()"\cwmm.dll") then
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


/* Now install mediafolder */
'copy 'progDLL' 'targetDLL


exit




uninstallIBM:

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


return