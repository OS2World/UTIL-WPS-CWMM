/* rexx -  Uninstall the CW-Audio classes */

call RxFuncAdd 'SysLoadFuncs', 'RexxUtil', 'SysLoadFuncs'
call SysLoadFuncs

call SysCls
SAY ''
SAY 'This skript will remove the CWAudio classes.'
SAY 'Do you want to continue?'
SAY ''
SAY 'Enter Y for yes and N for no.'
SAY ''
SAY '> '
parse value SysCurPos() with aktzeile aktspalte
newposition=SysCurPos(aktzeile-1,2)

resp=LINEIN()
IF TRANSLATE(resp)='N' THEN EXIT


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

rc=SysDeRegisterObjectClass('CWCDTrack')
IF rc=0 THEN DO
	SAY 'Error while deregistering the CDTrack-class.'
END
ELSE
	SAY 'CDTrack-class deregistered.'

'repclass MMAudio CWAudio u' 
rc=SysDeRegisterObjectClass('CWAudio')
IF rc=0 THEN DO
	SAY 'Error while deregistering the CWAudio replacement class.'
END
'@PAUSE'
Exit
