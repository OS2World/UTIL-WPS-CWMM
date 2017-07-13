/* */
call RxFuncAdd 'SysLoadFuncs', 'RexxUtil', 'SysLoadFuncs'
call SysLoadFuncs

buildInclude="build.inc"
DEF_LINE="BUILD_STRING = -Build "

if stream(buildInclude, 'C','query exists')="" THEN
'@echo 'DEF_LINE' 0 > 'buildInclude

theLine=LINEIN(buildInclude)
call stream buildInclude, 'C', 'close'
buildNr=WORD(theLine, 4)
IF buildNr="" THEN buildNr=0
buildNr=buildNr+1

SAY "New Build: "buildNr
SAY VALUE("CWMMRELEASE", ,"OS2ENVIRONMENT")
'@type NUL > 'buildInclude
call LINEOUT buildInclude, DEF_LINE||buildNr||' '||DATE(E)
call stream buildInclude, 'C', 'close'
SAY 
Exit(0)