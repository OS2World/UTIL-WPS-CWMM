/* Create documentation for functions */
 call RxFuncAdd 'SysLoadFuncs', 'RexxUtil', 'SysLoadFuncs'
 call SysLoadFuncs

/* Set this to '1' to get debug messages */
DEBUG=0

docInfo.=''

theDir=ARG(1)

IF theDir="" THEN theDir=STRIP(DIRECTORY())

/* Where to place the generated IPF source */
docdir="r:\temp"
ipf=docdir||'\CWTKDoc.ipf'            /* The IPF file        */

SAY "Base directory: "theDir
SAY

/*** Read all the C files and parse them ***/
SAY "Checking C files..."
rc=SysFileTree(theDir"\*.c", "files.", "FSO")

IF DEBUG=1 THEN DO
	SAY "Found the following C files:"
	DO a= 1 to files.0
		SAY " "||a||" "||files.a
	END
	SAY ""
END

numLines=0
index=0 /* This index holds the number of found descriptions */
DO a = 1 to files.0
	/* The stem contents holds the lines of the C source file */
	drop contents
	contents.=''
	numLines=0
	/* Count and read in lines of file */
	DO WHILE LINES(files.a)
		numLines=numLines+1
		contents.numlines=LINEIN(files.a)
 	END
	contents.0=numLines
	call STREAM files.a, "C","close"

	/* Now parse each source file */
	exposeList='files. theLine numLines index docinfo. contents.'
	call parseFile a
END


IF files.0\=0 THEN DO
	result._C=files.0
	result._CLines=numLines
END
ELSE DO
	result._C=0
	result._CLines=0
END


SAY ""
SAY result._Clines||" lines in "||result._c||" C files"

IF DEBUG=1 THEN DO
	/* Show the information we got */
	DO a= 1 to index
		SAY ''
		SAY a||': '
		SAY 'Function: 'docinfo.a.function
		SAY 'Line: 'docinfo.a.line
		SAY'Desc: 'docinfo.a.desc
		SAY 'File: 'docinfo.a.file
	END
END

/*create docs */
res=200                             /* The initial res id */
leftWidth="30%"
syntaxres=1                      /* Res id for syntax table. To be added to base. E.G. 201 */
remarksres=2
returnsres=3
exampleres=4
overrideres=5
usageres=6
paramsres=10

'@type NUL > 'ipf

/* Write IPF header */
call writeIpfHeader ipf

/* Sort the function names in the array a. */
a.=''
DO b= 1 to index
	a.b=STRIP(WORD(docinfo.b.function,2))
END
a.0=index
call qqsort 1, index

IF DEBUG=1 THEN DO
	/* Show the sorted functions */
	SAY 'Sorted functionarray:'
	DO b= 1 to index
		SAY b||': 'a.b
	END
END

/* Build a stem with the right order of indexes so in the document the functions are ordered */
indStem.=''
DO b=1 to index
	DO c=1 to index
		if STRIP(WORD(docinfo.c.function,2))=a.b THEN DO
		indStem.b=c
		LEAVE
		END
	END
END

/* Write function reference */
call writeIpfFunctionRef ipf

/* Footer */
call writeIpfFooter ipf

call STREAM ipf, "C","close"

/* Compile the document */
'@ipfc -i 'ipf

exit

/**************** Procedures *********************************/

writeIpfFunctionRef:

	IF DEBUG=1 THEN SAY "DEBUG: Writing function reference..."
	res=res+100
	call lineout ipf, ":h1 res="||res||".Function reference"
	call lineout ipf, ""

	/* Write the function descriptions. The syntax pane is always written. The others only if
	    sufficient information is available. */
	DO b=1 to index
		a=indStem.b
		res=res+100
		call writeIpfFunction                 /* Write header for this function desc panel */
		call writeIpfFuncLinks               /* Add links to left pane (Syntax, remarks...) */
		call writeIpfFuncSyntax             /* Write Syntax pane */
		call writeIpfFuncReturns            /* Write return pane (lower right) */
		call writeIpfFuncRemarks          /* Write remarks pane */
		call writeIpfFuncParams            /* Write a pane for every known parameter */
		call writeIpfFuncOverride           /* Write a panel for the override information */
		call writeIpfFuncUsage              /* Write a panel for the Usage information */
	END
return

/*********************/
writeIpfFunction:
		/* Write the header of the function panel */
		call lineout ipf, ".*************** "||getFunctionName(docinfo.a.function)||"() *****************"
		call lineout ipf, ":h2 res="||res
		call lineout ipf, "width="leftWidth
		call lineout ipf, "."||getFunctionName2(docinfo.a.function, docinfo.a.isSom)          /* Function name */
		call lineout ipf, ":link reftype=hd res="res+syntaxres               /* Syntax will be opened initialy */
		call lineout ipf, "auto dependent group=2."
		call lineout ipf, ":p."
return

/*******************************************/
/* Write the links in the left panel for   */
/* this function.                                  */
/*******************************************/
writeIpfFuncLinks:
		/* Syntax link is always written */
		call lineout ipf, ":link reftype=hd res="||res+syntaxres||" dependent.Syntax:elink."
		call lineout ipf, ".br"
		if docinfo.a.numparams ><0 THEN DO
			call lineout ipf, ":link reftype=hd res="||res+paramsres||" dependent.Parameters:elink."
			call lineout ipf, ".br"
		END
		IF docinfo.a.returns >< "" THEN DO
			call lineout ipf, ":link reftype=hd res="||res+returnsres||" dependent.Returns:elink."
			call lineout ipf, ".br"
		END 
		IF docinfo.a.remarks >< "" THEN DO
			call lineout ipf, ":link reftype=hd res="||res+remarksres||" dependent.Remarks:elink."
			call lineout ipf, ".br"
		END
		IF docinfo.a.override><"" THEN DO
			call lineout ipf, ":link reftype=hd res="||res+overrideres||" dependent.How to override:elink."
			call lineout ipf, ".br"
		END
		IF docinfo.a.usage><"" THEN DO
			call lineout ipf, ":link reftype=hd res="||res+usageres||" dependent.Usage:elink."
			call lineout ipf, ".br"
		END
		IF docinfo.index.example >< "" THEN DO
			call lineout ipf, ":link reftype=hd res="||res+exampleres||" dependent.Example:elink."
			call lineout ipf, ".br"
		END
return

/*********************************/
/* Write the panel for the return value */
/*********************************/
writeIpfFuncReturns:
	if docinfo.a.returns="" THEN return

	call lineout ipf, ":h2 res="||res+returnsres
	call lineout ipf, "x=30%"
	call lineout ipf, "width=70% height=35%"
	call lineout ipf, "group=3"
	call lineout ipf, "hide"
	call lineout ipf, "."||getFunctionName2(docinfo.a.function, docinfo.a.isSom)||" Return value - "||WORD(docinfo.a.returns, 2)
	call lineout ipf, ":p."
	call lineout ipf, ":hp2."||WORD(docinfo.a.returns, 2)||":ehp2. ("||WORD(docinfo.a.returns, 1)||") - returns"
	call lineout ipf, ":p."
	call lineout ipf, wrapString(SUBWORD(docinfo.a.returns, 3))
return

/*****************/
writeIpfFuncOverride:
	if docinfo.a.override="" THEN return

	call lineout ipf, ":h2 res="||res+overrideres
	call lineout ipf, "x=30%"
	call lineout ipf, "width=70%"
	call lineout ipf, "group=2"
	call lineout ipf, "hide"
	call lineout ipf, "."||getFunctionName2(docinfo.a.function, docinfo.a.isSom)||" - How to override"
	call lineout ipf, ":p."
	call lineout ipf, wrapString(docinfo.a.override)

return

/*****************/
writeIpfFuncUsage:
	if docinfo.a.usage="" THEN return

	call lineout ipf, ":h2 res="||res+usageres
	call lineout ipf, "x=30%"
	call lineout ipf, "width=70%"
	call lineout ipf, "group=2"
	call lineout ipf, "hide"
	call lineout ipf, "."||getFunctionName2(docinfo.a.function, docinfo.a.isSom)||" - Usage"
	call lineout ipf, ":p."
	call lineout ipf, wrapString(docinfo.a.usage)

return

/*****************/
writeIpfFuncParams:
	if docinfo.a.numparams=0 THEN return   /* No params given */

	/* Write big parameter panel */
	call lineout ipf, ":h2 res="||res+paramsres
	call lineout ipf, "x=30%"
	call lineout ipf, "width=70%"
	call lineout ipf, "group=2"
	call lineout ipf, "hide"
	call lineout ipf, "."||getFunctionName2(docinfo.a.function, docinfo.a.isSom)||" - Parameters"
	call lineout ipf, ":p."
	DO parms = 1 to docinfo.a.numparams
		pindex='param'||parms
		call lineout ipf, ":hp2."||WORD(docinfo.a.pindex, 2)||":ehp2. ("||WORD(docinfo.a.pindex, 1)||") - "||WORD(docinfo.a.pindex, 3)
		call lineout ipf, ":p."
		call lineout ipf, ":lm margin=5."
		call lineout ipf, wrapString(SUBWORD(docinfo.a.pindex, 4))
		call lineout ipf, ":lm margin=1."
		call lineout ipf, ":p."
	END
	call lineout ipf, ""          /* Space to make source readable */

	/* Write small panel (lower right) for every parameter */
	DO parms = 1 to docinfo.a.numparams
		pindex='param'||parms
		call lineout ipf, ":h2 res="||res+paramsres+parms
		call lineout ipf, "x=30%"
		call lineout ipf, "width=70% height=35%"
		call lineout ipf, "group=3"
		call lineout ipf, "hide"
		call lineout ipf, "."||getFunctionName2(docinfo.a.function, docinfo.a.isSom)||" Parameter - "||WORD(docinfo.a.pindex, 2)
		call lineout ipf, ":p."
		call lineout ipf, ":hp2."||WORD(docinfo.a.pindex, 2)||":ehp2. ("||WORD(docinfo.a.pindex, 1)||") - "||WORD(docinfo.a.pindex, 3)
		call lineout ipf, ":p."
		call lineout ipf, wrapString(SUBWORD(docinfo.a.pindex, 4))
	END
return

/*****************************/
/* Write the Remarks panel          */
/*****************************/
writeIpfFuncRemarks:
	if docinfo.a.remarks="" THEN return

	call lineout ipf, ":h2 res="||res+remarksres
	call lineout ipf, "x=30%"
	call lineout ipf, "width=70%"
	call lineout ipf, "group=2"
	call lineout ipf, "hide"
	call lineout ipf, "."||getFunctionName2(docinfo.a.function, docinfo.a.isSom)||" - Remarks"
	call lineout ipf, ":p."
	call lineout ipf, wrapString(docinfo.a.remarks)

return

/*****************/
writeIpfFuncSyntax:
		/* docinfo.a.function contains the full function header: e.g. 'BOOL theFunc(char theChar)' */
		call lineout ipf, ""
		call lineout ipf, ":h2 res="||res+syntaxres
		call lineout ipf, "x=30%"
		call lineout ipf, "width=70%"
		call lineout ipf, "group=2"
		call lineout ipf, "hide"
		call lineout ipf, "."||getFunctionName2(docinfo.a.function, docinfo.a.isSom)||" - Syntax"
		call lineout ipf, ":p."
		call lineout ipf, wrapString(docinfo.a.desc)
		call lineout ipf, ":nt."
		call lineout ipf, "This function can be found in the source file :hp2."||FILESPEC("name", docinfo.a.file) ||":ehp2.."
		call lineout ipf, ":ent."
		call lineout ipf, ":xmp."
		call lineout ipf, ""
		call lineout ipf, ":parml compact tsize=25 break=none."
	/* Check if parameters are given. If yes print them and provide a link in the syntax panel */
		allParams=''
		IF docinfo.a.numparams >< 0 THEN DO
		DO parms = 1 to docinfo.a.numparams
			pindex='param'||parms
			allParams=allParams||":pt."||WORD(docinfo.a.pindex, 1)||":pd.:link reftype=hd res="||res+paramsres+parms||" dependent."WORD(docinfo.a.pindex, 2 )":elink.;"
		END
		END
	/* Check if return info is given. If yes provide a link on syntax panel */
		IF docinfo.a.returns >< "" THEN DO
			allParams=allParams||":pt."||WORD(docinfo.a.returns, 1)||":pd.:link reftype=hd res="||res+returnsres||" dependent."WORD(docinfo.a.returns, 2 )":elink.;"
			theString=WORD(docinfo.a.returns,2)||" = "||removeTypesFromFunc(SUBWORD(docinfo.a.function,2))||';'
		END /* docinfo.a.returns */
		ELSE
			theString=removeTypesFromFunc(SUBWORD(docinfo.a.function,2))||';'
		call lineout ipf, allParams
		call lineout ipf, ":eparml."
		thePos=1
		tabPos=1
	/* Split function in lines */
		DO WHILE thePos >< 0
			theString=STRIP(SUBSTR(theString, thePos))
			thePos=POS(',' , theString)
			if thePos >< 0 THEN DO
				if tabPos=1 then DO
					call lineout ipf, LEFT(theString, thePos)
					tabPos=POS('(' , theString)
				END
				ELSE
					call lineout ipf, COPIES(' ',tabPos)||LEFT(theString, thePos)
				thePos=thePos+1
			END
			ELSE
				call lineout ipf, COPIES(' ',tabPos)||theString
		END
		call lineout ipf, ":exmp."
		call lineout ipf, ""
return

/***************************/
writeIpfHeader: procedure expose res
ipf=ARG(1)
	IF DEBUG=1 THEN
		 SAY "DEBUG: Writing IPF header..."
	call lineout ipf, ":userdoc."
	call lineout ipf, ""
	call lineout ipf, ":docprof."
	call lineout ipf, ""
	call lineout ipf, ":title.Function reference"
	call lineout ipf, ""
	call lineout ipf, ":h1 res=100.Introduction"
	call lineout ipf, ":p."
	call lineout ipf, "Created "DATE()
	call lineout ipf, ":p."
	call lineout ipf, "To be written..."
return

/**************************/
writeIpfFooter:
	IF DEBUG=1 THEN
		SAY "DEBUG: Writing IPF footer..."
	/* Footer */
	call lineout ARG(1), ":euserdoc."
return


/*************************************/
/* This proc removes the types from  */
/* the function definition but keeps */
/* the var names.                    */
/*************************************/
removeTypesFromFunc: procedure
	theString=SPACE(ARG(1))
	/* Make sure there's always a space after '(' and ',' */
	thePos=POS('(', theString)
	theString=INSERT(' ', theString, thePos)
	thePos2=POS(' ' , theString, thePos+3)
	/* Remove '*' in front of somSelf if any (have to check if this is a hack...) */
	IF thePos2 >< 0 THEN DO
		tmpString=STRIP(subStr(theString, thePos2))
		IF LEFT(tmpString, 1)='*' THEN
			tmpString=RIGHT(tmpString, LENGTH(tmpString)-1)
		theString=LEFT(theString, thePos)||STRIP(tmpString)
	END
	thePos=POS(',' , theString)
	DO while thePos >< 0
		theString=INSERT(' ', theString, thePos)
		thePos2=POS(' ' , theString, thePos+3)
/* 12.08.05 be sure a single '*' is removed from any type or var.
   MAy not work with '**' */
IF SUBSTR(theString, thePos2+1,1)='*' THEN DO
	thePos2=thePos2+2
END
		theString=LEFT(theString, thePos)||STRIP(subStr(theString, thePos2))
		thePos=POS(',', theString, thePos+1)
	END
return theString


/*******************************/
/* Strip the parameter list from the   */
/* function and return only the name */
/*                                                 */
/* During file parsing the whole        */
/* function declaration was read in   */
/*******************************/
getFunctionName: procedure
	func=ARG(1)
	thePos=POS('(', func)
	func=LEFT(func,thePos-1)
	
return WORD(func,WORDS(func))

/*******************************/
/* Strip the parameter list from the   */
/* function and return only the name */
/*                                                 */
/* During file parsing the whole        */
/* function declaration was read in   */
/*******************************/
getFunctionName2: procedure
	func=ARG(1)
	thePos=POS('(', func)
	func=LEFT(func,thePos-1)
	func=WORD(func,WORDS(func))
	IF ARG(2)="YES" THEN	
		return RIGHT(func, LENGTH(func)-1)
	ELSE
		return func

/***************************************************************/

/*******************************/
/* Parse the contents of the source  */
/* file. This means finding comments  */
/* containing function descriptions.    */
/*******************************/
parseFile: procedure expose (exposelist)

a=ARG(1)
	curline=0
	DO while curline <= contents.0
		curline=curline+1
		tempLine=STRIP(contents.curline)
		if LENGTH(templine)<5 then iterate

		if LEFT(tempLine, 4)="/*!*" then do
			/* Comment with function description found. Now parse it. */
			call parseComment a
		END
	END
return

/*******************************/
/* Parse the whole comment block   */
/* with information                         */
/*******************************/
parseComment: 
	index=index+1
	docinfo.index.file=STRIP(files.a)
	docinfo.index.remarks=""
	docinfo.index.returns=""
	docinfo.index.example=""
	docinfo.index.numparams=0
	docinfo.index.override=""
	docinfo.index.usage=""
	DO WHILE curline<=contents.0
		curline=curline+1
		theLine=STRIP(contents.curline)
		IF POS("@@",theLine) ><0 THEN call parseEntry   /* Found a parameter */

		IF LEFT(STRIP(theLine), 5)="/*!!*" THEN DO
			/* End of comment. Get function name. */
			docinfo.index.line=curline+1                    /* Line of function */
			docinfo.index.function=getCompleteFunction()
			return
		END
 	END
return

/********************************/
/* Read lines until a '{' is found.          */
/* This means reading in a function    */
/* declaration.                                 */
/*                                                   */
/*                                                   */
/*                                                   */
/********************************/

getCompleteFunction:
	theLine= ""		
	DO WHILE curline<=contents.0
		curLine=curLine+1
		theLine=theLine||STRIP(contents.curline)
		IF POS("{",theLine) ><0 THEN LEAVE
 	END
	theLine=STRIP(TRANSLATE(theLine,' ','{'))
	/* Ok, at this point we have the whole function declaration including the parameters */
	/* Check if we have a SOM function */
	IF POS("SOMLINK", theLine) >< 0 THEN DO
		/* Mark as a SOM function */
		docinfo.index.isSom="YES"
		/* Strip SOMLINK and SOM_Scope and the prefix */
		thePos=POS('_', SUBWORD(theLine, 4))
		theLine=WORD(theLine, 2)||" "||substr(SUBWORD(theLine,4), thePos)
		
	END
return theLine

/*******************************/
/* Parse entries starting with @@ in  */
/* comment block.                          */
/*******************************/
parseEntry:
	entry=stripCommentChars(theLine)
	SELECT
	WHEN entry="@@DESC" THEN DO
		/* Description */
		docinfo.index.desc=readEntryContents()
	END
	WHEN entry="@@REMARKS" THEN DO
		/* Remarks */
		docinfo.index.remarks=readEntryContents()
	END
	WHEN entry="@@RETURNS" THEN DO
		/* Return value */
		docinfo.index.returns=readEntryContents()
	END
	WHEN entry="@@EXAMPLE" THEN DO
		/* Example */
		docinfo.index.example=readEntryContents()
	END
	WHEN entry="@@OVERRIDE" THEN DO
		/* Override of a SOM method */
		docinfo.index.override=readEntryContents()
	END
	WHEN entry="@@USAGE" THEN DO
		/* Usage of a SOM method */
		docinfo.index.usage=readEntryContents()
	END
	WHEN LEFT(entry, 7)="@@PARAM" THEN DO
		docinfo.index.numparams=docinfo.index.numparams+1
		parm='param'||docinfo.index.numparams
		docinfo.index.parm=readEntryContents()
	END
	WHEN entry="@@MODULE" THEN DO
		/* Module specification */
		docinfo.index.module=readEntryContents()
	END
	OTHERWISE
		NOP
	END
return ""

/************************************/
/* Read the text associated with a known */
/* entry.                                               */
/* Reading takes place until another entry */
/* is found or the comment ends.              */
/************************************/
readEntryContents:
	theEntry=""
	DO WHILE curline<=contents.0
		curLine=curLine+1
		theLine=STRIP(contents.curline)
		IF POS("@@",theLine) ><0 THEN DO
			curline=curline-1
			return theEntry
		END
		IF LEFT(STRIP(theLine), 5)="/*!!*" THEN return theEntry
					/* SAY '!!'||stripCommentChars(theLine)||'!!' */

		if TRANSLATE(stripCommentChars(theLine))=":P." THEN DO
			theEntry=theEntry||stripCommentChars(theLine)
		END
		ELSE DO
			IF LENGTH(theEntry)>3 THEN DO
				IF TRANSLATE(RIGHT(theEntry,3))=":P." THEN
					theEntry=theEntry||stripCommentChars(theLine)
				ELSE
					theEntry=theEntry||' '||stripCommentChars(theLine)
			END
			ELSE DO
				theEntry=theEntry||' '||stripCommentChars(theLine)
			END

		END
		theEntry=STRIP(theEntry)
/*SAY '%%'||theEntry||'%%'*/
 	END
return theEntry

/********************************/
/* Remove starting and ending           */
/* comment chars from arg.               */
/********************************/
stripCommentChars:
	theString=STRIP(SUBSTR(ARG(1),3))
return STRIP(LEFT(theString, LENGTH(theString)-2))


/********************************/
/* Wrap a string over several lines by  */
/* inserting '0d'x'0a'x.                        */
/********************************/
wrapString:

inString=ARG(1)
numLines=TRUNC(LENGTH(inString)/80)
DO widx = 1 to numlines
	thePos=POS(' ',inString ,widx*80)
	if thePos=0 THEN return inString
	inString=INSERT('0d'x'0a'x, inString, thePos)
END
return inString 

 
    



  
 /* ------------------------------------------------------------------ */
/*  Author: Ruediger Wilke */
 /* function: quick sort routine                                       */
 /*                                                                    */
 /* call:     QuickSort first_element, last_element                    */
 /*                                                                    */
 /* returns:  nothing                                                  */
 /*                                                                    */
 /* notes:    You must save the elements to sort in the stem "a."      */
 /*           a.0 must contain the number of elements in the stem.     */
 /*                                                                    */
 /*                                                                    */
 qqsort: procedure expose a.
 
   arg lf, re
 
   if re -lf < 9 then
     do lf = lf to re -1
 
       m = lf
 
       do j = lf +1 to re
         if TRANSLATE(a.j) << TRANSLATE(a.m) then                                   /* v2.80 */
           m = j
       end /* j = lf +1 to re */
 
       t = a.m; a.m = a.lf; a.lf = t
 
     end /* lf = lf to re -1 */
     else
     do
       i = lf
       j = re
       k = (lf + re)%2
       t = TRANSLATE(a.k)
 
       do until i > j
 
         do while TRANSLATE(a.i) << t                         /* v2.80 + CW */
           i = i + 1
         end /* while a.i << t */
 
         do while TRANSLATE(a.j) >> t                         /* v2.80 + CW */
           j = j - 1
         end /* while a.j >> t */
 
         if i <= j then
         do
           xchg = a.i
           a.i = a.j
           a.j = xchg
           i = i + 1
           j = j - 1
         end /* if i <= j then */
 
       end /* until i > j */
 
       call qqsort lf, j
       call qqsort i, re
     end /* else */
 
 return
 
 
   

