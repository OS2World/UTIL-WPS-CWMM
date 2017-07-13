/* rexx - This script is called by the FreeDB query program.
   It sets the info of the CD tracks.

   This script is part of the media folder package.

   (c) Chris Wohlgemuth 2002-2003

   http://www.os2world.com/cdwriting
   http://www.geocities.com/SiliconValley/Sector/5785/

 */

call RxFuncAdd 'SysLoadFuncs', 'RexxUtil', 'SysLoadFuncs'
call SysLoadFuncs

/*
#define SETUP_AUDIOTRACKNAME    "CWAUDIOTRACKNAME"
#define SETUP_AUDIOARTIST       "CWAUDIOARTIST"
#define SETUP_AUDIOALBUM        "CWAUDIOALBUM"
#define SETUP_AUDIOCOMMENT      "CWAUDIOCOMMENT"
#define SETUP_AUDIOYEAR         "CWAUDIOYEAR"
#define SETUP_AUDIOGENRE        "CWAUDIOGENRE"
#define SETUP_AUDIOPLAYTIME     "CWAUDIOPLAYTIME"
*/

SAY ''
SAY ''
SAY ARG(1)

PARSE VALUE ARG(1) WITH drive cddbData theFolder 

drive=STRIP(TRANSLATE(drive,'  ','":'))
drive=TRANSLATE(drive)
cddbData=STRIP(TRANSLATE(cddbData,' ','"'))
theFolder=STRIP(TRANSLATE(theFolder,' ','"'))

rc=checkCddbData(cddbData)
if rc=0 then exit

SAY 'cddb data found, now processing...'
SAY ''

/* Seems we have all we need. Start processing the cddb data */

/* Read the cddb data into a stem */
rc=readCddbData()

disktitle=searchCddbValue("DTITLE=")
/* Split title and artist */
thePos=POS('/',disktitle)
if thePos\=0 THEN DO
	artist=STRIP(LEFT(disktitle,thePos-1))
	disktitle=STRIP(substr(disktitle,thePos+1))
END

SAY 'Album: 'disktitle
say 'Artist: 'artist

basename='<CDTRACK_'||drive||'_'
DO a=0 to 98 /* A CD has got 99 tracks max */
	/* Put cddb info into stem */
	tracktitle=searchCddbValue("TTITLE"||a)
 	if tracktitle='' THEN LEAVE
	setupstring='CWAUDIOTRACKNAME='||tracktitle||';CWAUDIOARTIST='||artist||';CWAUDIOALBUM='||disktitle
	rc=SysSetObjectData(basename||a+1||'>',setupstring)

	setupstring='CWAUDIOREFRESH=1'
	rc=SysSetObjectData(basename||a+1||'>',setupstring) 

END

exit



/*******************************************************************/
/* Procedures */
/*******************************************************************/

/* Search an entry in the cddb data file */
searchCddbValue: PROCEDURE expose cddb.

parse arg keyword

found=0
DO i=1 to cddb.0 while found=0
  if POS(keyword,cddb.i)=1 then found=i
END

if found<>0 then return SUBSTR(cddb.found,POS('=',cddb.found)+1)

return ''


/* Read the cddb data into a compund var */
readCddbData:

/* Clear var */
DROP cddb.
cddb.=""
cddb.0=0

/* Read the file */
DO i=cddb.0+1 while lines(cddbdata)<>0
   cddb.i=LINEIN(cddbdata)
END
cddb.0=i-1
return 0


/************************/
checkCddbData: PROCEDURE

cddbData=ARG(1)

rc=STREAM(cddbData ,'c','query exists')
if rc\='' then return 1
return 0

