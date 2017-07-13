/* */
 call RxFuncAdd 'SysLoadFuncs', 'RexxUtil', 'SysLoadFuncs'
 call SysLoadFuncs

theDir=ARG(1)
IF theDir="" THEN theDir=DIRECTORY()

SAY "Base directory: "theDir
SAY

SAY "Counting CPP files..."
rc=SysFileTree(theDir"\*.cpp", "files.", "FSO")
numLines=0
DO a= 1 to files.0
	/* Count lines in file */
	DO WHILE LINES(files.a)
		numLines=numLines+1
		dummy=LINEIN(files.a)
	END
	call STREAM files.a, "C","close"
END
IF files.0\=0 THEN DO
	result._CPP=files.0
	result._CPPLines=numLines
END
else DO
	result._CPP=0
	result._CPPLines=0
END

SAY "Counting C files..."
rc=SysFileTree(theDir"\*.c", "files.", "FSO")
numLines=0
DO a= 1 to files.0
	/* Count lines in file */
	DO WHILE LINES(files.a)
		numLines=numLines+1
		dummy=LINEIN(files.a)
	END
	call STREAM files.a, "C","close"
END
IF files.0\=0 THEN DO
	result._C=files.0
	result._CLines=numLines
END
ELSE DO
	result._C=0
	result._CLines=0
END

SAY "Counting H files..."
rc=SysFileTree(theDir"\*.H", "files.", "FSO")
numLines=0
DO a= 1 to files.0
	/* Count lines in file */
	DO WHILE LINES(files.a)
		numLines=numLines+1
		dummy=LINEIN(files.a)
	END
	call STREAM files.a, "C","close"
END
IF files.0\=0 THEN DO
	result._H=files.0
	result._HLines=numLines
END
ELSE DO
	result._H=0
	result._HLines=0
END

SAY "Counting HH files..."
rc=SysFileTree(theDir"\*.HH", "files.", "FSO")
numLines=0
DO a= 1 to files.0
	/* Count lines in file */
	DO WHILE LINES(files.a)
		numLines=numLines+1
		dummy=LINEIN(files.a)
	END
	call STREAM files.a, "C","close"
END
IF files.0\=0 THEN DO
	result._HH=files.0
	result._HHLines=numLines
END
ELSE DO
	result._HH=0
	result._HHLines=0
END

SAY "Counting IDL files..."
rc=SysFileTree(theDir"\*.IDL", "files.", "FSO")
numLines=0
DO a= 1 to files.0
	/* Count lines in file */
	DO WHILE LINES(files.a)
		numLines=numLines+1
		dummy=LINEIN(files.a)
	END
	call STREAM files.a, "C","close"
END
IF files.0\=0 THEN DO
	result._IDL=files.0
	result._IDLLines=numLines
END
ELSE DO
	result._IDL=0
	result._IDLLines=0
END

SAY "Counting CMD files..."
rc=SysFileTree(theDir"\*.cmd", "files.", "FSO")
numLines=0
DO a= 1 to files.0
	/* Count lines in file */
	DO WHILE LINES(files.a)
		numLines=numLines+1
		dummy=LINEIN(files.a)
	END
	call STREAM files.a, "C","close"
END
IF files.0\=0 THEN DO
	result._cmd=files.0
	result._cmdLines=numLines
END
ELSE DO
	result._cmd=0
	result._cmdLines=0
END

SAY "Counting DLG files..."
rc=SysFileTree(theDir"\*.dlg", "files.", "FSO")
numLines=0
DO a= 1 to files.0
	/* Count lines in file */
	DO WHILE LINES(files.a)
		numLines=numLines+1
		dummy=LINEIN(files.a)
	END
	call STREAM files.a, "C","close"
END
IF files.0\=0 THEN DO
	result._dlg=files.0
	result._dlgLines=numLines
END
ELSE DO
	result._dlg=0
	result._dlgLines=0
END

SAY "Counting RC files..."
rc=SysFileTree(theDir"\*.rc", "files.", "FSO")
numLines=0
DO a= 1 to files.0
	/* Count lines in file */
	DO WHILE LINES(files.a)
		numLines=numLines+1
		dummy=LINEIN(files.a)
	END
	call STREAM files.a, "C","close"
END
IF files.0\=0 THEN DO
	result._rc=files.0
	result._rcLines=numLines
END
ELSE DO
	result._rc=0
	result._rcLines=0
END

SAY "Counting INC files..."
rc=SysFileTree(theDir"\*.inc", "files.", "FSO")
numLines=0
DO a= 1 to files.0
	/* Count lines in file */
	DO WHILE LINES(files.a)
		numLines=numLines+1
		dummy=LINEIN(files.a)
	END
	call STREAM files.a, "C","close"
END
IF files.0\=0 THEN DO
	result._inc=files.0
	result._incLines=numLines
END
ELSE DO
	result._inc=0
	result._incLines=0
END

SAY "Counting IPF files..."
rc=SysFileTree(theDir"\*.ipf", "files.", "FSO")
numLines=0
DO a= 1 to files.0
	/* Count lines in file */
	DO WHILE LINES(files.a)
		numLines=numLines+1
		dummy=LINEIN(files.a)
	END
	call STREAM files.a, "C","close"
END
IF files.0\=0 THEN DO
	result._ipf=files.0
	result._ipfLines=numLines
END
ELSE DO
	result._ipf=0
	result._ipfLines=0
END

SAY ""
SAY result._Cpplines||" lines in "||result._cpp||" CPP files"
SAY result._Clines||" lines in "||result._c||" C files"
SAY result._hlines||" lines in "||result._h||" H files"
SAY result._hhlines||" lines in "||result._hh||" HH files"
SAY result._idllines||" lines in "||result._idl||" IDL files"
SAY result._cmdlines||" lines in "||result._cmd||" CMD files"
SAY result._dlglines||" lines in "||result._dlg||" DLG files"
SAY result._rclines||" lines in "||result._rc||" RC files"
SAY result._inclines||" lines in "||result._inc||" INC files"
SAY result._ipflines||" lines in "||result._ipf||" IPF files"

numLines=result._ipflines+result._inclines+result._rclines+result._dlglines+result._cmdlines+result._hhlines+result._idllines+result._hlines+result._clines+result._cpplines

numFiles=result._ipf+result._inc+result._rc+result._dlg+result._cmd+result._hh+result._h+result._idl+result._c+result._cpp
SAY 
SAY numLines||" lines in "||numFiles||" files"

'pause'
