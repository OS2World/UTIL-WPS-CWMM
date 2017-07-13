/* */
 call RxFuncAdd 'SysLoadFuncs', 'RexxUtil', 'SysLoadFuncs'
 call SysLoadFuncs


parse source . . sourceFile

thedir=FILESPEC('drive', sourcefile)||FILESPEC('path', sourcefile)
theDir=LEFT(theDir,LENGTH(theDir)-1)

globalData._languagefile=theDir"\lang\lang.ini"
theLang=getLanguage(globalData._languageFile)
theTitle=SysIni(globalData._languagefile, thelang, "wpsobject")
IF theTitle="ERROR:" THEN theTitle="Slide Show"
rc=SysCreateObject("WPProgram", theTitle, "<WP_DESKTOP>", "EXENAME="||theDir||"\slide-show.exe;OBJECTID=<CW_SLIDESHOW_PGM>", "update" )

EXIT

getLanguage: PROCEDURE


theLang=VALUE("LANG",,"OS2ENVIRONMENT")
if theLang="" then return "EN"
theLang=LEFT(theLang, 5)
IF LENGTH(theLang)<2 then return "EN"

theLang=TRANSLATE(RIGHT(theLang,2))
/* Try to load an entry */
rc=SysIni(ARG(1), theLang, "installed")
IF rc="ERROR:" THEN return "EN"

return TRANSLATE(RIGHT(theLang,2))

