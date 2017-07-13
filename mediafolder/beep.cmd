/* rexx -  Change the details and sort class
   of a folder to use MMAudio instead of WPFileSystem */

call RxFuncAdd 'SysLoadFuncs', 'RexxUtil', 'SysLoadFuncs'
call SysLoadFuncs


call beep 1000, 20
call beep 2000, 20
call beep 3000, 20

EXIT "ert ";


theString=ARG(1)||' !! '||ARG(2)||' !! '||ARG(3)||' !! '||ARG(4)

/*call RxMessageBox ARG(3)*/

SELECT

WHEN ARG(1)="/FILTERPOPUP" THEN DO
	PARSE VALUE ARG(3) WITH ulrc rest 
	EXIT (ulrc)
END
WHEN ARG(1)="/MODIFYPOPUP" THEN DO
	PARSE VALUE ARG(3) WITH ulrc rest 
	EXIT (ulrc)
END
WHEN ARG(1)="/MENUITEMSELECTED" THEN DO
	PARSE VALUE ARG(3) WITH hwndFrame ulMenuId 
	/* Return 1 if you handle this menu item */
	EXIT (0)
END

OTHERWISE
	return 0
END

/*call RxMessageBox theString*/
rc=12340
EXIT 1234

theFilepath=ARG(1)
theFolderPath=ARG(2)

a=0
DO WHILE lines(theFilePath)
	theLine=STRIP(LINEIN(theFilePath))
	if checkTheLine()=1 THEN DO
		a=a+1
		theTracks.a._name=theLine
	END
END
theTracks.0=a
call stream thefilepath, 'C','close'

DO a=theTracks.0 to 1 by -1
	rc=SysCreateObject(CWAudioShadow, " ", theFolderPath,"SHADOWID="theTracks.a._name)
END

rc=0
EXIT


checkTheLine:

if LENGTH(theLine)=0 then 
	return 0; /* Empty line */

if SUBSTR(theLine, 1, 1)='#' then 
	return 0; /* Comment */

if SUBSTR(theLine, 2, 1)\=':' THEN DO
	/* relative path. Prepend path of M3U file */
	if SUBSTR(theLine, 1, 1)\='\' THEN Do
		theLine=FILESPEC('drive', theFilePath)||FILESPEC('path', theFilePath)||theLine
	END
	ELSE
		theLine=FILESPEC('drive', theFilePath)||FILESPEC('path', theFilePath)||SUBSTR(theLine,2)

END
return 1;

