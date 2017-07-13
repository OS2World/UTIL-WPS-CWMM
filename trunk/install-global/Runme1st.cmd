/* rexx - Install the CW-MM classes */

/* Load all the Rexx Util functions */
call RxFuncAdd 'SysLoadFuncs', 'RexxUtil', 'SysLoadFuncs'
call SysLoadFuncs

/*
SAY RxFuncAdd( 'RxWinReplaceObjectClass', 'MMPARTS', 'RxWinReplaceObjectClass')
SAY RxFuncQuery('RxWinReplaceObjectClass')
*/

'mode 80,50'

curDir=DIRECTORY()
/* call SysCls */
SAY ""
SAY ""

deinstalling = '-R' /* We want to deinstall the classes */


/* Get bootdrive */
 bootdrv = translate(value('PATH', , 'os2environment'))
 bootdrv2=bootdrv
 parse var bootdrv bootdrv':\OS2;'rest
 parse var bootdrv2 bootdrv2':\MMOS2;'rest
 bootdrive = substr(bootdrv, length(bootdrv))
 bootdrive2 = substr(bootdrv2, length(bootdrv2))
 IF TRANSLATE(bootdrive2)=TRANSLATE(bootdrive) THEN
	/* Get config.sys */
	configsys = bootdrive':\config.sys'
 ELSE DO
	call Beep 1000, 100
	SAY ""
	SAY "[31;1;mCannot get boot drive![0;m"
	SAY ""
	SAY "Please specify your boot drive"
	SAY ">"
	parse value SysCurPos() with aktzeile aktspalte
	newposition=SysCurPos(aktzeile-1,2)
	resp=STRIP(LINEIN())
	configsys = LEFT(resp,1)':\config.sys'
 END

/* Find MMOS2 directory */
thePath=VALUE("MMBASE", , "OS2ENVIRONMENT")
	thePath=STRIP(thePath)
	IF LENGTH(thePATH)\=0 THEN DO
		if RIGHT(thePath, 1)=";" THEN thePath=LEFT(thePAth, LENGTH(thePath)-1)
	END
	IF LENGTH(thePATH)\=0 THEN DO
		if RIGHT(thePath, 1)="\" THEN thePath=LEFT(thePAth, LENGTH(thePath)-1)
	END
	/* Get IBM class dll */
	mmDLL=thePath||"\DLL\MMPARTS.DLL"

if STREAM(mmDLL, 'c', 'query exists')=mmDLL THEN DO
	/* IBM class dll found */

	if STREAM(thePath'\DLL\CWMMBack\MMPARTS.DLL', 'c', 'query exists')="" THEN DO
		SAY ""
		SAY "  Creating backup of MMPARTS.DLL in "||thePath'\DLL\CWMMBack\'
		SAY ""
		'@md 'thePath'\DLL\CWMMBack 2>NUL 1>NUL'
		'@copy 'mmDLL' 'thePath'\DLL\CWMMBack 1>NUL'
		IF rc\=0 THEN DO
			SAY "[31;1;m"
			SAY "Error while copying MMPARTS.DLL![0;m"
			SAY "Continue anyway? (You should create a copy by hand!!!)"
			SAY 'Type Y <return> to continue N <return> to abort.'
			SAY ">"
			parse value SysCurPos() with aktzeile aktspalte
			newposition=SysCurPos(aktzeile-1,2)
			resp=LINEIN()
			IF TRANSLATE(resp)='N' THEN EXIT
		END
		'@copy '||thePath||'\ab.exe '||thePath||'\DLL\CWMMBack 1>NUL'
		'@copy '||thePath||'\ib.exe '||thePath||'\DLL\CWMMBack 1>NUL'
		'@copy '||thePath||'\MPPM.exe '||thePath||'\DLL\CWMMBack 1>NUL'
		'@copy '||thePath||'\CDPM.exe '||thePath||'\DLL\CWMMBack 1>NUL'
		'@copy '||thePath||'\WEPM.exe '||thePath||'\DLL\CWMMBack 1>NUL'
		/* Put the new path into the ini so the classes may find it. */
		rc=SysIni("USER", "CWMM", "ab", thePath||'\DLL\CWMMBack\ab.exe')
		rc=SysIni("USER", "CWMM", "ib", thePath||'\DLL\CWMMBack\ib.exe')
		SAY ""
		SAY "  Creating backup of JPEGIO.DLL in "||thePath'\DLL\CWMMBack\'
		'@copy 'thePAth'\DLL\JPEGIO.DLL 'thePath'\DLL\CWMMBack 1>NUL'
		IF rc\=0 THEN DO
			SAY "[31;1;m"
			SAY "Error while copying JPEGIO.DLL![0;m"
			SAY "Continue anyway?  (You should create a copy by hand!)"
			SAY 'Type Y <return> to continue N <return> to abort.'
			SAY ">"
			parse value SysCurPos() with aktzeile aktspalte
			newposition=SysCurPos(aktzeile-1,2)
			resp=LINEIN()
			IF TRANSLATE(resp)='N' THEN EXIT
		END
	END
	ELSE DO
		call beep 1000, 100
		SAY ""
		SAY "[31;1;mInformation:[0;m Backup files already saved in "||thePath'\DLL\CWMMBack\'
		SAY ""
		SAY "Uninstall any prior release of the MM classes. If you already did that rename"
		SAY "the directory 'CWMMBack' and restart the installation."
		SAY ""
		'@pause'
		Exit
	END
	'@'curDir||'\install\unlock.exe 1>NUL 2>NUL'
	'@'curDir||'\install\unlock.exe 'thePath'\DLL\MMPARTS.DLL 1>NUL 2>NUL'
	'@attrib -r -s -h 'thePath'\DLL\MMPARTS.DLL 1>NUL 2>NUL'
	'@ren 'thePath'\DLL\MMPARTS.DLL MMPARTS.cwm'
END

/*
if STREAM(mmDLL, 'c', 'query exists')=mmDLL THEN DO
	SAY ""
	SAY "Now patching the class names in the file MMPARTS.DLL."
	SAY "This DLL contains the IBM multimedia classes. This patching prevents"
	SAY "the loading of these old classes which may lead to serious problems."
	SAY "A backup of this DLL was created in the directory "||thePath'\DLL\CWMMBack'
	'@pause'
	/* Unlock the MMPARTS.DLL */
	unlck='@"'curDir||'\install\unlock.exe"'
	ADDRESS CMD unlck' 'mmDLL' 2>NUL 1>NUL'
	patchit='"'curDir||'\install\patchdll.exe"'
	ADDRESS CMD patchit' 'mmDLL' '
END
*/

SAY ""
SAY "  Copying  uninstall skript to "||thePath'\DLL\CWMMBack\'
'@copy 'curDir||'\install\instIBM.cmd 'thePath'\DLL\CWMMBack 1>NUL'
'@copy 'curDir||'\install\unlock.exe 'thePath'\DLL\CWMMBack 1>NUL'
SAY ""

SAY ''
SAY 'This skript will now uninstall the IBM-MM classes.'
SAY ''
SAY 'Type Y <return> to continue N <return> to abort.'
SAY ">"
parse value SysCurPos() with aktzeile aktspalte
newposition=SysCurPos(aktzeile-1,2)
resp=LINEIN()
IF TRANSLATE(resp)='N' THEN EXIT


/* Deregister the PracticeImage and PracticeBitmap class if present */
SAY ''
SAY 'Deregistering the PracticeBitmap and PracticeImage classes'
SAY 'from the primgfix.zip package if present.'

'@'curDir||'\install\repclass.exe MMBitmap PracticeBitmap u'
'@'curDir||'\install\repclass.exe MMImage PracticeImage u'

ret=SysDeregisterObjectClass('PracticeBitmap')
ret=SysDeregisterObjectClass('PracticeImage')

SAY ''
SAY 'Removing old MM objects'

MMObjects.=''
MMObjects.1='<MMPM2_MMWAVETEMPLATE>'
MMObjects.2='<MMPM2_MMMMOTIONTEMPLATE>'
MMObjects.3='<MMPM2_MMTIFFTEMPLATE>'
MMObjects.4='<MMPM2_MMTARGATEMPLATE>'
MMObjects.5='<MMPM2_MMPCXTEMPLATE>'
MMObjects.6='<MMPM2_MMMIDITEMPLATE>'
MMObjects.7='<MMPM2_MMGIFTEMPLATE>'
MMObjects.8='<MMPM2_MMDIBTEMPLATE>'
MMObjects.9='<MMPM2_MMBITMAPTEMPLATE>'
MMObjects.10='<MMPM2_MMAVITEMPLATE>'
MMObjects.11='<MMPM2_MMAVCIMAGETEMPLATE>'
MMObjects.12='<MMPM2_MMFOLDERTEMPLATE>'
MMObjects.13='<MMPM2_MMLVDDEVICETEMPLATE>'
MMObjects.14='<MMPM2_MMCDDEVICETEMPLATE>'

MMObjects.15='<MMPM2_MMVOLUME>'
MMObjects.16='<MMPM2_MMCDDEVICE01>'
MMObjects.17='<MMPM2_MMCDDEVICE02>'
MMObjects.18='<MMPM2_MMCDDEVICE03>'
MMObjects.19='<MMPM2_MMCDDEVICE04>'
MMObjects.20='<MMPM2_AUDIOFINDER>'
MMObjects.21='<MMPM2_MIDIFINDER>'
MMObjects.22='<MMPM2_IMAGEFINDER>'
MMObjects.23='<MMPM2_VIDEOFINDER>'
MMObjects.24='<MMPM2_SNDSHAD>'
MMObjects.25='<MMPM2_MMMMotion ImageTEMPLATE>'
MMObjects.26='<MMPM2_MMTargaTEMPLATE>'
MMObjects.27='<MMPM2_MMBitmapTEMPLATE>'
MMObjects.28='<MMPM2_MMBilddateiTEMPLATE>'

MMObjects.0=28

DO a=1 to MMObjects.0
	rc=SysSetObjectData(MMObjects.a, "NODELETE=NO;TEMPLATE=NO;")
END
DO a=1 to MMObjects.0
	rc=SysDestroyObject(MMObjects.a)
END


SAY ''
SAY 'Deregistering the IBM multimedia classes if present.'

/* Remove autoload entries */
MMIpl.=''
MMIpl.1 = 'MMAU'
MMIpl.2 = 'MMAIF'
MMIpl.3  = 'MMAudio'
MMIpl.4 = 'MMAVC'
MMIpl.5 = 'MMAVI'
MMIpl.6 = 'MMDIB'
MMIpl.7 = 'MMFLI'
MMIpl.8  = 'MMGIF'
MMIpl.9 = 'MMIFF'
MMIpl.10 = 'MMJPG'
MMIpl.11  = 'MMMIDI'
MMIpl.12 = 'MMMMotion'
MMIpl.13 = 'MMMOV'
MMIpl.14 = 'MMMPG'
MMIpl.15  = 'MMPCX'
MMIpl.16 = 'MMPhotoCD'
MMIpl.17 = 'MMSNDF'
MMIpl.18  = 'MMTarga'
MMIpl.19  = 'MMTIFF'
MMIpl.20  = 'MMVideo'
MMIpl.21 = 'MMVOC'
MMIpl.22 = 'MMWAV'
MMIpl.23 = 'MM_AU'

MMIpl.0=23

DO a=1 to MMIpl.0
	    call SysIni 'USER', 'PM_Workplace:IplLoad', MMIpl.a, 'DELETE:'
END

/* First uninstall the standard MM-stuff */
MMClasses.   = ''
MMClasses.1  = 'MMDataFile'
MMClasses.2  = 'MMImage'
MMClasses.3  = 'MMAudio'
MMClasses.4  = 'MMVideo'
MMClasses.5  = 'MMMIDI'
MMClasses.6  = 'MMTarga'
MMClasses.7  = 'MMTIFF'
MMClasses.8  = 'MMGIF'
MMClasses.9  = 'MMPCX'
MMClasses.10 = 'MMPhotoCD'
MMClasses.11 = 'MMAVC'
MMClasses.12 = 'MMMMotion'
MMClasses.13 = 'MMDIB'
MMClasses.14 = 'MMDevice'
MMClasses.15 = 'MMCDDevice'
MMClasses.16 = 'MMLVDDevice'
MMClasses.17 = 'MMVolume'
MMClasses.18 = 'MMVOC'
MMClasses.19 = 'MM_AU'
MMClasses.20 = 'MMAU'
MMClasses.21 = 'MMAIF'
MMClasses.22 = 'MMIFF'
MMClasses.23 = 'MMSNDF'
MMClasses.24 = 'MMAVI'
MMClasses.25 = 'MMFLI'
MMClasses.26 = 'MMMPG'
MMClasses.27 = 'MMJPG'
MMClasses.28 = 'MMWAV'
MMClasses.29 = 'MMMOV'
MMClasses.30 = 'MMFolder'
MMClasses.31 = 'MMFinder'

x = 1;

do while MMClasses.x <> ''   /* loop until no more classes */
      if SysDeregisterObjectClass(MMClasses.x) then
         say MMClasses.x 'class deregistration successful'
   x = x + 1
end

/* MMShadow class */
/* MMSHD.DLL is a forwarder to MMPARTS.DLL */
'@'curDir||'\install\repclass.exe WPShadow MMShadow u'
      if SysDeregisterObjectClass('MMShadow') then
         say 'MMSHadow class deregistration successful'


/* Do MMBitmap Replacement Class */
'@'curDir||'\install\repclass.exe WPBitmap MMBitmap u'
      if SysDeregisterObjectClass('MMBitmap') then
         say 'MMBitmap class deregistration successful'

/* Correct settings of some MM objects */

/* Digital Audio Player */
rc=SysSetObjectData("<MMPM_DAPLAYER1>", "EXENAME="||thePath||"\DLL\CWMMBack\WEPM.EXE;STARTUPDIR="||thePath||"\DLL\CWMMBack;")
rc=SysSetObjectData("<MMPM_DAPLAYER2>", "EXENAME="||thePath||"\DLL\CWMMBack\WEPM.EXE;STARTUPDIR="||thePath||"\DLL\CWMMBack;")

/* Midi Player */
rc=SysSetObjectData("<MMPM_MIDIPLAYER1>", "EXENAME="||thePath||"\DLL\CWMMBack\MPPM.EXE;STARTUPDIR="||thePath||"\DLL\CWMMBack;")
rc=SysSetObjectData("<MMPM_MIDIPLAYER2>", "EXENAME="||thePath||"\DLL\CWMMBack\MPPM.EXE;STARTUPDIR="||thePath||"\DLL\CWMMBack;")

/* Video Player */
rc=SysSetObjectData("<MMPM2_SOFTWARE_MOTION_VIDEO1>", "EXENAME="||thePath||"\DLL\CWMMBack\MPPM.EXE;STARTUPDIR="||thePath||"\DLL\CWMMBack;")
rc=SysSetObjectData("<MMPM2_SOFTWARE_MOTION_VIDEO2>", "EXENAME="||thePath||"\DLL\CWMMBack\MPPM.EXE;STARTUPDIR="||thePath||"\DLL\CWMMBack;")

/* CD Player */
rc=SysSetObjectData("<MMPM_CDPLAYER1>", "EXENAME="||thePath||"\DLL\CWMMBack\CDPM.EXE;STARTUPDIR="||thePath||"\DLL\CWMMBack;")
rc=SysSetObjectData("<MMPM_CDPLAYER2>", "EXENAME="||thePath||"\DLL\CWMMBack\CDPM.EXE;STARTUPDIR="||thePath||"\DLL\CWMMBack;")
rc=SysSetObjectData("<MMPM_CDPLAYER3>", "EXENAME="||thePath||"\DLL\CWMMBack\CDPM.EXE;STARTUPDIR="||thePath||"\DLL\CWMMBack;")


'CLS'
SAY 
SAY
SAY '[31;1;m***********************************************[0;m'
SAY
SAY '    You[31;1;m MUST[0;m restart the WPS now!!'
SAY
SAY '[31;1;m***********************************************[0;m'
SAY ''
'@Pause'
EXIT
