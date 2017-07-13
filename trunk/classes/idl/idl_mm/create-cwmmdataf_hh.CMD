@echo off
rem /* */
rem call RxFuncAdd 'SysLoadFuncs', 'RexxUtil', 'SysLoadFuncs'
rem call SysLoadFuncs

sc -shh -S 64000   -m noqualifytypes cwmmdataf.idl
