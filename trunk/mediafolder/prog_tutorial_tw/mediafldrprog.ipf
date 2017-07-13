:userdoc.

:docprof.

:title.�h�C�����O���{���]�p���� 

:h1 res=100.�e��
:p.
Media-Folde ���ѤG�جɭ��ӱ���ΰ��{���]�p�C�A�i�H�ϥ� :link reftype=hd res=200. �]�w�r�Ŧ�:elink.�Ӱ���¦���u�@�A���O�Ұʤΰ����A�άO�� :link reftype=hd res=300.REXX �ɭ�:elink.�ӵn�����O��k�C
:p.
�ϥ� REXX �ɭ�
:p.
:link reftype=launch
object='netscape.exe'
data='http://www.os2world.com/cdwriting'.
WPS-Wizard V0.2.0:elink. (http&colon.//www.os2world.com/cdwriting)
:p.
�H�W���������w�ˡC 

:h1 res=200
x=left y=bottom width=50%.�]�w�r�Ŧ�
:p.
�H�U���]�w�r�Ŧ�Ҥ䴩�C�ӴC���Ƨ��C

:ul.
:li.:link reftype=hd res=210.MEDIAFLDRNEXT:elink.   �b��Ƨ�������U�@����
:li.:link reftype=hd res=211.MEDIAFLDRPLAY:elink.   �}�l����
:li.:link reftype=hd res=215.MEDIAFLDRPLAYFIRST:elink.   ����Ĥ@������
:li.:link reftype=hd res=212.MEDIAFLDRPREV:elink.   ���L�ܫe�@����
:li.:link reftype=hd res=216.MEDIAFLDRREPEAT:elink. �]�w���м���
:li.:link reftype=hd res=213.MEDIAFLDRSTOP:elink.   �����
:li.:link reftype=hd res=214.MEDIAFLDRVOLUME:elink. �]�w���q
:eul.
:p.
��Ƨ������}�Ҥ~��ϥγo�ǳ]�w�r�Ŧ�C
:p.
MM audio ���O�䴩�H�U���r�Ŧ�G
:ul.
:li.:link reftype=hd res=220.MMAUDIOTRACKNAME:elink.   �]�w���T�ɪ����ئW�١C
:li.:link reftype=hd res=221.MMAUDIOARTIST:elink.   �]�w���T�ɪ��t�X�̡C
:li.:link reftype=hd res=222.MMAUDIOALBUM:elink.    �]�w�M��W�١C
:li.:link reftype=hd res=223.MMAUDIOCOMMENT:elink.  �����T�ɳ]�w���ѡC
:li.:link reftype=hd res=224.MMAUDIOYEAR:elink.     �]�w���T�ɪ��~���C
:li.:link reftype=hd res=225.MMAUDIOGENRE:elink.    �]�w���T�ɪ���������C
:li.:link reftype=hd res=226.MMREFRESH:elink.       ���sŪ�����T�ɪ���T�C
:li.:link reftype=hd res=227.MMAUDIOCOPYTAGTO:elink.    �ƻs���T�ɪ��Ҧ� tags�C

:eul.


:h2 res=210
x=50% width=50%
group=2
.MEDIAFLDRNEXT
:p.
����ɸ���U�@���ءC�Y�|������h�Ĥ@���ط|�Q����C
:xmp.
/* REXX - This command skips to the next track */

rc=SysSetObjectData('x&colon.\path_to_folder','MEDIAFLDRNEXT=1')
:exmp.

:h2 res=211
x=50% width=50%
group=2
.MEDIAFLDRPLAY
:p.
�q�Ĥ@���ض}�l����C�Y����h�Ӧ��ثh�Q������Ĥ@�Ӧ��ط|������C
:xmp.
/* REXX - This command starts playing */

rc=SysSetObjectData('x&colon.\path_to_folder','MEDIAFLDRPLAY=1')
:exmp.

:h2 res=215
x=50% width=50%
group=2
.MEDIAFLDRPLAYFIRST
:p.
����Ĥ@�����ءA���ެO�_�����L���ءC

:xmp.
/* REXX - This command starts playing */

rc=SysSetObjectData('x&colon.\path_to_folder','MEDIAFLDRPLAYFIRST=1')
:exmp.


:h2 res=212
x=50% width=50%
group=2
.MEDIAFLDRPREV
:p.
����ɸ���e�@���ءC
:xmp.
/* REXX - This command skips to the previous track */

rc=SysSetObjectData('x&colon.\path_to_folder','MEDIAFLDRPREV=1')
:exmp.

:h2 res=216
x=50% width=50%
group=2
.MEDIAFLDRREPEAT
:p.
�]�w�����ݩʡC�Y�]�� 1 �h��Ƨ��������ط|���м���A�Y�]�� 0 �h�����м���C
:xmp.
/* REXX - This command sets repeat to yes */

rc=SysSetObjectData('x&colon.\path_to_folder','MEDIAFLDRREPEAT=1')
:exmp.

:h2 res=213
x=50% width=50%
group=2
.MEDIAFLDRSTOP
:p.
�����
:xmp.
/* REXX - This command stops playing */

rc=SysSetObjectData('x&colon.\path_to_folder','MEDIAFLDRSTOP=1')
:exmp.

:h2 res=214
x=50% width=50%
group=2
.MEDIAFLDRVOLUME
:p.
��ﭵ�q�]0 ~ 100%�^�C
:xmp.
/* REXX - This command sets the volume to 35% */

rc=SysSetObjectData('x&colon.\path_to_folder','MEDIAFLDRVOLUME=35')
:exmp.

.********************************************

:h2 res=220
x=50% width=50%
group=2
.MMAUDIOTRACKNAME
:p.
�]�w���T�ɮת����ئW�١C�o�Ӹ�T�|�x�s�b�ɮת������ݩʤ��C�� MP3 �ɮצӨ��A�o�Ӹ�T id �]�|�Q�x�s�b ID3 tag ���C
:xmp.
/* REXX - This command adds the track name to the audio file */

rc=SysSetObjectData('x&colon.\path_to_folder\audio_file.ext','MMAUDIOTRACKNAME=The track name')
:exmp.

:h2 res=221
x=50% width=50%
group=2
.MMAUDIOARTIST
:p.
�]�w���T�ɮת��t�X�̦W�١C�o�Ӹ�T�|�x�s�b�ɮת������ݩʤ��C�� MP3 �ɮצӨ��A�o�Ӹ�T id �]�|�Q�x�s�b ID3 tag ���C
:xmp.
/* REXX - This command adds the artist name to the audio file */

rc=SysSetObjectData('x&colon.\path_to_folder\audio_file.ext','MMAUDIOARTIST=Artist name')
:exmp.

:h2 res=222
x=50% width=50%
group=2
.MMAUDIOALBUM
:p.
�]�w���T�ɮת��M��W�١C�o�Ӹ�T�|�x�s�b�ɮת������ݩʤ��C�� MP3 �ɮצӨ��A�o�Ӹ�T id �]�|�Q�x�s�b ID3 tag ���C
:xmp.
/* REXX - This command adds the album name to the audio file */

rc=SysSetObjectData('x&colon.\path_to_folder\audio_file.ext','MMAUDIOALBUM=Album name')
:exmp.

:h2 res=223
x=50% width=50%
group=2
.MMAUDIOCOMMENT
:p.
�]�w���T�ɮת����ѡC�o�Ӹ�T�|�x�s�b�ɮת������ݩʤ��C�� MP3 �ɮצӨ��A�o�Ӹ�T id �]�|�Q�x�s�b ID3 tag ���C
:xmp.
/* REXX - This command adds a comment to the audio file */

rc=SysSetObjectData('x&colon.\path_to_folder\audio_file.ext','MMAUDIOCOMMENT=Track comment')
:exmp.

:h2 res=224
x=50% width=50%
group=2
.MMAUDIOYEAR
:p.
�w���T�ɮת��~���C�o�Ӹ�T�|�x�s�b�ɮת������ݩʤ��C�� MP3 �ɮצӨ��A�o�Ӹ�T id �]�|�Q�x�s�b ID3 tag ���C
:xmp.
/* REXX - This command adds the year to the audio file */

rc=SysSetObjectData('x&colon.\path_to_folder\audio_file.ext','MMAUDIOYEAR=2003')
:exmp.

:h2 res=225
x=50% width=50%
group=2
.MMAUDIOGENRE
:p.
�w���T�ɮת����������C�o�Ӹ�T�|�x�s�b�ɮת������ݩʤ��C�� MP3 �ɮצӨ��A�o�Ӹ�T id �]�|�Q�x�s�b ID3 tag ���C
:xmp.
/* REXX - This command adds the genre of the audio file */

rc=SysSetObjectData('x&colon.\path_to_folder\audio_file.ext','MMAUDIOGENRE=Blues')
:exmp.

:h2 res=226
x=50% width=50%
group=2
.MMREFRESH
:p.
�q�sŪ���ɮת����T��T�A�ç�s�����������ơC
:xmp.
/* REXX - This command adds the genre of the audio file */

rc=SysSetObjectData('x&colon.\path_to_folder\audio_file.ext','MMREFRESH=1')
:exmp.

:h2 res=227
x=50% width=50%
group=2
.MMAUDIOCOPYTAGTO
:p.
�q�sŪ���ɮת����T��T�A�ç�s�����������ơC
:xmp.
/* REXX - This command adds the genre of the audio file */

rc=SysSetObjectData('x&colon.\path_to_folder\audio_file.ext','MMAUDIOCOPYTAGTO=x&colon.\path_to_folder\another_file.ext')
:exmp.


.********************************************

:h1 res=300.REXX interface
:p.
�ϥ� REXX �ɭ����y�z�y���ӵn���C���Ƨ������O��k�C�䴩�F WPS�Bhelper �\��Τ@�ǥ� WPS-Wizard �Ҵ��Ѫ� PM �����\�઺�̭��n��k�C

:p.
�ϥΥ]�˨禡�]wrapper function�^�ɷ|�I�s�C���Ƨ���k�C

:table cols='20 35'.
:row.
:c.:link reftype=hd res=320.MFldrCallFunc:elink.
:c.�I�s�ѴC���Ƨ��ҾɤJ����k�C
:etable.
:p.
�ϥΥH�U�\��ɷ|�I�s�Ѧh�C�����O�ҾɤJ����k�C
:table cols='20 35'.
:row.
:c.:link reftype=hd res=330.MMCallFunc:elink.
:c.�I�s�Ѧh�C�����O�]CWAudio, CWMP3, CWOGG�^�ҾɤJ����k�C
:etable.

:p.
�b WPS �t���]PM �{���^�����y�z�{���|�b�ӧO�������������C�A�]�i��ϥ��B�~�� REXX dlls�CREXX �y�z�{�����W�٤����n�A���O�ɮץ����㦳���Ѥ~��ҰʡC

.*************************************

.***********************************************

:h2 res=320
width=30%
.MFldrCallFunc
:link reftype=hd res=321
auto dependent group=2.
:p.
�ϥγo�ӥ\��өI�s�@�ӷs�C���Ƨ���k�� helper ��k�C
:p.
:link reftype=hd res=321 dependent.�y�k:elink.
.*.br
.*:link reftype=hd res=322.�Ѽ�:elink.
.br
:link reftype=hd res=323 dependent.��^:elink.
.br
:link reftype=hd res=324 dependent.�Ƶ�:elink.
.*.br
.*:link reftype=hd res=325.�ϥ�:elink.
.br
:link reftype=hd res=326.�d��:elink.

:h2 res=321
x=30%
width=70%
group=2
hide
.MFldrCallFunc - �y�k
:xmp.
/* Call a method from REXX */

methodName  /* The name of a method */
Object         /* Object on which the method is invoked */
param1         /* First method parameter */
param2         /* Second method parameter */
:p.
rc=MFldrCallFunc(methodName, Object, 
                 param1, param2, ...);
:exmp.


:h2 res=322
x=30%
width=70%
group=2
hide
.MFldrCallFunc - �Ѽ�
:p.

:h2 res=323
x=30%
width=70% height=35%
group=3
hide
.MFldrCallFunc - ��^��
:p.
��^�Ȩ��M��Q�I�s����k�A�i��O�@�Ӧr�Ŧ�Τ@�ӼƭȡC�Ա��Ьݤ�k�y�z�C
:p.
�Y�䤣���k�h�|��^ :hp2.ERROR_METHOD&colon.:ehp2. �r�Ŧ�C

:h2 res=324
x=30%
width=70%
group=2
hide
.MFldrCallFunc - �Ƶ�
:p.
�Q�I�s����k�����O :hp2.Object:ehp2. ���󪺦����C�Y���󤣤䴩�ҭn�D����k�N�|�o�Ϳ��~�C�Ҧp CWMediaFolder ���󤣤䴩�p�P CWAudio ���󪺤�k�C

:h2 res=325
x=30%
width=70%
group=2
hide
.MFldrCallFunc - �ϥ�
:p.

:h2 res=326
x=30%
width=70%
group=2
hide
.MFldrCallFunc - �d��
:xmp.
/* This example queries the first object in  */
/* a CWMediaFolder, gets the linked file and */
/* queries the name of the artist from the   */
/* ID3 tag.                                  */

/* Get the first object of folder theFolder */
theObject=MFldrCallWPSFunc("wpQueryContent", theFolder, 0, QC_FIRST)

/* Get the linked file system object from the shadow.
   cwGetFileSystemObject is a member of the
   CWMediaFolder class. */
theFileObj=MFldrCallFunc("cwGetFileSystemObject", theFolder, theObject)

/* Get the artist name. cwmmQueryTrackInfo is a member of the
   CWAudio class. Every object in the CWMediaFolder is usually
   a shadow to an object instance of a subclass of CWAudio */
theName=MFldrCallFunc("cwmmQueryTrackInfo", theFileObj, 2)

/* Display the name */
rc=RxMessageBox("The artist name : "||theName)
:exmp.

.*********************************************
.***********************************************

:h2 res=330
width=30%
.MMCallFunc
:link reftype=hd res=331
auto dependent group=2.
:p.
�ϥγo�ӥ\��өI�s�P :hp2.CWAudio/MMAudio:ehp2. �@�P�Q�ɤJ���s�h�C�����O�C
:p.
:link reftype=hd res=331 dependent.�y�k:elink.
.*.br
.*:link reftype=hd res=332.�Ѽ�:elink.
.br
:link reftype=hd res=333 dependent.��^:elink.
.br
:link reftype=hd res=334 dependent.�Ƶ�:elink.
.*.br
.*:link reftype=hd res=335.�ϥ�:elink.
.br
:link reftype=hd res=336.�d��:elink.

:h2 res=331
x=30%
width=70%
group=2
hide
.MMCallFunc - �y�k
:xmp.
/* Call a method from REXX */

methodName  /* The name of a method */
Object         /* Object on which the method is invoked */
param1         /* First method parameter */
param2         /* Second method parameter */
:p.
rc=MMCallFunc(methodName, Object, 
                 param1, param2, ...);
:exmp.


:h2 res=332
x=30%
width=70%
group=2
hide
.MMCallFunc - �Ѽ�
:p.

:h2 res=333
x=30%
width=70% height=35%
group=3
hide
.MMCallFunc - ��^��
:p.
��^�Ȩ��M��Q�I�s����k�A�i��O�@�Ӧr�Ŧ�Τ@�ӼƭȡC�Ա��Ьݤ�k�y�z�C
:p.
�Y�䤣���k�h�|��^ :hp2.ERROR_METHOD&colon.:ehp2. �r�Ŧ�C

:h2 res=334
x=30%
width=70%
group=2
hide
.MMCallFunc - �Ƶ�
:p.
�Q�I�s����k�����O :hp2.Object:ehp2. ���󪺦����C�Y���󤣤䴩�ҭn�D����k�N�|�o�Ϳ��~�C�Ҧp CWMediaFolder ���󤣤䴩�p�P CWAudio ���󪺤�k�C
:p.
���\��b���e�������O�٬� :hp2.MMClsCallFunc:ehp2.�C���䴩���W�٤��L�w�Q depracted�C

:h2 res=335
x=30%
width=70%
group=2
hide
.MMCallFunc - �ϥ�
:p.

:h2 res=336
x=30%
width=70%
group=2
hide
.MMCallFunc - �d��
:xmp.
/* This example queries the first object in  */
/* a CWMediaFolder, gets the linked file and */
/* queries the name of the artist from the   */
/* ID3 tag.                                  */

/* Get the first object of folder theFolder */
theObject=WPSWizCallWPSFunc("wpQueryContent", theFolder, 0, QC_FIRST)

/* Get the linked file system object from the shadow.
   cwGetFileSystemObject is a member of the
   CWMediaFolder class. */
theFileObj=MFldrCallFunc("cwGetFileSystemObject", theFolder, theObject)

/* Get the artist name. cwmmQueryTrackInfo is a member of the
   CWAudio class. Every object in the CWMediaFolder is usually
   a shadow to an object instance of a subclass of CWAudio */
theName=MMCallFunc("cwmmQueryTrackInfo", theFileObj, 2)

/* Display the name */
rc=RxMessageBox("The artist name : "||theName)
:exmp.


.im wpsmethods.inc

:h1 res=400
x=left y=bottom width=50%.���ö���
:p.
�H�U�� CW-multimedia ���O���إi�H�b :hp2.OS2.INI:ehp2. ���]�w�@�ǭȨӱҥΡC
:ul.
:li.:link reftype=hd res=410.���N���T����:elink.
:li.:link reftype=hd res=411.���N���T�s�边:elink.
:li.:link reftype=hd res=412.���N AVI ����:elink.
:li.:link reftype=hd res=413.���N MPG ����:elink.
:li.:link reftype=hd res=419.���N MP3 ����:elink.
:li.:link reftype=hd res=414.���N�v���s�边:elink.
:li.:link reftype=hd res=415.�b CD �W�ҥμv�����ӥ\��:elink.
:li.:link reftype=hd res=416.���ιϰO���D�C�W���ɶ����:elink.
:li.:link reftype=hd res=417.���μ��񱱨�{�t�]����B�Ȱ��^:elink.
:li.:link reftype=hd res=418.���n�N��v�إߨ�C�鼽�񾹼Ҫ�:elink.
:li.:link reftype=hd res=420.���έ��T�ɮת��ˬd:elink.
:li.:link reftype=hd res=421.���� ID3 tag ��Ū��:elink.
:eul.

.im hiddenfeatures.inc

:h1 res=500.�����ݩ�
:p.
���T�ɮת���T�|�x�s�b�ɮת������ݩʤ��A�i�H�Q����{���� REXX �ҨϥΡC�ثe�Q�w�q�������ݩʦp�U�C

:table cols='20 35'.
:row.
:c.MMALBUM
:c.�����ة��ݪ��M��
:row.
:c.MMARTIST
:c.�t�X�̩m�W
:row.
:c.MMBPS
:c.Bits per sample
:row.
:c.MMCHANNELS
:c.�n�D�ƥ�
:row.
:c.MMCOMMENT
:c.���ɮת�����
:row.
:c.MMGENRE
:c.�����T�ɮת���������
:row.
:c.MMNUMAUDIOBYTES
:c.�b�����T�ɮפ������T bytes �ƥ�
:row.
:c.MMPLAYTIME
:c.����ɶ��]��^
:row.
:c.MMPLAYTIMEMS
:c.����ɶ��]�@��^
:row.
:c.MMSAMPLERATE
:c.���˳t��
:row.
:c.MMTRACKNAME
:c.���ئW��
:row.
:c.MMYEAR
:c.�~��
:etable.

:nt.
��ƬO�H ASCII �榡�x�s�C
:ent.
:euserdoc.
