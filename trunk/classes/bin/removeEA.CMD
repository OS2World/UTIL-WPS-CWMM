/*
	rexx -  Remove  .CLASSINFO EA and set .TYPE EA
*/

call RxFuncAdd 'SysLoadFuncs', 'RexxUtil', 'SysLoadFuncs'
call SysLoadFuncs

theFolder=ARG(1)


IF theFolder="" THEN DO
call usage
'PAUSE'
EXIT
END

SAY
SAY
call usage2
SAY
SAY 'Type Control-C to abort.'
'PAUSE'

rc=SySFileTree(theFolder'\*.mp3', 'mp3s', 'FSO')

theType='DFFF00000100FDFF'x||'0300'x||"MP3"
DO a=1 to mp3s.0
	/* Remove EA */
	rc=SysPutEA(mp3s.a, ".CLASSINFO", '')
	rc=SysPutEA(mp3s.a, ".TYPE", theType)
END

rc=SySFileTree(theFolder'\*.wav', 'mp3s', 'FSO')
theType='DFFF00000100FDFF'x||'0300'x||"WAV"
DO a=1 to mp3s.0
	/* Remove EA */
	rc=SysPutEA(mp3s.a, ".CLASSINFO", '')
	rc=SysPutEA(mp3s.a, ".TYPE", theType)
END

rc=SySFileTree(theFolder'\*.ogg', 'mp3s', 'FSO')
theType='DFFF00000100FDFF'x||'0300'x||"OGG"
DO a=1 to mp3s.0
	/* Remove EA */
	rc=SysPutEA(mp3s.a, ".CLASSINFO", '')
	rc=SysPutEA(mp3s.a, ".TYPE", theType)
END

SAY 'Done.'

EXIT


usage:
parse SOURCE . . theName
SAY
SAY
SAY 'Usage:'
SAY theName' <folder>'
return

usage2:
SAY 'This skript removes the .CLASSINFO extended attribute from'
SAY 'the files inside the folder and all subfolders.'
SAY ''
SAY '[31;1;m****************************************[0;m'
SAY ''
SAY '  Track names of OGG and Wave files will be lost!'
SAY ''
SAY '[31;1;m****************************************[0;m'

RETURN
