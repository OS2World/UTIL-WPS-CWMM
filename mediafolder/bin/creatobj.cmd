/* This script creates the CD player objects in the
   multimedia folder.

   This script is part of the media folder package.

   (c) Chris Wohlgemuth 2002-2004

   http://www.os2world.com/cdwriting
   http://www.geocities.com/SiliconValley/Sector/5785/

 */

 call RxFuncAdd 'SysLoadFuncs', 'RexxUtil', 'SysLoadFuncs'
 call SysLoadFuncs

PARSE source . . thePath
rc=SysSleep(2)

ADDRESS CMD FILESPEC('drive', thePath)||FILESPEC('path', thePath)'createcd.exe'


/* öberprÅfung ob WPS-Wizard installiert ist und ErgÑnzung
    fehlender Strings.
 */

ret=SysQueryClassList('classes.')
found=0
DO cls=1 to classes.0
	theclass=WORD(classes.cls, 1)
	thePath=SUBWORD(classes.cls, 2);
	IF TRANSLATE(theClass)='CWMENUFOLDER' THEN DO;
		found=1	;
		LEAVE;
	END;
END;
IF found=1 THEN DO
	iniPath=FILESPEC( 'drive', thePath)||FILESPEC( 'path', thePath)||'help\OBJHINTS.INI'
	if SysIni(iniPath, "nls", "convert")="ERROR:" THEN
		SysIni(iniPath, "nls", "convert", "Convert...")
	if SysIni(iniPath, "nls", "mvolume")="ERROR:" THEN
		SysIni(iniPath, "nls", "mvolume", "Master volume...")

END

exit


