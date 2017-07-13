/* rexx - Install the CW-Audio classes */

call RxFuncAdd 'SysLoadFuncs', 'RexxUtil', 'SysLoadFuncs'
call SysLoadFuncs

curDir=DIRECTORY()


call SysCls
SAY ''
SAY 'This skript will install the CWAudio classes.'
SAY 'Do you want to continue?'
SAY ''
SAY 'Enter Y for yes and N for no.'
SAY ''
SAY '> '
parse value SysCurPos() with aktzeile aktspalte
newposition=SysCurPos(aktzeile-1,2)

resp=LINEIN()
IF TRANSLATE(resp)='N' THEN EXIT

classDLL=DIRECTORY()||'\cwmm.dll'

rc=SysRegisterObjectClass('CWAudio', classDLL)
IF rc=0 THEN DO
	SAY 'Error while registering the class.'
	'@PAUSE'
	Exit
END
'repclass MMAudio CWAudio'

SAY 'Registering MP3-Class...'
rc=SysRegisterObjectClass('CWMP3', classDLL)
IF rc=0 THEN DO
	SAY 'Error while registering the class.'
	SAY 'Only the icon will be wrong now.' 
END


SAY 'Registering Ogg-Class...'
rc=SysRegisterObjectClass('CWOGG', classDLL)
IF rc=0 THEN DO
	SAY 'Error while registering the class.'
	SAY 'Only the icon will be wrong now.' 
END

SAY 'Registering CDTrack-Class...'
rc=SysRegisterObjectClass('CWCDTrack', classDLL)
IF rc=0 THEN DO
	SAY 'Error while registering the class.'
END

'@PAUSE'
EXIT
