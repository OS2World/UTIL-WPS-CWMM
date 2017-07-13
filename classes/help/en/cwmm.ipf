:userdoc.

:docprof.

:title.Help

:h1 res=010.MP3 audio file
:p.
This file is an MP3 audio file. It may be played using MMOS/2
after installing of MMIOMP3 a free multimedia IO procedure.
MP3 files contain compressed audio data.
The compression is not lossless.
:p.
Get the IO procedure from
:p.
:link reftype=launch
object='netscape.exe'
data='http://www.math.berkeley.edu/~roconnor/'.
http&colon.//www.math.berkeley.edu/~roconnor/:elink.

:h1 res=011.ID3 Tag
:p.
Use this page for the editing of ID3 tags.
If the entry fields
are empty this file does not have any tag yet.
The new information is only saved after pressing the :hp2.Save:ehp2.
button.

:p.
The following information may be specified:
:ul.
:li.:hp2.Name:ehp2.
.br
The name of the track. The length is restricted to 30 characters.

:li.:hp2.Artist:ehp2.
.br
The name of the Artist. The length is restricted to 30 characters.

:li.:hp2.Album:ehp2.
.br
The album containing this track. The length is restricted to 30 characters.

:li.:hp2.Comment:ehp2.
.br
An arbitrary comment. The length is restricted to 30 characters.

:li.:hp2.Year:ehp2.
.br
The year the album was published.

:li.:hp2.Genre:ehp2.
.br
The Genre of this track.

:eul.

:h1 res=013.ID3 Tag
:p.
This page displays the ID3 tags of the MP3 file. ID3 is a standard to add information
to an MP3 file like the title of the track or the tracks artist.
:p.
If the entry fields are empty this file does not have any tag yet.

:p.
:ul.
:li.:hp2.Name:ehp2.
.br
The name of the track. The length is restricted to 30 characters.

:li.:hp2.Artist:ehp2.
.br
The name of the Artist. The length is restricted to 30 characters.

:li.:hp2.Album:ehp2.
.br
The album containing this track. The length is restricted to 30 characters.

:li.:hp2.Comment:ehp2.
.br
An arbitrary comment. The length is restricted to 30 characters.

:li.:hp2.Year:ehp2.
.br
The year the album was published.

:li.:hp2.Genre:ehp2.
.br
The Genre of this track.

:eul.

:h1 res=012.Track names
:p.
Use this page for the editing of track information. The contents of the fields will be saved
in the extended attributes of the file.
The new information will be only saved after pressing the :hp2.Save:ehp2.
button.
:nt.
Extended attributes are only supported with OS/2 and they're not stored on CD. So
if you copy the files to CD or another operating system they will be lost.
:ent.
:p.
The following information may be specified. The length of each field is restricted to 256 characters. 
:ul.
:li.:hp2.Name&colon.:ehp2.
:p.The name of the track.

:li.:hp2.Artist&colon.:ehp2.
:p.The name of the Artist.

:li.:hp2.Album&colon.:ehp2.
:p.The album containing this track.

:li.:hp2.Comment&colon.:ehp2.
:p.An arbitrary comment.

:li.:hp2.Year&colon.:ehp2.
:p.The year the album was published.

:li.:hp2.Genre&colon.:ehp2.
:p.The Genre of this track.

:eul.

:h1 res=020.Help for Ogg audio file
:p.
This file is an Ogg audio file. It may be played using MMOS/2
after installing of MMIOVORB a free multimedia IO procedure.
Ogg files contain comressed audio data similar to MP3 files.
The compression is not lossless.
:p.
Get the IO procedure from
:p.
:link reftype=launch
object='netscape.exe'
data='http://www.math.berkeley.edu/~roconnor/'.
http&colon.//www.math.berkeley.edu/~roconnor/:elink.


:h1 res=030.Help for CD track
:p.
This object is a CD audio track. Track names and other info is
queried from a FreeDB server if an internet connection is available.
You have to specify your email address to use FreeDB. Use the configuration
object to do so.
:p.
The user information is stored in the cddb.ini file in the installation directory.


:h1 res=040.Help for M3U playlist
:p.
This object is a M3U playlist. An M3U playlist is a text file which contains
paths to the tracks to be played. These playlists are used e.g. by portable
MP3 players. An editor is available to modify M3U files using the mouse (drag and drop).

:h1 res=050.Help for Audio stream
:p.
This object plays a stream from a streaming server on the internet.
The URL for the server can be specified using the properties notebook.

:nt.
Only Icecast server are supported. Servers using the shoutcast protocol won't work.
:ent.

:euserdoc.
