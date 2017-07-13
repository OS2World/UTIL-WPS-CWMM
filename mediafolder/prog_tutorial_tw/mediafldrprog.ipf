:userdoc.

:docprof.

:title.多媒體類別的程式設計導覽 

:h1 res=100.前言
:p.
Media-Folde 提供二種界面來控制及做程式設計。你可以使用 :link reftype=hd res=200. 設定字符串:elink.來做基礎的工作，像是啟動及停止播放，或是用 :link reftype=hd res=300.REXX 界面:elink.來登錄類別方法。
:p.
使用 REXX 界面
:p.
:link reftype=launch
object='netscape.exe'
data='http://www.os2world.com/cdwriting'.
WPS-Wizard V0.2.0:elink. (http&colon.//www.os2world.com/cdwriting)
:p.
以上都必須先安裝。 

:h1 res=200
x=left y=bottom width=50%.設定字符串
:p.
以下的設定字符串皆支援每個媒體資料夾。

:ul.
:li.:link reftype=hd res=210.MEDIAFLDRNEXT:elink.   在資料夾中播放下一曲目
:li.:link reftype=hd res=211.MEDIAFLDRPLAY:elink.   開始播放
:li.:link reftype=hd res=215.MEDIAFLDRPLAYFIRST:elink.   播放第一首曲目
:li.:link reftype=hd res=212.MEDIAFLDRPREV:elink.   略過至前一曲目
:li.:link reftype=hd res=216.MEDIAFLDRREPEAT:elink. 設定重覆播放
:li.:link reftype=hd res=213.MEDIAFLDRSTOP:elink.   停止播放
:li.:link reftype=hd res=214.MEDIAFLDRVOLUME:elink. 設定音量
:eul.
:p.
資料夾必須開啟才能使用這些設定字符串。
:p.
MM audio 類別支援以下的字符串：
:ul.
:li.:link reftype=hd res=220.MMAUDIOTRACKNAME:elink.   設定音訊檔的曲目名稱。
:li.:link reftype=hd res=221.MMAUDIOARTIST:elink.   設定音訊檔的演出者。
:li.:link reftype=hd res=222.MMAUDIOALBUM:elink.    設定專輯名稱。
:li.:link reftype=hd res=223.MMAUDIOCOMMENT:elink.  為音訊檔設定註解。
:li.:link reftype=hd res=224.MMAUDIOYEAR:elink.     設定音訊檔的年份。
:li.:link reftype=hd res=225.MMAUDIOGENRE:elink.    設定音訊檔的類型風格。
:li.:link reftype=hd res=226.MMREFRESH:elink.       重新讀取音訊檔的資訊。
:li.:link reftype=hd res=227.MMAUDIOCOPYTAGTO:elink.    複製音訊檔的所有 tags。

:eul.


:h2 res=210
x=50% width=50%
group=2
.MEDIAFLDRNEXT
:p.
播放時跳到下一曲目。若尚未播放則第一曲目會被播放。
:xmp.
/* REXX - This command skips to the next track */

rc=SysSetObjectData('x&colon.\path_to_folder','MEDIAFLDRNEXT=1')
:exmp.

:h2 res=211
x=50% width=50%
group=2
.MEDIAFLDRPLAY
:p.
從第一曲目開始播放。若選取多個曲目則被選取的第一個曲目會先播放。
:xmp.
/* REXX - This command starts playing */

rc=SysSetObjectData('x&colon.\path_to_folder','MEDIAFLDRPLAY=1')
:exmp.

:h2 res=215
x=50% width=50%
group=2
.MEDIAFLDRPLAYFIRST
:p.
播放第一首曲目，不管是否選取其他曲目。

:xmp.
/* REXX - This command starts playing */

rc=SysSetObjectData('x&colon.\path_to_folder','MEDIAFLDRPLAYFIRST=1')
:exmp.


:h2 res=212
x=50% width=50%
group=2
.MEDIAFLDRPREV
:p.
播放時跳到前一曲目。
:xmp.
/* REXX - This command skips to the previous track */

rc=SysSetObjectData('x&colon.\path_to_folder','MEDIAFLDRPREV=1')
:exmp.

:h2 res=216
x=50% width=50%
group=2
.MEDIAFLDRREPEAT
:p.
設定重覆屬性。若設為 1 則資料夾中的曲目會重覆播放，若設為 0 則不重覆播放。
:xmp.
/* REXX - This command sets repeat to yes */

rc=SysSetObjectData('x&colon.\path_to_folder','MEDIAFLDRREPEAT=1')
:exmp.

:h2 res=213
x=50% width=50%
group=2
.MEDIAFLDRSTOP
:p.
停止播放
:xmp.
/* REXX - This command stops playing */

rc=SysSetObjectData('x&colon.\path_to_folder','MEDIAFLDRSTOP=1')
:exmp.

:h2 res=214
x=50% width=50%
group=2
.MEDIAFLDRVOLUME
:p.
更改音量（0 ~ 100%）。
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
設定音訊檔案的曲目名稱。這個資訊會儲存在檔案的延伸屬性中。對 MP3 檔案而言，這個資訊 id 也會被儲存在 ID3 tag 中。
:xmp.
/* REXX - This command adds the track name to the audio file */

rc=SysSetObjectData('x&colon.\path_to_folder\audio_file.ext','MMAUDIOTRACKNAME=The track name')
:exmp.

:h2 res=221
x=50% width=50%
group=2
.MMAUDIOARTIST
:p.
設定音訊檔案的演出者名稱。這個資訊會儲存在檔案的延伸屬性中。對 MP3 檔案而言，這個資訊 id 也會被儲存在 ID3 tag 中。
:xmp.
/* REXX - This command adds the artist name to the audio file */

rc=SysSetObjectData('x&colon.\path_to_folder\audio_file.ext','MMAUDIOARTIST=Artist name')
:exmp.

:h2 res=222
x=50% width=50%
group=2
.MMAUDIOALBUM
:p.
設定音訊檔案的專輯名稱。這個資訊會儲存在檔案的延伸屬性中。對 MP3 檔案而言，這個資訊 id 也會被儲存在 ID3 tag 中。
:xmp.
/* REXX - This command adds the album name to the audio file */

rc=SysSetObjectData('x&colon.\path_to_folder\audio_file.ext','MMAUDIOALBUM=Album name')
:exmp.

:h2 res=223
x=50% width=50%
group=2
.MMAUDIOCOMMENT
:p.
設定音訊檔案的註解。這個資訊會儲存在檔案的延伸屬性中。對 MP3 檔案而言，這個資訊 id 也會被儲存在 ID3 tag 中。
:xmp.
/* REXX - This command adds a comment to the audio file */

rc=SysSetObjectData('x&colon.\path_to_folder\audio_file.ext','MMAUDIOCOMMENT=Track comment')
:exmp.

:h2 res=224
x=50% width=50%
group=2
.MMAUDIOYEAR
:p.
定音訊檔案的年份。這個資訊會儲存在檔案的延伸屬性中。對 MP3 檔案而言，這個資訊 id 也會被儲存在 ID3 tag 中。
:xmp.
/* REXX - This command adds the year to the audio file */

rc=SysSetObjectData('x&colon.\path_to_folder\audio_file.ext','MMAUDIOYEAR=2003')
:exmp.

:h2 res=225
x=50% width=50%
group=2
.MMAUDIOGENRE
:p.
定音訊檔案的風格類型。這個資訊會儲存在檔案的延伸屬性中。對 MP3 檔案而言，這個資訊 id 也會被儲存在 ID3 tag 中。
:xmp.
/* REXX - This command adds the genre of the audio file */

rc=SysSetObjectData('x&colon.\path_to_folder\audio_file.ext','MMAUDIOGENRE=Blues')
:exmp.

:h2 res=226
x=50% width=50%
group=2
.MMREFRESH
:p.
從新讀取檔案的音訊資訊，並更新內部的物件資料。
:xmp.
/* REXX - This command adds the genre of the audio file */

rc=SysSetObjectData('x&colon.\path_to_folder\audio_file.ext','MMREFRESH=1')
:exmp.

:h2 res=227
x=50% width=50%
group=2
.MMAUDIOCOPYTAGTO
:p.
從新讀取檔案的音訊資訊，並更新內部的物件資料。
:xmp.
/* REXX - This command adds the genre of the audio file */

rc=SysSetObjectData('x&colon.\path_to_folder\audio_file.ext','MMAUDIOCOPYTAGTO=x&colon.\path_to_folder\another_file.ext')
:exmp.


.********************************************

:h1 res=300.REXX interface
:p.
使用 REXX 界面的描述語言來登錄媒體資料夾的類別方法。支援了 WPS、helper 功能及一些由 WPS-Wizard 所提供的 PM 視窗功能的最重要方法。

:p.
使用包裝函式（wrapper function）時會呼叫媒體資料夾方法。

:table cols='20 35'.
:row.
:c.:link reftype=hd res=320.MFldrCallFunc:elink.
:c.呼叫由媒體資料夾所導入的方法。
:etable.
:p.
使用以下功能時會呼叫由多媒體類別所導入的方法。
:table cols='20 35'.
:row.
:c.:link reftype=hd res=330.MMCallFunc:elink.
:c.呼叫由多媒體類別（CWAudio, CWMP3, CWOGG）所導入的方法。
:etable.

:p.
在 WPS 系絡（PM 程式）中的描述程式會在個別的執行緒中執行。你也可能使用額外的 REXX dlls。REXX 描述程式的名稱不重要，但是檔案必須具有註解才能啟動。

.*************************************

.***********************************************

:h2 res=320
width=30%
.MFldrCallFunc
:link reftype=hd res=321
auto dependent group=2.
:p.
使用這個功能來呼叫一個新媒體資料夾方法或 helper 方法。
:p.
:link reftype=hd res=321 dependent.語法:elink.
.*.br
.*:link reftype=hd res=322.參數:elink.
.br
:link reftype=hd res=323 dependent.返回:elink.
.br
:link reftype=hd res=324 dependent.備註:elink.
.*.br
.*:link reftype=hd res=325.使用:elink.
.br
:link reftype=hd res=326.範例:elink.

:h2 res=321
x=30%
width=70%
group=2
hide
.MFldrCallFunc - 語法
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
.MFldrCallFunc - 參數
:p.

:h2 res=323
x=30%
width=70% height=35%
group=3
hide
.MFldrCallFunc - 返回值
:p.
返回值取決於被呼叫的方法，可能是一個字符串或一個數值。詳情請看方法描述。
:p.
若找不到方法則會返回 :hp2.ERROR_METHOD&colon.:ehp2. 字符串。

:h2 res=324
x=30%
width=70%
group=2
hide
.MFldrCallFunc - 備註
:p.
被呼叫的方法必須是 :hp2.Object:ehp2. 物件的成員。若物件不支援所要求的方法就會發生錯誤。例如 CWMediaFolder 物件不支援如同 CWAudio 物件的方法。

:h2 res=325
x=30%
width=70%
group=2
hide
.MFldrCallFunc - 使用
:p.

:h2 res=326
x=30%
width=70%
group=2
hide
.MFldrCallFunc - 範例
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
使用這個功能來呼叫與 :hp2.CWAudio/MMAudio:ehp2. 一同被導入的新多媒體類別。
:p.
:link reftype=hd res=331 dependent.語法:elink.
.*.br
.*:link reftype=hd res=332.參數:elink.
.br
:link reftype=hd res=333 dependent.返回:elink.
.br
:link reftype=hd res=334 dependent.備註:elink.
.*.br
.*:link reftype=hd res=335.使用:elink.
.br
:link reftype=hd res=336.範例:elink.

:h2 res=331
x=30%
width=70%
group=2
hide
.MMCallFunc - 語法
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
.MMCallFunc - 參數
:p.

:h2 res=333
x=30%
width=70% height=35%
group=3
hide
.MMCallFunc - 返回值
:p.
返回值取決於被呼叫的方法，可能是一個字符串或一個數值。詳情請看方法描述。
:p.
若找不到方法則會返回 :hp2.ERROR_METHOD&colon.:ehp2. 字符串。

:h2 res=334
x=30%
width=70%
group=2
hide
.MMCallFunc - 備註
:p.
被呼叫的方法必須是 :hp2.Object:ehp2. 物件的成員。若物件不支援所要求的方法就會發生錯誤。例如 CWMediaFolder 物件不支援如同 CWAudio 物件的方法。
:p.
此功能在之前釋放的類別稱為 :hp2.MMClsCallFunc:ehp2.。仍支援此名稱不過已被 depracted。

:h2 res=335
x=30%
width=70%
group=2
hide
.MMCallFunc - 使用
:p.

:h2 res=336
x=30%
width=70%
group=2
hide
.MMCallFunc - 範例
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
x=left y=bottom width=50%.隱藏項目
:p.
以下的 CW-multimedia 類別項目可以在 :hp2.OS2.INI:ehp2. 中設定一些值來啟用。
:ul.
:li.:link reftype=hd res=410.取代音訊播放器:elink.
:li.:link reftype=hd res=411.取代音訊編輯器:elink.
:li.:link reftype=hd res=412.取代 AVI 播放器:elink.
:li.:link reftype=hd res=413.取代 MPG 播放器:elink.
:li.:link reftype=hd res=419.取代 MP3 播放器:elink.
:li.:link reftype=hd res=414.取代影像編輯器:elink.
:li.:link reftype=hd res=415.在 CD 上啟用影像明細功能:elink.
:li.:link reftype=hd res=416.停用圖記標題列上的時間顯示:elink.
:li.:link reftype=hd res=417.停用播放控制的閃礫（播放、暫停）:elink.
:li.:link reftype=hd res=418.不要將投影建立到媒體播放器模版:elink.
:li.:link reftype=hd res=420.停用音訊檔案的檢查:elink.
:li.:link reftype=hd res=421.停用 ID3 tag 的讀取:elink.
:eul.

.im hiddenfeatures.inc

:h1 res=500.延伸屬性
:p.
音訊檔案的資訊會儲存在檔案的延伸屬性中，可以被其住的程式或 REXX 所使用。目前被定義的延伸屬性如下。

:table cols='20 35'.
:row.
:c.MMALBUM
:c.此曲目所屬的專輯
:row.
:c.MMARTIST
:c.演出者姓名
:row.
:c.MMBPS
:c.Bits per sample
:row.
:c.MMCHANNELS
:c.聲道數目
:row.
:c.MMCOMMENT
:c.此檔案的註解
:row.
:c.MMGENRE
:c.此音訊檔案的風格類型
:row.
:c.MMNUMAUDIOBYTES
:c.在此音訊檔案中的音訊 bytes 數目
:row.
:c.MMPLAYTIME
:c.播放時間（秒）
:row.
:c.MMPLAYTIMEMS
:c.播放時間（毫秒）
:row.
:c.MMSAMPLERATE
:c.取樣速度
:row.
:c.MMTRACKNAME
:c.曲目名稱
:row.
:c.MMYEAR
:c.年份
:etable.

:nt.
資料是以 ASCII 格式儲存。
:ent.
:euserdoc.
