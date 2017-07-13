/* rexx -  register the CWMailFile class */

call RxFuncAdd 'SysLoadFuncs', 'RexxUtil', 'SysLoadFuncs'
call SysLoadFuncs

IF ARG(1)='' THEN DO
SAY ''
SAY 'Installation:    Register.cmd i'
SAY 'Deinstallation:  Register.cms u'
'@PAUSE'
EXIT
END


classDLL=DIRECTORY()||'\cwmm.dll'

IF ARG(1)='i' THEN DO
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


'@PAUSE'
EXIT
END


rc=SysDeRegisterObjectClass('CWMP3')
IF rc=0 THEN DO
	SAY 'Error while deregistering the MP3-class.'
END
ELSE
	SAY 'MP3-class deregistered.'

rc=SysDeRegisterObjectClass('CWOGG')
IF rc=0 THEN DO
	SAY 'Error while deregistering the Ogg-class.'
END
ELSE
	SAY 'Ogg-class deregistered.'

'repclass MMAudio CWAudio u' 
rc=SysDeRegisterObjectClass('CWAudio')
IF rc=0 THEN DO
	SAY 'Error while deregistering the CWAudio replacement class.'
END
'@PAUSE'
Exit