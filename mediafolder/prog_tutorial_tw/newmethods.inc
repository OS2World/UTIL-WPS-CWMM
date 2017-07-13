.******************************************************

:h2 res=1010
width=30%
.cwGetFileSystemObject
:link reftype=hd res=1011
auto dependent group=2.
:p.
:link reftype=hd res=1011.�y�k:elink.
.br
:link reftype=hd res=1013 dependent.��^:elink.
.br
:link reftype=hd res=1014 dependent.�Ƶ�:elink.
.*.br
.*:link reftype=hd res=315.�ϥ�:elink.
.*.br
.*:link reftype=hd res=416 dependent.�d��:elink.

:h2 res=1011
x=30%
width=70%
hide
group=2
.cwGetFileSystemObject - �y�k
:xmp.
/* Get the file system object a shadow links to */

mediaFolder   /* Pointer to a media folder */
theShadow     /* A shadow object  */
:p.
fsObj=MFldrCallFunc("cwGetFileSystemObject", 
                        mediaFolder, theShadow);
:exmp.


:h2 res=1013
x=30%
width=70% height=35%
group=3
hide
.cwGetFileSystemObject - ��^��
:p.
:hp2.fsObj:ehp2. numeric
:p.
��^�ȬO��v :hp2.theShadow:ehp2. �ҳs�����ɮרt�Ϊ��󪺫��СC�Y�s�����ѫh�|��^ 0 �ȡC

:h2 res=1014
x=30%
width=70%
hide
group=2
.cwGetFileSystemObject - �Ƶ�
:p.
�o�Ӥ�k�O�C���Ƨ��������A�ҥH�������Ѷǵ��C���Ƨ������СC
:p.
�o�Ӥ�k�O�� CWProgFolder ���O�]�o�O�X�ӷs��Ƨ����O�������O�^�ҾɤJ���C�ҥH�A�]�i�H�N���жǵ� CWProgFolder ����L�l���O�C�U�����C�@�Ǥl���O�C

:ul.
:li.CWAudioFolder
:li.CWDataFolder
:li.CWMediaFolder
:eul.



.******************************************************
:h2 res=1040
width=30%
.cwPauseAudioFile
:link reftype=hd res=1041
auto dependent group=2.
:p.
:link reftype=hd res=1041.�y�k:elink.
.br
:link reftype=hd res=1043 dependent.��^:elink.
.br
:link reftype=hd res=1044 dependent.�Ƶ�:elink.
.*.br
.*:link reftype=hd res=315.�ϥ�:elink.
.*.br
.*:link reftype=hd res=416 dependent.�d��:elink.

:h2 res=1041
x=30%
width=70%
hide
group=2
.cwPauseAudioFile - �y�k
:xmp.
/* This method pauses playing */

mediaFolder   /* Pointer to the media folder */
frameHandle   /* Handle of the folder frame  */
:p.
rc=MFldrCallFunc("cwPauseAudioFile", 
                        mediaFolder, frameHandle);
:exmp.


:h2 res=1043
x=30%
width=70% height=35%
group=3
hide
.cwPauseAudioFile - ��^��
:p.
:hp2.rc:ehp2. �ƭ�
:p.


:h2 res=1044
x=30%
width=70%
hide
group=2
.cwPauseAudioFile - �Ƶ�
:p.
�C���Ƨ��� frame handle �|�ǵ��y�z�y���H�����Ұʮɪ��ѼơC

.******************************************************
:h2 res=1050
width=30%
.cwPlayAudioFile
:link reftype=hd res=1051
auto dependent group=2.
:p.
:link reftype=hd res=1051.�y�k:elink.
.br
:link reftype=hd res=1053 dependent.��^:elink.
.br
:link reftype=hd res=1054 dependent.�Ƶ�:elink.
.*.br
.*:link reftype=hd res=315.�ϥ�:elink.
.*.br
.*:link reftype=hd res=416 dependent.�d��:elink.

:h2 res=1051
x=30%
width=70%
hide
group=2
.cwPlayAudioFile - �y�k
:xmp.
/* This method starts playing the contents 
    of the media folder */

mediaFolder   /* Pointer to the media folder */
frameHandle   /* Handle of the folder frame  */
flag          /* Flag specifying which track to start */
:p.
rc=MFldrCallFunc("cwPlayAudioFile", 
                        mediaFolder, frameHandle, flag);
:exmp.

:h2 res=1053
x=30%
width=70% height=35%
group=3
hide
.cwPlayAudioFile - ��^��
:p.
:hp2.rc:ehp2. �ƭ�
:p.


:h2 res=1054
x=30%
width=70%
hide
group=2
.cwPlayAudioFile - �Ƶ�
:p.
�C���Ƨ��� frame handle �|�ǵ��y�z�y���H�����Ұʮɪ��ѼơC�X�аѼƥi�঳�H�U���ȡC

:ul.
:li.PLAY_FIRST    1
.br
�}�ҴC���Ƨ������Ĥ@�ӭ��T���ءC�Y��������ءA�h������ҿ�����Ĥ@�Ӧ��ءC
:li.PLAY_NEXT     2
.br
����U�@����
:li.PLAY_PREV     3
.br
����e�@�D���ءC
:eul.


.******************************************************
:h2 res=1060
width=30%
.cwQueryCurrentlyPlayingObject
:link reftype=hd res=1061
auto dependent group=2.
:p.
:link reftype=hd res=1061.�y�k:elink.
.br
:link reftype=hd res=1063 dependent.��^:elink.
.*.br
.*:link reftype=hd res=1054 dependent.�Ƶ�:elink.
.*.br
.*:link reftype=hd res=315.�ϥ�:elink.
.*.br
.*:link reftype=hd res=416 dependent.�d��:elink.

:h2 res=1061
x=30%
width=70%
hide
group=2
.cwQueryCurrentlyPlayingObject - �y�k
:xmp.
/* This method queries the pointer to the 
   currently playing object */

mediaFolder   /* Pointer to the media folder */
:p.
rc=MFldrCallFunc("cwQueryCurrentlyPlayingObject", 
                  mediaFolder);
:exmp.

:h2 res=1063
x=30%
width=70% height=35%
group=3
hide
.cwQueryCurrentlyPlayingObject - ��^��
:p.
:hp2.rc:ehp2. numeric
:p.
������жǵ��ثe���b���񪺪���A�Y�S����h�O 0�C


.******************************************************
:h2 res=1090
width=30%
.cwSetStatusLineText
:link reftype=hd res=1091
auto dependent group=2.
:p.
:link reftype=hd res=1091.�y�k:elink.
.br
:link reftype=hd res=1093 dependent.��^:elink.
.br
:link reftype=hd res=1094 dependent.�Ƶ�:elink.
.*.br
.*:link reftype=hd res=315.�ϥ�:elink.
.*.br
.*:link reftype=hd res=416 dependent.�d��:elink.

:h2 res=1091
x=30%
width=70%
hide
group=2
.cwSetStatusLineText - �y�k
:xmp.
/* This method sets the status line text of a media folder */

mediaFolder   /* Pointer to a media folder on which the method is invoked */
theText       /* Text for status line or 0 */
:p.
info=MFldrCallFunc("cwSetStatusLineText", 
                    mediaFolder, theText);
:exmp.

:h2 res=1093
x=30%
width=70% height=35%
group=3
hide
.cwSetStatusLineText - ��^��
:p.
:hp2.info:ehp2. �r��
:p.


:h2 res=1094
x=30%
width=70%
hide
group=2
.cwSetStatusLineText - �Ƶ�
:p.
�Y��r�ѼƬO 0�A�h��Ƨ��|��ܨ�зǪ��A�� line text�C


.******************************************************
:h2 res=1100
width=30%
.cwStopAudioFile
:link reftype=hd res=1101
auto dependent group=2.
:p.
:link reftype=hd res=1101.�y�k:elink.
.br
:link reftype=hd res=1103 dependent.��^:elink.
.br
:link reftype=hd res=1104 dependent.�Ƶ�:elink.
.*.br
.*:link reftype=hd res=315.�ϥ�:elink.
.*.br
.*:link reftype=hd res=416 dependent.�d��:elink.

:h2 res=1101
x=30%
width=70%
hide
group=2
.cwStopAudioFile - �y�k
:xmp.
/* This method stops playing */

mediaFolder   /* Pointer to the media folder */
frameHandle   /* Handle of the folder frame  */
:p.
rc=MFldrCallFunc("cwStopAudioFile", 
                        mediaFolder, frameHandle);
:exmp.


:h2 res=1103
x=30%
width=70% height=35%
group=3
hide
.cwStopAudioFile - ��^��
:p.
:hp2.rc:ehp2. �ƭ�
:p.


:h2 res=1104
x=30%
width=70%
hide
group=2
.cwStopAudioFile - �Ƶ�
:p.
�C���Ƨ��� frame handle �|�ǵ��y�z�y���H�����Ұʮɪ��ѼơC


.******************************************************
:h2 res=1070
width=30%
.cwmmQueryTrackInfo
:link reftype=hd res=1071
auto dependent group=2.
:p.
:link reftype=hd res=1071.�y�k:elink.
.br
:link reftype=hd res=1073 dependent.��^:elink.
.br
:link reftype=hd res=1074 dependent.�Ƶ�:elink.
.*.br
.*:link reftype=hd res=315.�ϥ�:elink.
.*.br
.*:link reftype=hd res=416 dependent.�d��:elink.

:h2 res=1071
x=30%
width=70%
hide
group=2
.cwmmQueryTrackInfo - �y�k
:xmp.
/* This method queries information from an
   audio object */

audioObject   /* Pointer to an audio object on which the method is invoked */
flag          /* Flag specifying which info to query */
:p.
info=MMClsCallFunc("cwmmQueryTrackInfo", 
                  audioObject, flag);
:exmp.

:h2 res=1073
x=30%
width=70% height=35%
group=3
hide
.cwmmQueryTrackInfo - ��^��
:p.
:hp2.info:ehp2. string
:p.
�n�D����T�|�H�r���^�C�Y�L��T�ݨD�h�|�Ǧ^�Ŧr��C
:p.
�Y�����~�o�͡A�h�Ǧ^ :hp2.ERROR&colon.:ehp2.�C

:h2 res=1074
x=30%
width=70%
hide
group=2
.cwmmQueryTrackInfo - �Ƶ�
:p.
���󥲶��O :hp2.CWAudio:ehp2. ������]instance�^�A�άO��l���O���@�]:hp2.CWMP3:ehp2., :hp2.MMWAV:ehp2....�^�C�o�Ӥ�k�O�� :hp2.CWAudio:ehp2. �ҾɤJ�C�q V0.2.5 ���_�A���O�W�٬� :hp2.MMAudio:ehp2.�B:hp2.MMWAV:ehp2. �����A����P OS/2 ���� IBM ���O�ۮe�C
:p.
���X�аѼƥi�঳�H�U���ȡC
:ul.
:li.IDINFO_NAME              1
.br
���ئW��
:li.IDINFO_ARTIST            2
.br
�t�X�̦W��
:li.IDINFO_ALBUM             3
.br
�M��W��
:li.IDINFO_YEAR              4
.br
ID3 tag �����~��
:li.IDINFO_COMMENT           5
.br
����
:li.IDINFO_GENRE             6
.br
�H���������@���r��
:li.IDINFO_PLAYTIME          7
.br
����ɶ��]���A�ƭȡ^
:li.IDINFO_BPS               8
.br
Bits per sample (�ƭ�).
:li.IDINFO_SAMPLERATE        9
.br
�����W�v�]Hz�A�ƭȡ^
:li.IDINFO_CHANNELS          10
.br
Number of channels (numeric value).
:li.IDINFO_BITRATE           11
.br
Bitrate in Hz (�ƭ�)�C�o�ӺX�Хu�� MMMP3 �� MMOGG �����O���馳�ΡA��L���O����^�ȵL�k�Q�w�q�C�o�O V0.2.7 �����s�\��C
:eul.


.******************************************************
:h2 res=1080
width=30%
.cwmmQueryImageInfo
:link reftype=hd res=1081
auto dependent group=2.
:p.
:link reftype=hd res=1081.�y�k:elink.
.br
:link reftype=hd res=1082 dependent.Parameters:elink.

.br
:link reftype=hd res=1083 dependent.��^:elink.
.*.br
.*:link reftype=hd res=1084 dependent.�Ƶ�:elink.
.*.br
.*:link reftype=hd res=315.�ϥ�:elink.
.*.br
.*:link reftype=hd res=416 dependent.�d��:elink.

:h2 res=1081
x=30%
width=70%
hide
group=2
.cwmmQueryImageInfo - �y�k
:xmp.
/* This method queries information from an
   image object */

imageObject   /* Pointer to an image object on which the method is invoked */
flag          /* Flag specifying which info to query */
:p.
info=MMCallFunc("cwmmQueryTrackInfo", 
                  imageObject, flag);
:exmp.

:h2 res=1083
x=30%
width=70% height=35%
group=3
hide
.cwmmQueryImageInfo - ��^��
:p.
:hp2.info:ehp2. string
:p.
�n�D����T�|�H�r���^�C�Y�L��T�ݨD�h�|�Ǧ^�Ŧr��C
:p.
�Y�����~�o�͡A�h�Ǧ^ :hp2.ERROR&colon.:ehp2.�C

:h2 res=1082
x=30%
width=70%
hide
group=2
.cwmmQueryImageInfo - �Ѽ�
:p.
:parml tsize=10.
:pt.:hp2.imageObject:ehp2. (�ƭ�)
:pd.�v���ɮת�������СC
:pt.:hp2.flag:ehp2. (numeric)
:pd.�M�w�d�ߪ���T�C
:parml tsize=3.
:pt.IDINFO_WIDTH    0x0001
:pd.�v���e��
:pt.IDINFO_HEIGHT   0x0002
:pd.�v������
:pt.IDINFO_BPP      0x0003  
:pd.�C�ӹ����� bits �ƥءC
:pt.IDINFO_FORMAT   0x0004  
:pd.�v���榡�C
:eparml.
:pt.:hp2.info:ehp2. (�ƭ�)
:pd.�d�߸�T�@���r��C
:eparml.