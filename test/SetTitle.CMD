/* */
 call RxFuncAdd 'SysLoadFuncs', 'RexxUtil', 'SysLoadFuncs'
 call SysLoadFuncs

file=ARG(1)
file=STRIP(TRANSLATE(file,' ','"'))
SAY ''
SAY ''
SAY ''
SAY file

say SysSetObjectData(file, "MMAUDIOTRACKNAME=Zu Sp„t123456")

'pause'