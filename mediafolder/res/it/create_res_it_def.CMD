/* */
call RxFuncAdd 'SysLoadFuncs', 'RexxUtil', 'SysLoadFuncs'
call SysLoadFuncs

deffile=ARG(1)
if ARG(1)="" then deffile="tst.def"

SAY ""
SAY "Creating DEF file "deffile
SAY ""

theVersion=VALUE("MMCLSVERSION", ,"OS2ENVIRONMENT")
if theVersion="" THEN theVersion=0.0.0

def="; mediafolder_it.def: DEF-file for italian resource DLL"||'0d'x||'0a'x
def=def||";"||'0d'x||'0a'x

def=def||";The following license applies ONLY TO THIS resource DLL. Be aware"||'0d'x||'0a'x
def=def||";that the whole Media-folder package may be covered by another license."||'0d'x||'0a'x
def=def||";"||'0d'x||'0a'x
def=def||";/*"||'0d'x||'0a'x
def=def||"; * Copyright (c) Chris Wohlgemuth 2002-"||WORD(DATE(), 3)  ||'0d'x||'0a'x
def=def||"; * All rights reserved."||'0d'x||'0a'x
def=def||"; *"||'0d'x||'0a'x
def=def||"; * http://www.geocities.com/SiliconValley/Sector/5785/"||'0d'x||'0a'x
def=def||"; * http://www.os2world.com/cdwriting"||'0d'x||'0a'x
def=def||"; *"||'0d'x||'0a'x
def=def||"; * Redistribution and use in source and binary forms, with or without"||'0d'x||'0a'x
def=def||"; * modification, are permitted provided that the following conditions"||'0d'x||'0a'x
def=def||"; * are met:"||'0d'x||'0a'x
def=def||"; * 1. Redistributions of source code must retain the above copyright"||'0d'x||'0a'x
def=def||"; *    notice, this list of conditions and the following disclaimer."||'0d'x||'0a'x
def=def||"; * 2. Redistributions in binary form must reproduce the above copyright"||'0d'x||'0a'x
def=def||"; *    notice, this list of conditions and the following disclaimer in the"||'0d'x||'0a'x
def=def||"; *    documentation and/or other materials provided with the distribution."||'0d'x||'0a'x
def=def||"; * 3. The authors name may not be used to endorse or promote products"||'0d'x||'0a'x
def=def||"; *    derived from this software without specific prior written permission."||'0d'x||'0a'x
def=def||"; *"||'0d'x||'0a'x
def=def||"; * THIS SOFTWARE IS PROVIDED ``AS IS'' AND"||'0d'x||'0a'x
def=def||"; * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE"||'0d'x||'0a'x
def=def||"; * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE"||'0d'x||'0a'x
def=def||"; * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE"||'0d'x||'0a'x
def=def||"; * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL"||'0d'x||'0a'x
def=def||"; * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS"||'0d'x||'0a'x
def=def||"; * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)"||'0d'x||'0a'x
def=def||"; * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT"||'0d'x||'0a'x
def=def||"; * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY"||'0d'x||'0a'x
def=def||"; * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF"||'0d'x||'0a'x
def=def||"; * SUCH DAMAGE."||'0d'x||'0a'x
def=def||"; *"||'0d'x||'0a'x
def=def||"; */"||'0d'x||'0a'x

def=def||'0d'x||'0a'x

def=def||"LIBRARY MFLDR_IT INITGLOBAL"||'0d'x||'0a'x
def=def||"DESCRIPTION '$@#(C) Gabriele Gamba "
def=def||WORD(DATE(),3)||":"||LEFT(theVersion,3)||"        #@##1## "||DATE("E")||" "||TIME("N")||"        My ;-)  ::::"||WORD(TRANSLATE(theVersion," ", "."),3)||"::@@Media-Folder V"||theVersion||" resource DLL -italian-'"||'0d'x||'0a'x
def=def||"PROTMODE"||'0d'x||'0a'x
def=def||"CODE LOADONCALL DISCARDABLE"||'0d'x||'0a'x
def=def||"DATA SINGLE SHARED DISCARDABLE"||'0d'x||'0a'x
def=def||"EXPORTS  "||'0d'x||'0a'x
def=def||"        queryLanguage"||'0d'x||'0a'x

'@type NUL > 'deffile
call lineout deffile,def
call stream deffile, "C", "close"
exit
