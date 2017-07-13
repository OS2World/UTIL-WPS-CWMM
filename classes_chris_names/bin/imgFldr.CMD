/* rexx -  Change the details and sort class
   of a folder to use CWImage instead of WPFileSystem */

call RxFuncAdd 'SysLoadFuncs', 'RexxUtil', 'SysLoadFuncs'
call SysLoadFuncs

theFolder=ARG(1)
theFolder=STRIP(TRANSLATE(theFolder,' ','"'))
SAY theFolder

rc=SysSetObjectData(theFolder,'DETAILSCLASS=CWImage;SORTCLASS=CWImage')
IF rc=0 THEN DO
	SAY 'Cannot set the details and sort class.'
	'@PAUSE'
	EXIT
END

SAY 'Detailsclass and sort class for folder changed.'
'@PAUSE'
EXIT

