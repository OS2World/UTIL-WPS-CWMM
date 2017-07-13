/* rexx

   This script is part of the media folder package.

   (c) Chris Wohlgemuth 2002-2003

   http://www.os2world.com/cdwriting
   http://www.geocities.com/SiliconValley/Sector/5785/

*/

call RxFuncAdd 'SysLoadFuncs', 'RexxUtil', 'SysLoadFuncs'
call SysLoadFuncs

IF ARG(1)='' THEN DO
SAY ''
SAY 'Dieses Skript setzt die Detailklasse auf CWAudioShadow'
SAY 'Folder auf skript fallen lassen'
SAY ''

'@PAUSE'
EXIT
END


/*rc=SysSetObjectData(ARG(1), 'DETAILSCLASS=CWAudioShadow;SORTCLASS=CWAudioShadow;DETAILSTODISPLAY=0,1,4,12,13,14,15,16,17,18,19,20,21;SORTBYATTR=6,24,25,26,27,28,29,30,31,32,33')*/

rc=SysSetObjectData(ARG(1), 'DETAILSCLASS=CWAudioShadow;SORTCLASS=CWAudioShadow;')

IF rc=0 THEN DO
	SAY 'Fehler beim Erstellen!'
	'@PAUSE'
	Exit
END

'@PAUSE'
EXIT
END

/*
Sort:

Gr”áe: 6

Details:

0:  Symbol
1:  Title
2:  Klasse
3:  Echter Name
4:  Gr”áe

12: Playtime
13: Samplerate
14: Channels
15: BPS
16: Title
17: Artist
18: Album
19: Year
20: Comment
21: Genre
*/
