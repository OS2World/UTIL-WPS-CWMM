/* rexx -  Change the details and sort class
   of a folder to use MMAudio instead of WPFileSystem

   This script is part of the media folder package.

   (c) Chris Wohlgemuth 2002-2003

   http://www.os2world.com/cdwriting
   http://www.geocities.com/SiliconValley/Sector/5785/

 */

call RxFuncAdd 'SysLoadFuncs', 'RexxUtil', 'SysLoadFuncs'
call SysLoadFuncs

theFolder=ARG(1)
theFolder=STRIP(TRANSLATE(theFolder,' ','"'))
SAY theFolder

rc=SysSetObjectData(theFolder,'DETAILSCLASS=MMAudio;SORTCLASS=MMAudio')
IF rc=0 THEN DO
	SAY 'Cannot set the details and sort class.'
	'@PAUSE'
	EXIT
END

SAY 'Detailsclass and sort class for folder changed.'
'@PAUSE'
EXIT

