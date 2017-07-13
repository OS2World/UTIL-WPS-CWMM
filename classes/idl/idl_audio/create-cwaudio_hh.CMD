@echo off
rem /* */
rem call RxFuncAdd 'SysLoadFuncs', 'RexxUtil', 'SysLoadFuncs'
rem call SysLoadFuncs

sc -shh -S 64000  -I ../idl_mm -m noqualifytypes cwaudio.idl
