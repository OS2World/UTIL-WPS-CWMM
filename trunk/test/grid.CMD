/* rexx -  Change the details and sort class
   of a folder to use CWAudio instead of WPFileSystem */

call RxFuncAdd 'SysLoadFuncs', 'RexxUtil', 'SysLoadFuncs'
call SysLoadFuncs

theFolder=ARG(1)
theFolder=STRIP(TRANSLATE(theFolder,' ','"'))
SAY theFolder

rc=SysSetObjectData("D:\Arbeitsoberflaeche\Light table!1",'ICONVIEW='ARG(1))
IF rc=0 THEN DO
	SAY ''
	'@PAUSE'
	EXIT
END

SAY 'Done.'
'@PAUSE'
EXIT

