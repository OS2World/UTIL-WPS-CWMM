.******************************************************

:h2 res=1010
width=30%
.cwGetFileSystemObject
:link reftype=hd res=1011
auto dependent group=2.
:p.
:link reftype=hd res=1011.語法:elink.
.br
:link reftype=hd res=1013 dependent.返回:elink.
.br
:link reftype=hd res=1014 dependent.備註:elink.
.*.br
.*:link reftype=hd res=315.使用:elink.
.*.br
.*:link reftype=hd res=416 dependent.範例:elink.

:h2 res=1011
x=30%
width=70%
hide
group=2
.cwGetFileSystemObject - 語法
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
.cwGetFileSystemObject - 返回值
:p.
:hp2.fsObj:ehp2. numeric
:p.
返回值是投影 :hp2.theShadow:ehp2. 所連結的檔案系統物件的指標。若連結失敗則會返回 0 值。

:h2 res=1014
x=30%
width=70%
hide
group=2
.cwGetFileSystemObject - 備註
:p.
這個方法是媒體資料夾的成員，所以必須提供傳給媒體資料夾的指標。
:p.
這個方法是由 CWProgFolder 類別（這是幾個新資料夾類別的母類別）所導入的。所以你也可以將指標傳給 CWProgFolder 的其他子類別。下面表列一些子類別。

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
:link reftype=hd res=1041.語法:elink.
.br
:link reftype=hd res=1043 dependent.返回:elink.
.br
:link reftype=hd res=1044 dependent.備註:elink.
.*.br
.*:link reftype=hd res=315.使用:elink.
.*.br
.*:link reftype=hd res=416 dependent.範例:elink.

:h2 res=1041
x=30%
width=70%
hide
group=2
.cwPauseAudioFile - 語法
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
.cwPauseAudioFile - 返回值
:p.
:hp2.rc:ehp2. 數值
:p.


:h2 res=1044
x=30%
width=70%
hide
group=2
.cwPauseAudioFile - 備註
:p.
媒體資料夾的 frame handle 會傳給描述語言以做為啟動時的參數。

.******************************************************
:h2 res=1050
width=30%
.cwPlayAudioFile
:link reftype=hd res=1051
auto dependent group=2.
:p.
:link reftype=hd res=1051.語法:elink.
.br
:link reftype=hd res=1053 dependent.返回:elink.
.br
:link reftype=hd res=1054 dependent.備註:elink.
.*.br
.*:link reftype=hd res=315.使用:elink.
.*.br
.*:link reftype=hd res=416 dependent.範例:elink.

:h2 res=1051
x=30%
width=70%
hide
group=2
.cwPlayAudioFile - 語法
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
.cwPlayAudioFile - 返回值
:p.
:hp2.rc:ehp2. 數值
:p.


:h2 res=1054
x=30%
width=70%
hide
group=2
.cwPlayAudioFile - 備註
:p.
媒體資料夾的 frame handle 會傳給描述語言以做為啟動時的參數。旗標參數可能有以下的值。

:ul.
:li.PLAY_FIRST    1
.br
開啟媒體資料夾中的第一個音訊曲目。若有選取曲目，則先播放所選取的第一個曲目。
:li.PLAY_NEXT     2
.br
播放下一曲目
:li.PLAY_PREV     3
.br
播放前一道曲目。
:eul.


.******************************************************
:h2 res=1060
width=30%
.cwQueryCurrentlyPlayingObject
:link reftype=hd res=1061
auto dependent group=2.
:p.
:link reftype=hd res=1061.語法:elink.
.br
:link reftype=hd res=1063 dependent.返回:elink.
.*.br
.*:link reftype=hd res=1054 dependent.備註:elink.
.*.br
.*:link reftype=hd res=315.使用:elink.
.*.br
.*:link reftype=hd res=416 dependent.範例:elink.

:h2 res=1061
x=30%
width=70%
hide
group=2
.cwQueryCurrentlyPlayingObject - 語法
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
.cwQueryCurrentlyPlayingObject - 返回值
:p.
:hp2.rc:ehp2. numeric
:p.
物件指標傳給目前正在播放的物件，若沒播放則是 0。


.******************************************************
:h2 res=1090
width=30%
.cwSetStatusLineText
:link reftype=hd res=1091
auto dependent group=2.
:p.
:link reftype=hd res=1091.語法:elink.
.br
:link reftype=hd res=1093 dependent.返回:elink.
.br
:link reftype=hd res=1094 dependent.備註:elink.
.*.br
.*:link reftype=hd res=315.使用:elink.
.*.br
.*:link reftype=hd res=416 dependent.範例:elink.

:h2 res=1091
x=30%
width=70%
hide
group=2
.cwSetStatusLineText - 語法
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
.cwSetStatusLineText - 返回值
:p.
:hp2.info:ehp2. 字串
:p.


:h2 res=1094
x=30%
width=70%
hide
group=2
.cwSetStatusLineText - 備註
:p.
若文字參數是 0，則資料夾會顯示其標準狀態的 line text。


.******************************************************
:h2 res=1100
width=30%
.cwStopAudioFile
:link reftype=hd res=1101
auto dependent group=2.
:p.
:link reftype=hd res=1101.語法:elink.
.br
:link reftype=hd res=1103 dependent.返回:elink.
.br
:link reftype=hd res=1104 dependent.備註:elink.
.*.br
.*:link reftype=hd res=315.使用:elink.
.*.br
.*:link reftype=hd res=416 dependent.範例:elink.

:h2 res=1101
x=30%
width=70%
hide
group=2
.cwStopAudioFile - 語法
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
.cwStopAudioFile - 返回值
:p.
:hp2.rc:ehp2. 數值
:p.


:h2 res=1104
x=30%
width=70%
hide
group=2
.cwStopAudioFile - 備註
:p.
媒體資料夾的 frame handle 會傳給描述語言以做為啟動時的參數。


.******************************************************
:h2 res=1070
width=30%
.cwmmQueryTrackInfo
:link reftype=hd res=1071
auto dependent group=2.
:p.
:link reftype=hd res=1071.語法:elink.
.br
:link reftype=hd res=1073 dependent.返回:elink.
.br
:link reftype=hd res=1074 dependent.備註:elink.
.*.br
.*:link reftype=hd res=315.使用:elink.
.*.br
.*:link reftype=hd res=416 dependent.範例:elink.

:h2 res=1071
x=30%
width=70%
hide
group=2
.cwmmQueryTrackInfo - 語法
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
.cwmmQueryTrackInfo - 返回值
:p.
:hp2.info:ehp2. string
:p.
要求的資訊會以字串返回。若無資訊需求則會傳回空字串。
:p.
若有錯誤發生，則傳回 :hp2.ERROR&colon.:ehp2.。

:h2 res=1074
x=30%
width=70%
hide
group=2
.cwmmQueryTrackInfo - 備註
:p.
物件必須是 :hp2.CWAudio:ehp2. 的實體（instance），或是其子類別之一（:hp2.CWMP3:ehp2., :hp2.MMWAV:ehp2....）。這個方法是由 :hp2.CWAudio:ehp2. 所導入。從 V0.2.5 版起，類別名稱為 :hp2.MMAudio:ehp2.、:hp2.MMWAV:ehp2. 等等，都能與 OS/2 中的 IBM 類別相容。
:p.
此旗標參數可能有以下的值。
:ul.
:li.IDINFO_NAME              1
.br
曲目名稱
:li.IDINFO_ARTIST            2
.br
演出者名稱
:li.IDINFO_ALBUM             3
.br
專輯名稱
:li.IDINFO_YEAR              4
.br
ID3 tag 中的年份
:li.IDINFO_COMMENT           5
.br
註解
:li.IDINFO_GENRE             6
.br
以風格類型作為字串
:li.IDINFO_PLAYTIME          7
.br
播放時間（秒，數值）
:li.IDINFO_BPS               8
.br
Bits per sample (數值).
:li.IDINFO_SAMPLERATE        9
.br
取樣頻率（Hz，數值）
:li.IDINFO_CHANNELS          10
.br
Number of channels (numeric value).
:li.IDINFO_BITRATE           11
.br
Bitrate in Hz (數值)。這個旗標只對 MMMP3 及 MMOGG 的類別實體有用，其他類別的返回值無法被定義。這是 V0.2.7 版的新功能。
:eul.


.******************************************************
:h2 res=1080
width=30%
.cwmmQueryImageInfo
:link reftype=hd res=1081
auto dependent group=2.
:p.
:link reftype=hd res=1081.語法:elink.
.br
:link reftype=hd res=1082 dependent.Parameters:elink.

.br
:link reftype=hd res=1083 dependent.返回:elink.
.*.br
.*:link reftype=hd res=1084 dependent.備註:elink.
.*.br
.*:link reftype=hd res=315.使用:elink.
.*.br
.*:link reftype=hd res=416 dependent.範例:elink.

:h2 res=1081
x=30%
width=70%
hide
group=2
.cwmmQueryImageInfo - 語法
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
.cwmmQueryImageInfo - 返回值
:p.
:hp2.info:ehp2. string
:p.
要求的資訊會以字串返回。若無資訊需求則會傳回空字串。
:p.
若有錯誤發生，則傳回 :hp2.ERROR&colon.:ehp2.。

:h2 res=1082
x=30%
width=70%
hide
group=2
.cwmmQueryImageInfo - 參數
:p.
:parml tsize=10.
:pt.:hp2.imageObject:ehp2. (數值)
:pd.影像檔案的物件指標。
:pt.:hp2.flag:ehp2. (numeric)
:pd.決定查詢的資訊。
:parml tsize=3.
:pt.IDINFO_WIDTH    0x0001
:pd.影像寬度
:pt.IDINFO_HEIGHT   0x0002
:pd.影像長度
:pt.IDINFO_BPP      0x0003  
:pd.每個像素的 bits 數目。
:pt.IDINFO_FORMAT   0x0004  
:pd.影像格式。
:eparml.
:pt.:hp2.info:ehp2. (數值)
:pd.查詢資訊作為字串。
:eparml.
