� �� 0�   d    h   i   j   k   f   l   m   n   o   p   q   C  a  �    �   �   �   �   �   ,   �  �  �  �  �  �   c  b	  d  X   �	  �  ��� 0J$  errorHandler�theLine=SIGL
call d500.open

theMsg="An error occured:"||'0a'x
IF theLine > 15 THEN theLine=theLine-15

DO a=theLine to theLine+15
	IF a=SIGL THEN
		theMsg=theMsg||'0a'x||'0a'x||"--->   "||SOURCELINE(a)||"   <---"||'0a'x
	ELSE
		theMsg=theMsg||'0a'x||SOURCELINE(a)
END
DO a=theLine+16 to theLine+30
		theMsg=theMsg||'0a'x||SOURCELINE(a)
END

call d500.c1122.text theMsg

/*rc=RxMessageBox('Syntax error detected! Instruction is: '||SOURCELINE(SIGL)||'. Contact the author.',,,'ERROR')
*/
SIGNAL return
getLanguage�PROCEDURE


theLang=VALUE("LANG",,"OS2ENVIRONMENT")
if theLang="" then return "EN"
IF LENGTH(theLang)<2 then return "EN"
theLang=TRANSLATE(LEFT(theLang, 2))

/* Try to load an entry */
rc=SysIni(ARG(1), theLang, "installed")
IF rc="ERROR:" THEN return "EN"

return TRANSLATE(RIGHT(theLang,2))

sizeImgControls�
PARSE VALUE d200.position() WITH x y cx cy
y2=globalData._cySizeBorder

IF globalData._size<>"" THEN DO
	PARSE VAR globalData._size width height

	IF globalData._showControls="YES" THEN DO
		PARSE VALUE d300.position() WITH x3 y3 cx3 cy3	
		y2=y2+cy3
		cy=cy-cy3
	END

	cy=cy-2*globalData._cySizeBorder-globalData._cyTitlebar
	cx=cx-2*globalData._cxSizeBorder

	factor=cy / height
	IF width * factor > cx then factor=cx / width
	newWidth=TRUNC(width*factor)
	newHeight=TRUNC(height*factor)

/*	IF globalData._currentImgCntrl=0 THEN*/
		call d200.c201.position globalData._cxSizeBorder+TRUNC((cx-newWidth)/2), y2+TRUNC((cy-newHeight)/2), newWidth, newHeight
/*	ELSE*/
	call d200.c202.position globalData._cxSizeBorder+TRUNC((cx-newWidth)/2), y2+TRUNC((cy-newHeight)/2), newWidth, newHeight

 	IF globalData._showControls="YES" THEN DO
		/* Position the play control */
		PARSE VALUE d200.position() WITH x y cx cy
		IF x<>TRUNC((cx-cx3)/2) THEN
			call d300.position TRUNC((cx-cx3)/2),  globalData._cySizeBorder, cx3, cy3
	END 
	
END
ELSE DO
	IF globalData._showControls="YES" THEN DO
		PARSE VALUE d300.position() WITH x3 y3 cx3 cy3	
		y2=y2+cy3
		cy=cy-cy3
		IF x<>TRUNC((cx-cx3)/2) THEN
			call d300.position TRUNC((cx-cx3)/2),  globalData._cySizeBorder, cx3, cy3

	END
END

setThreadVars�rc=Val('global._gbm',globalData._gbm)
rc=Val('global._yscreen',globalData._cyscreen)
rc=Val('global._time',d100.c108.Select())
rc=Val('global._random', d100.c109.SELECT())
rc=Val('global._quit', "NO")
rc=Val('global._tempDir', globalData._tempDir)

/* This will be overriden if in interactive mode. See
'Start' button. */
  rc=Val('global.1', globalData._dirToShow)
  rc=Val('global.0', 1)

showPlayControls0globalData._showControls="YES"
call d300.open
hidePlayControlsDglobalData._showControls="NO"
call d300.close
call sizeImgControlspreviousPicture#	rc=Val('global._quit', "PREVIOUS")nextPicture rc=Val('global._quit', "NEXT")
goWindow;globalData._fullscreen=0
call d200.open
call d400.close
goFullScreen�globalData._fullscreen=1
call d400.open
call d400.show
pos=d200.Position()
	rc=SysIni(globalData._inifile, "windowposition","slidewindow", pos)
call d200.close

stopShow�/* */

call d100.c110.enable
rc=Val('global._quit', "YES")

/* Stop button */
call d100.c111.disable
call disable

if globalData._fullscreen=1 THEN
	call d400.close
ELSE DO
	call d200.close
END	pauseShow�if globalData._pause="YES" THEN DO
	rc=Val('global._quit', "")
	globalData._pause="NO"
END
ELSE DO
	rc=Val('global._quit', "PAUSE_")
	globalData._pause="YES"
END


GetTempDir�PROCEDURE

theDir=VALUE("TMP",,"OS2ENVIRONMENT")
IF theDir="" THEN theDir=VALUE("TEMP",,"OS2ENVIRONMENT")
IF theDir="" THEN theDir=VALUE("TMPDIR",,"OS2ENVIRONMENT")
IF theDir="" THEN theDir=VALUE("TEMPDIR",,"OS2ENVIRONMENT")
/* We need a fallback at some point in the future here */

theDir=TRANSLATE(theDir, "\", "/")
if RIGHT(theDir, 1)='\' THEN theDir=LEFT(theDir, LENGTH(theDir)-1)

return theDir

showThread�/* */

globalData._gbm=Val('global._gbm')
globalData._cyscreen=Val('global._yscreen')
globalData._random=Val('global._random')
globalData._tempDir=Val('global._tempDir')

/* Get dirs for thread */
theDirs.0=Val('global.0')
DO a=1 to theDirs.0
  theDirs.a=Val('global.'a)
END

allFiles.=''
allFiles.0=0
Do c=1 to thedirs.0
rc=SysFileTree(theDirs.c||"\*.*", theFiles., "SO")
	DO a= 1 to theFiles.0
		b=allFiles.0
		b=b+1
		allFiles.b=theFiles.a
		allFiles.0=b
	end
END


if globalData._random=1 THEN DO
/* Randomize the files */
	DO a= 1 to allFiles.0
		r=RANDOM(1, allFiles.0)
		tmpFile=allFiles.r
		allFiles.r=allFiles.1
		allFiles.1=tmpFile
	END
END

tmpFile= ""
b=0
newHeight =globalData._cyscreen
DO a= 1 to allFiles.0
/*	SAY theFiles.a*/
	
	ADDRESS CMD 'del "'||tmpFile'" 2>NUL 1 >NUL'
	tmpFile= SysTempFileName(globalData._tempDir||"\img????.bmp");
	theCommand='"'globalData._gbm'" -a  -h '||newheight||' "'||allFiles.a||'" "'|| tmpFile'"'
 	ADDRESS CMD '"'theCommand'"' 
	rc=SSCtrlQueryImageInfo(tmpFile)
	IF rc<>"ERROR:" THEN DO
	PARSE VAR rc width height .
	call notify d100 , "SIZE", width||" "||height
	call notify d100 , "CONVERTED", tmpFile

	/* Wait fot time to elapse */
	DO t=1 to Val('global._time')*10
		/* sleep only 100 ms so we get user interaction */
		rc=Sleep(100)
		if "YES"=Val('global._quit') THEN DO
			leave a
		END
		IF "PAUSE_"=Val('global._quit') THEN DO
			DO WHILE "PAUSE_"=Val('global._quit') 
				rc=Sleep(200)
			END
			/* Reset pause marker and text if any */
			call notify d100 , "PAUSE", tmpFile
			leave t
		END
		IF "NEXT"=Val('global._quit') THEN DO
			rc=Val('global._quit', "")
			leave t
		END
		IF "PREVIOUS"=Val('global._quit') THEN DO
			rc=Val('global._quit', "")
			IF a>1 THEN DO
				a=a-2
				leave t
			END
		END
	END
	END
END
ADDRESS CMD 'del "'||tmpFile'" 2>NUL 1 >NUL'
theEnd�/* Save window position */
pos=d100.Position()
	rc=SysIni(globalData._inifile, "windowposition","main", pos)


/* Save last list */
num=d100.c104.Item()
IF num>0 THEN DO
	/* Discard previous list */
	numDirs=SysIni(globalData._inifile, "lastdirs","num")
	if numDirs\="ERROR:" THEN DO
		Do a=1 to numDirs
			rc=SysIni(globalData._inifile, "lastdirs", a, "DELETE:")
		END
		rc=SysIni(globalData._inifile, "lastdirs","num", 0)
	END
	rc=SysIni(globalData._inifile, "lastdirs","num", num)
	DO a=1 to num
		rc=SysIni(globalData._inifile, "lastdirs", a, d100.c104.Item(a))
	END
END
	rc=SysIni(globalData._inifile, "settings","random", d100.c109.select())
	rc=SysIni(globalData._inifile, "settings","time", d100.c108.select())


Exit

init�call RxFuncAdd 'SysLoadFuncs', 'RexxUtil', 'SysLoadFuncs'
call SysLoadFuncs

 SIGNAL on SYNTAX NAME errorHandler 

DLLVERSION="drctl017"
parse source . . sourceFile

thedir=FILESPEC('drive', sourcefile)||FILESPEC('path', sourcefile)
theDir=LEFT(theDir,LENGTH(theDir)-1)

globalData._directory=theDir

oldDir=DIRECTORY(theDir||"\bin")
/* Register user controls  */
rc=RxFuncAdd("SSCtrlRegister", DLLVERSION , "DRCtrlRegister")
call SSCtrlRegister
rc=RxFuncAdd("SSCtrlVersion", DLLVERSION , "DRCtrlVersion")
rc=RxFuncAdd("SSCtrlPickDirectory", DLLVERSION , "DRCtrlPickDirectory")
rc=RxFuncAdd("SSCtrlQueryImageInfo", DLLVERSION , "DRCtrlQueryImageInfo")
rc=RxFuncAdd("SSCtrlSetParent", DLLVERSION , "DRCtrlSetParent")
rc=RxFuncAdd("SSCtrlQuerySysValue", DLLVERSION , "DRCtrlQuerySysValue")
rc=RxFuncAdd("SSCtrlSleep", DLLVERSION , "DRCtrlSleep")

/* rc=DIRECTORY(oldDir) */

call SSCtrlQuerySysValue("globalData.")

globalData._gbm=theDir||"\bin\gbmsize.exe"
globalData._yscreen=globalData._cyscreen
globalData._xscreen=globalData._cxscreen

globalData._inifile=theDir||"\slidshow.ini"
globalData._tempDir=GetTempDir()
globalData._osVersion=RIGHT(SysOS2Ver(),2)

globalData._random=SysIni(globalData._inifile, "settings","random")
IF globalData._random="ERROR:" THEN globalData._random=0
globalData._time=SysIni(globalData._inifile, "settings","time")
IF globalData._time="ERROR:" then globalData._time=5;

globalData._currentImgCntrl=0
globalData._fullscreen=0
globalData._size=""
globalData._currentImg=""
globalData._showControls="NO"
globalData._languagefile=theDir"\lang\lang.ini"
globalData._lang=getLanguage(globalData._languagefile)

IF ARG()=1 & ARG(1)<> "" THEN DO
	/* We only want a diashow for one folder */
	globalData._dirToShow=ARG(1)
	globalData._silent="YES"
	globalData._showControls="YES"
END
ELSE DO
/*
	IF SysSaveOBject('<CW_SLIDESHOW_PGM>', 'TRUE') =0 THEN DO
		theTitle=SysIni(globalData._languagefile,globalData._lang, "question")
		IF theTitle="ERROR:" THEN theTitle="Question"
		theText=SysIni(globalData._languagefile, globalData._lang, "questiontext")
		IF theText="ERROR:" THEN theText="There's no WPS object for the Slide Show. Do you want me to create one on your desktop?"

		rc=RxMessagebox(theText, theTitle, "YESNO", "QUESTION")
		IF rc=6 THEN DO
			theTitle=SysIni(globalData._languagefile,globalData._lang, "wpsobject")
			IF theTitle="ERROR:" THEN theTitle="Slide Show"
			rc=SysCreateObject("WPProgram", theTitle, "<WP_DESKTOP>", "EXENAME="||sourceFile||";OBJECTID=<CW_SLIDESHOW_PGM>", "update" )
		END
	END
*/
	globalData._silent="NO"
END

�� �d 0!  !  �  ��         
 �    � j d ���         � � &  < h ����       
 �  �� T /  i ����        �   � E /  j ����       	 �  �� ' /  k ����        ��  8  f ����          � �@    	 l ����        � �r  @ 	 m ����        � �  "  n ����        �  �&  "  o ����          �P  '  p ����          ��  "  q ����          � $ � E C����          �  �  a����Slide show     Add dir... Add file... Remove... Time (sec):  Random Start Stop Quit Help Directories Settings ���d 0�  �d Key�call eventdata

/* SAY eventdata.1 eventdata.2*/
if eventdata.1 ="ESC" THEN call theEnd

if eventdata.1 ="F1" THEN
	call d600.open
Notify�call EventData

IF eventdata.1="SWITCHFULLSCREEN" THEN 
	call goFullScreen

IF eventdata.1="SWITCHWINDOW" THEN
	call goWindow

/* Save current size */
IF eventdata.1="SIZE" THEN
	globalData._size=eventdata.2

IF eventdata.1="PAUSE" THEN DO
if globalData._pause="YES" THEN DO
	rc=Val('global._quit', "")
	globalData._pause="NO"
END
END

IF globalData._fullscreen=0 THEN DO
/* There're two dialogs. One for fullscreen one
for windowed slide show */
IF eventdata.1="SIZE" THEN
	call notify d200 , "SIZE", eventdata.2
IF eventData.1="CONVERTED" THEN DO
	globalData._currentImg=eventdata.2
	call notify d200 , "CONVERTED", eventdata.2
END
END
ELSE DO
IF eventdata.1="SIZE" THEN
	call notify d400 , "SIZE", eventdata.2
IF eventData.1="CONVERTED" THEN DO
	call notify d400 , "CONVERTED", eventdata.2
	globalData._currentImg=eventdata.2
END
ENDOpenx
IF globalData._silent="NO" THEN
	call show
ELSE DO
	call setThreadVars
	call d200.open
	rc=start(showThread)
ENDInit�if globalData._osVersion >=40 then
	call Font '9.WarpSans'

pos=SysIni(globalData._inifile, "windowposition","main")
if pos\="ERROR:" THEN DO
	PARSE var pos x y dx dy
	call position x, y, dx, dy
END

rc=SysIni(globalData._languageFile, globalData._lang, "main")
IF rc<>"ERROR:" THEN call text rc
�aInitfrc=SysIni(globalData._languageFile, globalData._lang, "settings")
IF rc<>"ERROR:" THEN call text rc
�CInitirc=SysIni(globalData._languageFile, globalData._lang, "directories")
IF rc<>"ERROR:" THEN call text rc
�q Clickcall d600.open
Initbrc=SysIni(globalData._languageFile, globalData._lang, "help")
IF rc<>"ERROR:" THEN call text rc
�p Initbrc=SysIni(globalData._languageFile, globalData._lang, "quit")
IF rc<>"ERROR:" THEN call text rc
Clickcall theEnd�o Initbrc=SysIni(globalData._languageFile, globalData._lang, "stop")
IF rc<>"ERROR:" THEN call text rc
Clickcall stopShow�n Initcrc=SysIni(globalData._languageFile, globalData._lang, "start")
IF rc<>"ERROR:" THEN call text rc
Click�call setThreadVars

/* add the directories */
num=d100.c104.Item()
rc=Val('global.0', num)
DO a=1 to num
  rc=Val('global.'a, d100.c104.Item(a))
END

call Disable
call d100.c111.enable

if globalData._fullscreen=1 THEN call goFullScreen
ELSE DO
	call d200.open
END
rc=start(showThread)
�m Init�rc=SysIni(globalData._languageFile, globalData._lang, "random")
IF rc<>"ERROR:" THEN call text rc


call select globaldata._random�l Changing rc=Val('global._time', Select())Init1CALL RANGE 1, 999
CALL SELECT globalData._time
�f Initbrc=SysIni(globalData._languageFile, globalData._lang, "time")
IF rc<>"ERROR:" THEN call text rc
�k Initgrc=SysIni(globalData._languageFile, globalData._lang, "removedir")
IF rc<>"ERROR:" THEN call text rc
ClickDitem=d100.c104.select()
if ITEM\=0 THEN rc=d100.c104.delete(item)
�i Initdrc=SysIni(globalData._languageFile, globalData._lang, "adddir")
IF rc<>"ERROR:" THEN call text rc
Click�ret=SSCtrlPickDirectory( "" ,"Add image directory")

/* Check if item is already in list */
num=d100.c104.item()


found=0
DO a= 1 to num
	if TRANSLATE(ret)=TRANSLATE(d100.c104.item(a)) THEN DO
		found=1
		LEAVE
	END
END
IF found=0 THEN 
	call d100.c104.add ret
ELSE
	call RxMessageBox "This directory was already added to the list."
�h DropOcall EventData


IF eventData.4="FILE" THEN DO
	call add EventData.1
END
Init�num=SysIni(globalData._inifile, "lastdirs","num")
IF num\="ERROR:" THEN DO
	DO a=1 to num
		call Add SysIni(globalData._inifile, "lastdirs", a)
	END
END

 CALL Drop "ANY:ANY=ANY"�� �� 0�   �   �  ��         
 h     � k � ��s     	 w   �    
 1 ' 3 � ����    	 �   �       4  � ����Slide Show 3   DRD_IMAGE  DRD_IMAGE  ���� 0x  �� Timerccall timer

pos=position()
rc=SysIni(globalData._inifile, "windowposition","slidewindow", pos)
Movecall timer
call timer 500
Sizeccall sizeImgControls /* Resizes and recenters the iamge controls */
call timer
call timer 500

Exit�/* This dialog will also be closed when going to fullscreen
	so check if we really quit */

IF globalData._fullscreen=0 THEN DO
	IF globalData._Silent="NO" THEN
		call stopShow
	ELSE
		call theEnd
END
IF globalData._showControls="YES" THEN
	call d300.close
Key�call eventdata

/* SAY eventdata.1 eventdata.2*/

if eventdata.1 = "SPACE" THEN call pauseShow

if eventdata.1 ="ESC" THEN DO
	IF globalData._Silent="NO" THEN DO
		call stopShow
	END
	ELSE DO
		call stopShow
		call theEnd
	END
END
if eventdata.1 ="NEWLINE" THEN call notify d100, "SWITCHFULLSCREEN"
if eventdata.1 ="RIGHT" THEN call nextPicture
if eventdata.1 ="LEFT" THEN call previousPicture
if eventdata.1 ="s" THEN call showPlayControls
if eventdata.1 ="h" THEN call hidePlayControls
if eventdata.1 ="F1" THEN
	call d600.open
Open�call show
IF globalData._showControls="YES" THEN
	call d300.open


if globalData._size\="" THEN DO

call sizeImgControls

IF globalData._currentImgCntrl=0 THEN DO
	call d200.c201.text globaldata._currentImg
	call d200.c202.hide
	call d200.c201.show
	globalData._currentImgCntrl=1
END
ELSE DO
	call d200.c202.text globalData._currentImg
	call d200.c201.hide
	call d200.c202.show
	globalData._currentImgCntrl=0
END
END

call show
Init�call color "-", "#0 0 0"
pos=SysIni(globalData._inifile, "windowposition","slidewindow")
if pos\="ERROR:" THEN DO
	PARSE var pos x y dx dy
	call position x, y, dx, dy
END

rc=SysIni(globalData._languageFile, globalData._lang, "main")
IF rc<>"ERROR:" THEN call text rc
Notify�call EventData

IF EventData.1="SIZE" THEN DO
	PARSE VAR EVENTDATA.2 width height /* Sent from img create thread */

	globalData._size=width||" "||height
	pos=position()
	rc=SysIni(globalData._inifile, "windowposition","slidewindow", pos)

END

/* Tmpfile name sent by thread */
IF EventData.1="CONVERTED" THEN DO
IF globalData._currentImgCntrl=0 THEN DO
	call d200.c201.text eventdata.2
	call d200.c202.hide
	call sizeImgControls
	call d200.c201.show
	globalData._currentImgCntrl=1
END
ELSE DO
	call d200.c202.text eventdata.2
	call d200.c201.hide
	call sizeImgControls
	call d200.c202.show
	globalData._currentImgCntrl=0
END
END



/*

IF EventData.1="SIZE" THEN DO
	PARSE VALUE position() WITH x y cx cy
	PARSE VAR EVENTDATA.2 width height /* Sent from img create thread */

	cy=cy-2*globalData._cySizeBorder-globalData._cyTitlebar
	cx=cx-2*globalData._cxSizeBorder

	y2=globalData._cySizeBorder
	IF globalData._showControls="YES" THEN DO
		PARSE VALUE d300.position() WITH x3 y3 cx3 cy3	
		y2=y2+cy3
		cy=cy-cy3
	END

	factor=cy / height
	IF width * factor > cx then factor=cx / width
	newWidth=TRUNC(width*factor)
	newHeight=TRUNC(height*factor)
	IF globalData._currentImgCntrl=0 THEN
		call d200.c201.position globalData._cxSizeBorder+TRUNC((cx-newWidth)/2), y2+TRUNC((cy-newHeight)/2), newWidth, newHeight
	ELSE
		call d200.c202.position globalData._cxSizeBorder+TRUNC((cx-newWidth)/2), y2+TRUNC((cy-newHeight)/2), newWidth, newHeight
	globalData._size=width||" "||height
END
*/

�� Init
call color "-", "#0 0 0"�� Initcall color "-", "#0 0 0"�� ��0�   �   �  ��           h    & I � w ���i     	 m   w     3 ' 3 � ����    	 x   �       4  � ����     DRD_IMAGE  DRD_IMAGE  ����02  ��Key�call eventdata

if eventdata.1 = "SPACE" THEN DO
	call pauseShow
END
if eventdata.1 ="ESC" THEN DO
	call notify d100, "SWITCHWINDOW"
END
if eventdata.1 ="F1" THEN
	call d600.open

if eventdata.1 ="RIGHT" THEN
	rc=Val('global._quit', "NEXT")
if eventdata.1 ="LEFT" THEN
	rc=Val('global._quit', "PREVIOUS")
Open�call show

if globalData._size\="" THEN DO
PARSE VALUE position() WITH x y cx cy
PARSE VAR globalData._size width height

factor=cy / height
IF width * factor > cx then factor=cx / width
newWidth=TRUNC(width*factor)
newHeight=TRUNC(height*factor)

IF globalData._currentImgCntrl=0 THEN DO
	call c201.position TRUNC((cx-newWidth)/2), TRUNC((cy-newHeight)/2), newWidth, newHeight
	call c201.text globalData._currentImg
	call c202.hide
	call c201.show
	globalData._currentImgCntrl=1
END
ELSE DO
	call c202.position TRUNC((cx-newWidth)/2), TRUNC((cy-newHeight)/2), newWidth, newHeight
	call c202.text globalData._currentImg
	call c201.hide
	call c202.show
	globalData._currentImgCntrl=0
END
Init^call color "-", "#0 0 0"

 call position 0, 0, globalData._cxScreen, globalData._cyscreen 
Notify�call EventData

IF EventData.1="SIZE" THEN DO
	PARSE VALUE position() WITH x y cx cy
	PARSE VAR EVENTDATA.2 width height

	factor=cy / height
	IF width * factor > cx then factor=cx / width
	newWidth=TRUNC(width*factor)
	newHeight=TRUNC(height*factor)
	IF globalData._currentImgCntrl=0 THEN
		call c201.position TRUNC((cx-newWidth)/2), TRUNC((cy-newHeight)/2), newWidth, newHeight
	ELSE
		call c202.position TRUNC((cx-newWidth)/2), TRUNC((cy-newHeight)/2), newWidth, newHeight
END

/* Tmpfile name sent by thread */
IF EventData.1="CONVERTED" THEN DO
IF globalData._currentImgCntrl=0 THEN DO
	call c201.text EventData.2
	call c202.hide
	call c201.show
	globalData._currentImgCntrl=1
END
ELSE DO
	call c202.text EventData.2
	call c201.hide
	call c202.show
	globalData._currentImgCntrl=0
END
END

�� Init2
call color "-", "#0 0 0"

call text "#h 1"

�� Init.call color "-", "#0 0 0"

call text "#h 1"
�� �,0�   �   �  ��           �    s } �  ,���         �   �    �����        �   �    �����        �   �<    �����        �   �Y    �����       
 �   �{  %  �����     << Stop Pause >> Fullscreen ���,0�  �,Open�/* The ID of the main dialog is 200  */
/* The ID of the child dialog is 300 */


call SSCtrlSetParent 300, 200
IF globalData._showControls="YES" THEN DO
	call sizeImgControls
	call show
	call TOP
ENDKey�call eventdata

if eventdata.1 ="ESC" THEN DO
	IF globalData._Silent="NO" THEN
		call stopShow
	ELSE DO
 		call stopShow
		call theEnd
	END
END

if eventdata.1 ="s" THEN call showPlayControls
if eventdata.1 ="h" THEN call hidePlayControls��Inithrc=SysIni(globalData._languageFile, globalData._lang, "fullscreen")
IF rc<>"ERROR:" THEN call text rc
Click$call notify d100, "SWITCHFULLSCREEN"��Initbrc=SysIni(globalData._languageFile, globalData._lang, "next")
IF rc<>"ERROR:" THEN call text rc
Clickcall NextPicture��Initcrc=SysIni(globalData._languageFile, globalData._lang, "pause")
IF rc<>"ERROR:" THEN call text rc
Click�call pauseShow

IF globalData._pause="YES" THEN DO
	rc=SysIni(globalData._languageFile, globalData._lang, "continue")
	IF rc<>"ERROR:" THEN call text rc
	ELSE call text "~Continue"
END
ELSE  DO
	rc=SysIni(globalData._languageFile, globalData._lang, "pause")
	IF rc<>"ERROR:" THEN call text rc
	ELSE call text "~Pause"
END
��Initbrc=SysIni(globalData._languageFile, globalData._lang, "stop")
IF rc<>"ERROR:" THEN call text rc
Clickcall stopShow��Initfrc=SysIni(globalData._languageFile, globalData._lang, "previous")
IF rc<>"ERROR:" THEN call text rc
Click call previousPicture �� ��0�   �   �  ��          �   K 6 � � ����          � @� p �  c����      
   �  �  � ^ b����        �   �      d����Error      OK ����0I  ��Open�PARSE VALUE POSITION() WITH x y cx cy
call position TRUNC((globalData._cxScreen-cx)/2), TRUNC((globalData._cyscreen-cy)/2) 

call showInit�if globalData._osVersion >=40 then
	call Font '9.WarpSans'

CALL Color  "-", "#15"

rc=SysIni(globalData._languageFile, globalData._lang, "error")
IF rc<>"ERROR:" THEN call text rc
�dInitbrc=SysIni(globalData._languageFile, globalData._lang, "ok")
IF rc<>"ERROR:" THEN call text rc

Click5 SIGNAL on SYNTAX NAME errorHandler 
call d500.close�cInit�rc=SysIni(globalData._languageFile, globalData._lang, "errortext")
IF rc<>"ERROR:" THEN call text rc
ELSE call text "An error occured during execution of this program. Send a notice to the author with the message below attached. Or even better try to fix it on your own using the provided source code and send a patch."�� �X0�   �   �  ��          h   6 5 � o X��m       
   q 7 �  � b �r ��        �  �      �� ��Help                  Ok             ���X0%  �XKey?call eventdata


if eventdata.1 = "ESC" THEN call d600.closeOpen�PARSE VALUE POSITION() WITH x y cx cy
call position TRUNC((globalData._cxScreen-cx)/2), TRUNC((globalData._cyscreen-cy)/2) 

call showInitWCALL Color  "-", "#15"
if globalData._osVersion >=40 then
	call Font '9.WarpSans'

��Clickcall d600.closeInitbrc=SysIni(globalData._languageFile, globalData._lang, "ok")
IF rc<>"ERROR:" THEN call text rc

��Initfrc=SysIni(globalData._languageFile, globalData._lang, "helptext")
IF rc<>"ERROR:" THEN call text rc
�