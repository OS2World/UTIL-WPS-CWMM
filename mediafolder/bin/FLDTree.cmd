/*
Create folder tree with playlists for all MP3 and OGG
files in a given directory and its subdirectories.

Requires the MMClasses from Chris Wohlgemuth.

Copyright (c) 2002 Eirik Oeverby (ltning@anduin.net)
This software is provided AS IS without any warranties
whatsoever. You are free to modify and distribute this
software in any way, shape or form, as long as the
author is given credit. If no significant parts of the
original code remains after modification, credit needs
not be given, but would be appreciated.
********************************************************/
/*

Slightly modified by Chris Wohlgemuth so that folders without
MP3s and OGGs are created as normal folders and folder names with
spaces are accepted.

*/

call RxFuncAdd 'SysLoadFuncs', 'rexxutil', 'SysLoadFuncs'
call SysLoadFuncs

/* Handle filenames with spaces */
IF POS('"', arg(1))=0 THEN
	parse VALUE arg(1) WITH src_dir dest_dir
ELSE
	parse VALUE arg(1) WITH '"'src_dir'"' '"'dest_dir'"'

/*!!!!!!! Set your desktop here !!!!!!!!!!!!!*/

DESKTOP_DIR='D:\Arbeitsoberflaeche'

/* Check parameters and print usage information */
if src_dir='' | dest_dir='' then do
PARSE source . . theFile
say 'Usage:'
say FILESPEC('name', theFile)' <source> <target>'
say
say 'Where:'
say '<source> = Directory to scan for MP3/OGG files'
say '<target> = Target folder in which to create the tree.'
say
say '<target> is a folder that will be created on your desktop. It will be created'
say 'if it does not exist, and its default view will be a tree view.'
say
say 'If <source> or <target> contain spaces then BOTH filenames have to be put'
say 'between quotation marks!'

exit
end

/* Create target folder */
dest_loc=DESKTOP_DIR
      rc=SySFileTree(src_dir'\*.mp3', 'mp3s', 'FO')
      rc=SySFileTree(src_dir'\*.ogg', 'OGGs', 'FO')
      IF mp3s.0==0 & oggs.0==0 THEN
	rc=SysCreateObject("WPFolder", dest_dir, "<WP_DESKTOP>", "OBJECTID=<PLST_"||dest_dir||">;DEFAULTVIEW=TREE", 'update')
      ELSE
	      rc=SysCreateObject("CWMediaFolder", dest_dir, "<WP_DESKTOP>", "OBJECTID=<PLST_"||dest_dir||"_"||i||">;DEFAULTVIEW=DETAILS;DETAILSCLASS=CWAudioShadow;SORTCLASS=CWAudioShadow;SORTBYATTR=6;DEFAULTSORT=-2;ALWAYSSORT=YES;", 'update')

if rc<>1 then do
      say "Object creation/update failed!"
      exit
end

say "Searching for directories, please wait..."
rc=SySFileTree(src_dir'\', 'dirs', 'DSO')

say "Creating folder tree..."
do i=1 to dirs.0
loc=dirs.i
loc=right(loc, length(loc)-length(src_dir)-1)
if loc<>'' then do
      loc=reverse(loc)
      parse var loc rest'\'base
      base=reverse(base)
      rest=reverse(rest)
      target=dest_loc||"\"||dest_dir||"\"||base
      target=strip(target, 't', '\')
      say "Creating media folder "||rest
      /* Check if directory contains MP3 or OGG. If not create normal folder */
      rc=SySFileTree(dirs.i'\*.mp3', 'mp3s', 'FO')
      rc=SySFileTree(dirs.i'\*.ogg', 'OGGs', 'FO')
      IF mp3s.0==0 & oggs.0==0 THEN
	      rc=SysCreateObject("WPFolder", rest, target, "OBJECTID=<PLST_"||dest_dir||"_"||i||">;DEFAULTVIEW=DETAILS;DETAILSCLASS=CWAudioShadow;SORTCLASS=CWAudioShadow;SORTBYATTR=6;DEFAULTSORT=-2;ALWAYSSORT=YES;", 'update')
      ELSE
	      rc=SysCreateObject("CWMediaFolder", rest, target, "OBJECTID=<PLST_"||dest_dir||"_"||i||">;DEFAULTVIEW=DETAILS;DETAILSCLASS=CWAudioShadow;SORTCLASS=CWAudioShadow;SORTBYATTR=6;DEFAULTSORT=-2;ALWAYSSORT=YES;", 'update')
end
   /* fname=filespec('name', files_mp3.i) */
end

say "Searching for music files, please wait..."
rc=SysFileTree(src_dir||'\*.mp3', 'files', 'FSO')
call create_shadows
rc=SysFileTree(src_dir||'\*.ogg', 'files', 'FSO')
call create_shadows

exit

create_shadows:
do i=1 to files.0
     loc=files.i
     loc=right(loc, length(loc)-length(src_dir)-1)
     target=dest_loc||"\"||dest_dir||"\"||filespec('path', loc)
     target=strip(target, 't', '\')
     fname=filespec('name', loc)
     say "Creating shadow for "||fname
     rc=SysCreateObject("CWAudioShadow", fname, target, "SHADOWID="||files.i, 'update')
end
return
