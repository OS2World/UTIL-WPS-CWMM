/*                                                         */
/* Create DEF file for a MM helper program from a template */
/*                                                         */
/*         (c) Chris Wohlgemuth 2005                       */
/*                                                         */

call RxFuncAdd 'SysLoadFuncs', 'RexxUtil', 'SysLoadFuncs'
call SysLoadFuncs

DEBUG=0


deffile=ARG(1)
if ARG(1)="" then deffile="tst.def"

/* License file to be added to the DEF file */
licenseFile=DIRECTORY()||'\'||license.txt

/* Template file */
templateFile=ARG(2)
IF arg(2)="" THEN
	templateFile=DIRECTORY()||'\'||def.tpl

/* Get the version of the classes */
theVersion=VALUE("MMCLSVERSION", ,"OS2ENVIRONMENT")
if theVersion="" THEN theVersion="0.0.0"

/* The stem holding all the information we get from the template file */
fileData.=''


call getTemplateData templateFile
/* Index */
call assignTemplateVars

IF DEBUG=1 THEN DO
	DO a=1 to filedata.0
		SAY fileData.a.sectionname":"
		SAY fileData.a
	END
END

/* Specify save defaults */
IF fileData.buildmachine="" THEN
	fileData.buildmachine="Default"
fileData.buildmachine.desc="Build machine: "

IF fileData.author="" THEN
	fileData.author="                      "
fileData.author.desc="Author: "
IF fileData.name='' THEN
	fileData.name=""

IF DEBUG=1 THEN DO
	SAY ""
	SAY "License file: "||licensefile
	SAY "DEF-file: "||deffile
	SAY "Version: "||theversion
	SAY "Template data: "
	DO a=1 to fileData.0
		SAY " "||fileData.a.desc||": "||fileData.a
	END
END

'@type NUL > 'deffile

/* Write header */
call lineout deffile, ';'
call lineout deffile, '; '||WORD(fileData.name,2)||": "fileData.description
call lineout deffile, ';'

/* Write copyright */
call lineout deffile, ';'
IF fileData.copyright><"" THEN
	call lineout deffile, ';    '||fileData.copyright||"-"WORD(DATE(),3)
ELSE
	call lineout deffile, ';    '||fileData.author||" "WORD(DATE(),3)
call lineout deffile, ';'
/* Write creation date */
call lineout deffile, ';'
call lineout deffile, '; Created '||DATE()||" "||TIME()
call lineout deffile, ';'

/* Write license */
DO WHILE LINES(licenseFile)
	theLine=LINEIN(licenseFile)
        IF LENGTH(theLine) >0 THEN DO
		IF substr(theLine,1,1) >< ';' THEN
			theLine=';'||theLine
	END
	ELSE
	   theLine=theLine||';'
	call LINEOUT deffile, theLine
END
call stream licenseFile, "C", "close"

/* Now write the NAME data */
call lineout deffile, fileData.name

/* Description with bldlevel info */
timeData=OVERLAY(DATE("E")||" "||TIME("N"), "                       ")
versionData=OVERLAY( LEFT(theVersion,3),"           ")
def="DESCRIPTION '$@#"||fileData.author||" "
def=def||WORD(DATE(),3)||":"||versionData||"#@##1## "||timeData
def=def||"  "||SUBSTR(fileData.buildmachine,1,11)||"::::"||WORD(TRANSLATE(theVersion," ", "."),3)
def=def||"::@@"||fileData.description||" V"||theVersion||"'"
call lineout deffile, def

/* Additional info we may have */
call  lineout deffile, fileData.other

call stream deffile, "C", "close"

SAY "New DEF file created for V"||theVersion||"."
exit

/************************************************************/
/* Assign the symbols specified by the sections list the position value
   they have in the list of sections. By doing this the symbols may be used
   as indexes in the filedata. stem*/
assignTemplateVars: procedure expose (fileData.sections)
	DO a=1 to WORDS(fileData.sections)
		call VALUE WORD(fileData.sections,a),a
	END
return

/************ Parse the template file ***********************/
getTemplateData: procedure expose fileData.
	theFile=arg(1)

	fileData.=''
	fileContents.=''

	fileContents.0=0
	/* Read in the file for easier processing */
	DO WHILE LINES(theFile)
		fileContents.0=fileContents.0+1
		b=fileContents.0
		fileContents.b=LINEIN(theFile)
	END
	call stream theFile, "C", "close"

	DO b=1 to fileContents.0
		theLine=fileContents.b
		fileContents.current=b
		section = Strip(Translate(word(theline,1)))
		select
	    		when (Left(section,1) = '#') | section = '' then
			do
			/* Jump comments and empty lines */
			end;
		WHEN LEFT(section,1)="[" THEN DO
			call parseSection
			b=fileContents.current
		END
		OTHERWISE
			nop			
		END

	END
	fileData.0=WORDS(fileData.sections)
	DROP fileContents.
return

parseSection: procedure expose fileData. fileContents.
	b=fileContents.current
	/* Add this section to the list of sections */
	filedata.sections=filedata.sections||" "||SPACE(TRANSLATE(fileContents.b, "  ", "[]"))
	index=WORDS(fileData.sections)
	fileData.index=""
	fileData.index.sectionname=SPACE(TRANSLATE(fileContents.b, "  ", "[]"))
	/* Parse this section */
	b=b+1
	DO a=b to fileContents.0
		theLine=fileContents.a
		fileContents.current=a
		section = Strip(Translate(word(theline,1)))
		select
	    		when (Left(section,1) = '#') | section = '' then
			do
			/* Jump comments and empty lines */
			end;
		WHEN LEFT(section,1)="[" THEN DO
			/* New section jump back after correcting the index */
			fileContents.current=fileContents.current-1
			return
		END
		OTHERWISE
			IF fileData.index <>"" THEN fileData.index=fileData.index||'0d'x||'0a'x
			fileData.index=fileData.index||theLine			
		END

	END

return
/***********************************************/
