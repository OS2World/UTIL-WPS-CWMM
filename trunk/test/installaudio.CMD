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

SAY 'Registering MP3-Class...'
rc=SysRegisterObjectClass('CWMP3', classDLL)
IF rc=0 THEN DO
	SAY 'Error while registering the class.'
END


SAY 'Registering Ogg-Class...'
rc=SysRegisterObjectClass('CWOGG', classDLL)
IF rc=0 THEN DO
	SAY 'Error while registering the class.'
END

SAY 'Registering WAV-Class...'
rc=SysRegisterObjectClass('CWWAV', classDLL)
IF rc=0 THEN DO
	SAY 'Error while registering the class.'
END

SAY 'Registering AIF-Class...'
rc=SysRegisterObjectClass('CWAIF', classDLL)
IF rc=0 THEN DO
	SAY 'Error while registering the class.'
END

SAY 'Registering AU-Class...'
rc=SysRegisterObjectClass('CWAU', classDLL)
IF rc=0 THEN DO
	SAY 'Error while registering the class.'
END

SAY 'Registering _AU-Class...'
rc=SysRegisterObjectClass('CW_AU', classDLL)
IF rc=0 THEN DO
	SAY 'Error while registering the class.'
END

SAY 'Registering IFF-Class...'
rc=SysRegisterObjectClass('CWIFF', classDLL)
IF rc=0 THEN DO
	SAY 'Error while registering the class.'
END

SAY 'Registering SNDF-Class...'
rc=SysRegisterObjectClass('CWSNDF', classDLL)
IF rc=0 THEN DO
	SAY 'Error while registering the class.'
END

SAY 'Registering VOC-Class...'
rc=SysRegisterObjectClass('CWVOC', classDLL)
IF rc=0 THEN DO
	SAY 'Error while registering the class.'
END

SAY 'Registering CDTrack-Class...'
rc=SysRegisterObjectClass('CWCDTrack', classDLL)
IF rc=0 THEN DO
	SAY 'Error while registering the class.'
END

SAY 'Registering MIDI-Class...'
rc=SysRegisterObjectClass('CWMIDI', classDLL)
IF rc=0 THEN DO
	SAY 'Error while registering the class.'
END

'@PAUSE'
EXIT
