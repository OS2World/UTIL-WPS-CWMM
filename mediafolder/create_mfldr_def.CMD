/* */
call RxFuncAdd 'SysLoadFuncs', 'RexxUtil', 'SysLoadFuncs'
call SysLoadFuncs

deffile=ARG(1)
if ARG(1)="" then deffile="tst.def"

theVersion=VALUE("MMCLSVERSION", ,"OS2ENVIRONMENT")
if theVersion="" THEN theVersion=0.0.0

def="; mediafolder.DEF: DEF-file for mediafld.dll"||'0d'x||'0a'x
def=def||";"||'0d'x||'0a'x
def=def||";                    (C) Chris Wohlgemuth 2002-"||WORD(DATE(), 3)  ||'0d'x||'0a'x
def=def||";/*"||'0d'x||'0a'x
def=def||"; * This program is free software; you can redistribute it and/or modify"||'0d'x||'0a'x
def=def||"; * it under the terms of the GNU General Public License as published by"||'0d'x||'0a'x
def=def||"; * the Free Software Foundation; either version 2, or (at your option)"||'0d'x||'0a'x
def=def||"; * any later version."||'0d'x||'0a'x
def=def||"; *"||'0d'x||'0a'x
def=def||"; * This program is distributed in the hope that it will be useful,"||'0d'x||'0a'x
def=def||"; * but WITHOUT ANY WARRANTY; without even the implied warranty of"||'0d'x||'0a'x
def=def||"; * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the"||'0d'x||'0a'x
def=def||"; * GNU General Public License for more details."||'0d'x||'0a'x
def=def||"; *"||'0d'x||'0a'x
def=def||"; * You should have received a copy of the GNU General Public License"||'0d'x||'0a'x
def=def||"; * along with this program; see the file COPYING.  If not, write to"||'0d'x||'0a'x
def=def||"; * the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA."||'0d'x||'0a'x
def=def||"; */"||'0d'x||'0a'x
def=def||";/*"||'0d'x||'0a'x
def=def||"; * If you need another license for your project/product contact me at"||'0d'x||'0a'x
def=def||"; * "||'0d'x||'0a'x
def=def||"; * http://www.os2world.com/cdwriting"||'0d'x||'0a'x
def=def||"; * http://www.geocities.com/SiliconValley/Sector/5785/"||'0d'x||'0a'x
def=def||"; */"||'0d'x||'0a'x

def=def||'0d'x||'0a'x

def=def||"LIBRARY MEDIAFLD INITGLOBAL"||'0d'x||'0a'x
def=def||"DESCRIPTION '$@#(C) C. Wohlgemuth "
def=def||WORD(DATE(),3)||":"||LEFT(theVersion,3)||"        #@##1## "||DATE("E")||" "||TIME("N")||"        My ;-)  ::::"||WORD(TRANSLATE(theVersion," ", "."),3)||"::@@Media-Folder V"||theVersion||"'"||'0d'x||'0a'x
def=def||"PROTMODE"||'0d'x||'0a'x
def=def||"CODE LOADONCALL DISCARDABLE"||'0d'x||'0a'x
def=def||"DATA SINGLE SHARED DISCARDABLE"||'0d'x||'0a'x
def=def||"EXPORTS  "||'0d'x||'0a'x
def=def||"        CWMediaFolderClassData"||'0d'x||'0a'x
def=def||"        CWMediaFolderCClassData"||'0d'x||'0a'x
def=def||"        CWMediaFolderNewClass"||'0d'x||'0a'x
def=def||""||'0d'x||'0a'x
def=def||"        M_CWMediaFolderClassData"||'0d'x||'0a'x
def=def||"        M_CWMediaFolderCClassData"||'0d'x||'0a'x
def=def||"        M_CWMediaFolderNewClass"||'0d'x||'0a'x
def=def||""||'0d'x||'0a'x
def=def||"        CWAudioShadowClassData"||'0d'x||'0a'x
def=def||"        CWAudioShadowCClassData"||'0d'x||'0a'x
def=def||"        CWAudioShadowNewClass"||'0d'x||'0a'x
def=def||""||'0d'x||'0a'x
def=def||"        M_CWAudioShadowClassData"||'0d'x||'0a'x
def=def||"        M_CWAudioShadowCClassData"||'0d'x||'0a'x
def=def||"        M_CWAudioShadowNewClass"||'0d'x||'0a'x
def=def||""||'0d'x||'0a'x
def=def||"       CWCDFolderClassData"||'0d'x||'0a'x
def=def||"        CWCDFolderCClassData"||'0d'x||'0a'x
def=def||"        CWCDFolderNewClass"||'0d'x||'0a'x
def=def||""||'0d'x||'0a'x
def=def||"        M_CWCDFolderClassData"||'0d'x||'0a'x
def=def||"        M_CWCDFolderCClassData"||'0d'x||'0a'x
def=def||"        M_CWCDFolderNewClass"||'0d'x||'0a'x
def=def||""||'0d'x||'0a'x
def=def||"        CWM3UFolderClassData"||'0d'x||'0a'x
def=def||"        CWM3UFolderCClassData"||'0d'x||'0a'x
def=def||"        CWM3UFolderNewClass"||'0d'x||'0a'x
def=def||""||'0d'x||'0a'x
def=def||"        M_CWM3UFolderClassData"||'0d'x||'0a'x
def=def||"        M_CWM3UFolderCClassData"||'0d'x||'0a'x
def=def||"        M_CWM3UFolderNewClass"||'0d'x||'0a'x

'@type NUL > 'deffile
call lineout deffile,def
call stream deffile, "C", "close"
exit
