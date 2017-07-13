:h2 res=410
x=50% width=50%
group=2
.取代聲音播放器
:p.
MMOS2 隨附的預設聲音播放器會因為加入下面的 key 到 OS2.INI 而被取代：
:p.
:hp2.CWMM->聲音播放器-><path to new player>:ehp2.
:nt.若程式在你的 PATH 設定中，則你不需要提供完整的路徑。必須是 PM 程式！
:ent.
:p.
即使新程式沒被指定在 INI 檔案中，類別也會提供預設的行為，不需要開啟播放器來做為 WPS 處理的一部份。故若播放器當掉，WPS 並不會當掉。

:h2 res=411
x=50% width=50%
group=2
.取代音訊編輯器
:p.
MMOS2 隨附的預設音訊編輯器會因為加入下面的 key 到 OS2.INI 而被取代：
:p.
:hp2.CWMM->音訊編輯器-><path to new editor>:ehp2.
:nt.若程式在你的 PATH 設定中，則你不需要提供完整的路徑。必須是 PM 程式！
:ent.
:p.
即使新程式沒被指定在 INI 檔案中，類別也會提供預設的行為，不需要開啟編輯器來做為 WPS 處理的一部份。故若編輯器當掉，WPS 並不會當掉。

:h2 res=412
x=50% width=50%
group=2
.取代 AVI 播放器
:p.
MMOS2 隨附的預設 AVI 播放器會因為加入下面的 key 到 OS2.INI 而被取代：
:p.
:hp2.CWMM->aviplayer-><path to new player>:ehp2.
:nt.若程式在你的 PATH 設定中，則你不需要提供完整的路徑。必須是 PM 程式！
:ent.
:p.
即使新程式沒被指定在 INI 檔案中，類別也會提供預設的行為，不需要開啟播放器來做為 WPS 處理的一部份。故若播放器當掉，WPS 並不會當掉。

:h2 res=413
x=50% width=50%
group=2
.取代 MPG 播放器
:p.
MMOS2 隨附的預設 MPG 播放器會因為加入下面的 key 到 OS2.INI 而被取代：
:p.
:hp2.CWMM->mpgplayer-><path to new player>:ehp2.
:nt.若程式在你的 PATH 設定中，則你不需要提供完整的路徑。必須是 PM 程式！
:ent.
:p.
即使新程式沒被指定在 INI 檔案中，類別也會提供預設的行為，不需要開啟播放器來做為 WPS 處理的一部份。故若播放器當掉，WPS 並不會當掉。

:h2 res=419
x=50% width=50%
group=2
.取代 MP3 播放器
:p.
預設的 MP3 播放器會因為加入下面的 key 到 OS2.INI 而被取代：
:p.
:hp2.CWMM->mp3player-><path to new player>:ehp2.
:nt.若程式在你的 PATH 設定中，則你不需要提供完整的路徑。必須是 PM 程式！
:ent.
:p.
即使新程式沒被指定在 INI 檔案中，類別也會提供預設的行為，不需要開啟播放器來做為 WPS 處理的一部份。故若播放器當掉，WPS 並不會當掉。

:h2 res=422
x=50% width=50%
group=2
.取代 M3U 編輯器
:p.
預設的 M3U 編輯器會因為加入下面的 key 到 OS2.INI 而被取代：
:p.
:hp2.CWMM->m3ueditor-><path to new editor>:ehp2.
:nt.若程式在你的 PATH 設定中，則你不需要提供完整的路徑。必須是 PM 程式！
:ent.

:h2 res=423
x=50% width=50%
group=2
.取代 M3U 播放器
:p.
預設的 M3U 播放器會因為加入下面的 key 到 OS2.INI 而被取代：
:p.
:hp2.CWMM->m3uplayer-><path to new player>:ehp2.
:nt.若程式在你的 PATH 設定中，則你不需要提供完整的路徑。必須是 PM 程式！
:ent.
:p.
預設行為是建立一個新的 MEdia 資料夾並加入指定於 M3U 檔案中所指定的曲目。使用這個設定還是有可能其他的 MP3 播放器。

:h2 res=414
x=50% width=50%
group=2
.取代影像編輯器
:p.
MMOS2 隨附的預設影像編輯器會因為加入下面的 key 到 OS2.INI 而被取代：
:p.
:hp2.CWMM->imageeditor-><path to new editor>:ehp2.
:nt.若程式在你的 PATH 設定中，則你不需要提供完整的路徑。必須是 PM 程式！
:ent.
:p.
即使新程式沒被指定在 INI 檔案中，類別也會提供預設的行為，不需要開啟播放器來做為 WPS 處理的一部份。故若播放器當掉，WPS 並不會當掉。


:h2 res=415
x=50% width=50%
group=2
.在 CD 上啟用影像明細功能
:p.
預設設定是不支援在 CD 上啟用影像明細功能。理由是這會降低光碟機收集資訊的速度，因這很花時間。你可以加入以下的 key 到 OS2.INI 來開啟這個新的類別功能：
:p.
:hp2.CWMM->useNewImgClsFeaturesOnCD->1:ehp2.

:nt.注意需使用大寫字母！
:ent.
:nt.你必須重新啟動 WPS 才能讓設定生效。
:ent.


:h2 res=416
x=50% width=50%
group=2
.關閉時間顯示在圖記標題
:p.
當利用雙擊開啟音訊檔案時，會有經過時間顯示在圖記標題上。要關閉這個新設定可以將下面的 key 加到 OS2.INI 中。
:p.
:hp2.CWMM->mmClsNoExtendedIconTitles->1:ehp2.

:nt.注意需使用大寫字母！
:ent.
:nt.你必須重新啟動 WPS 才能讓設定生效。
:ent.

:h2 res=417
x=50% width=50%
group=2
.播放時關閉播放控制閃爍
:p.
當曲目播放時會有可見的回饋播放控制（播放、暫停）。要關閉這個新設定可以將下面的 key 加到 OS2.INI 中。
:p.
:hp2.CWMM->bNoPlayCtrlBlinking->1:ehp2.

:nt.注意需使用大寫字母！
:ent.
:nt.你必須重新啟動 WPS 才能讓設定生效。
:ent.

:h2 res=418
x=50% width=50%
group=2
.避免將投影建到媒體播放模版
:p.
從多媒體資料夾中所做的多媒體播放模版投影，會自動建在多媒體資料夾中來做早期的登錄。設定這個 key 後此功能就會被關閉。

:p.
:hp2.CWMM->bNoMediaFolderTemplateShadow->1:ehp2.

:nt.注意需使用大寫字母！
:ent.

:nt.你可以將建立的投影移到另一個地方。若它從系統上被刪除，會在多媒體資料夾中重新建立。
:ent.


:h2 res=420
x=50% width=50%
group=2
.關閉音訊檔案的檢查
:p.
你可以將磁碟代號加到 OS2.INI 中，就可以關閉音訊檔案的檢查功能。
:p.
:hp2.CWMM->mmClsDontCheckDrives-><drive list>:ehp2.
:nt.注意需使用大寫字母！
:ent.

這對連線速度較慢或是其他較慢的裝置來說是很有用的。正常來說，收集到的資訊是儲存在音訊檔案的延伸屬性中，所以只會檢查一次。若執行中的檔案系統不支援延伸屬性，你可以將此磁碟加到清單中以避免去掃瞄。
:nt.

需提醒的是，指定在清單中的裝置，其聲音物件的音訊資訊不會被 WPS 所顯示。
:ent.
:nt.
必須重新啟動 WPS 才能讓設定生效。
:ent.

:p.
例子：
:xmp.
CWMM->mmClsDontCheckDrives->abcghzy
:exmp.

:nt.
這個設定目前已過時了，因為 MMIOMP3 IO 處理包含在:hp2.MMAudio Pak 2 中，:ehp2.（2003.08.30 釋出），不會再掃瞄 MP3 檔案但是會讀取 VBR 編碼檔案的 header。
:ent.


:h2 res=421
x=50% width=50%
group=2
.關閉 ID3 tag 的讀取
:p.
你可以將磁碟代號加到 OS2.INI 中，就可以關閉 ID3 tag 的讀取功能。
:p.
:hp2.CWMM->mmClsDontReadID3OnDrives-><drive list>:ehp2.
:nt.注意需使用大寫字母！
:ent.

這對連線速度較慢或是其他較慢的裝置來說是很有用的。

:nt.
需提醒的是，指定在清單中的裝置，其 MP3 檔案的 ID3 tag 資訊不會被 WPS 所顯示。
:ent.
:nt.
必須重新啟動 WPS 才能讓設定生效。
:ent.

:p.
例子：
:xmp.
CWMM->mmClsDontReadID3OnDrives->abcghzy
:exmp.

