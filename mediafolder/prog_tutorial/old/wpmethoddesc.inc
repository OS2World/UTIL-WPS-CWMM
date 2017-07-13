.******************************************************

:h2 res=450
width=30%
.wpClose
:link reftype=hd res=451
auto dependent group=2.
:p.
:link reftype=hd res=451.Syntax:elink.
.br
:link reftype=hd res=453 dependent.Returns:elink.
.*.br
.*:link reftype=hd res=314 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=315.Usage:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h2 res=451
x=30%
width=70%
hide
group=2
.wpClose - Syntax
:xmp.
/* Close all open views of an object */

theObject         /* WPS-object  */
:p.
rc=MFldrCallWPSFunc("wpClose", theObject);
:exmp.


:h2 res=453
x=30%
width=70% height=35%
group=3
hide
.wpClose - Return value
:p.
:hp2.rc:ehp2. numeric
:p.
The return value is 0 if an error occured.

.******************************************************

:h2 res=460
width=30%
.wpHide
:link reftype=hd res=461
auto dependent group=2.
:p.
:link reftype=hd res=461.Syntax:elink.
.br
:link reftype=hd res=463 dependent.Returns:elink.
.*.br
.*:link reftype=hd res=314 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=315.Usage:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h2 res=461
x=30%
width=70%
hide
group=2
.wpHide - Syntax
:xmp.
/* Hide all open views of an object */

theObject         /* WPS-object  */
:p.
rc=MFldrCallWPSFunc("wpHide", theObject);
:exmp.


:h2 res=463
x=30%
width=70% height=35%
group=3
hide
.wpHide - Return value
:p.
:hp2.rc:ehp2. numeric
:p.
The return value is 0 if an error occured.

.******************************************************

:h2 res=470
width=30%
.wpIsDeleteable
:link reftype=hd res=471
auto dependent group=2.
:p.
:link reftype=hd res=471.Syntax:elink.
.br
:link reftype=hd res=473 dependent.Returns:elink.
.*.br
.*:link reftype=hd res=314 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=315.Usage:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h2 res=471
x=30%
width=70%
hide
group=2
.wpIsDeleteable - Syntax
:xmp.
/* Check if an object is deleteable */

theObject         /* The WPS-object to be checked */
:p.
rc=MFldrCallWPSFunc("wpIsDeleteable", theObject);
:exmp.


:h2 res=473
x=30%
width=70% height=35%
group=3
hide
.wpIsDeleteable - Return value
:p.
:hp2.rc:ehp2. numeric
:p.
The return value is 0 if not deletable.

.******************************************************

:h2 res=480
width=30%
.wpQueryAttr
:link reftype=hd res=481
auto dependent group=2.
:p.
:link reftype=hd res=481.Syntax:elink.
.br
:link reftype=hd res=483 dependent.Returns:elink.
.*.br
.*:link reftype=hd res=314 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=315.Usage:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h2 res=481
x=30%
width=70%
hide
group=2
.wpQueryAttr - Syntax
:xmp.
/* Query the file attributes of an object */

theObject         /* The WPS-object to be checked
                     Must be of class WPFileSystem  */
:p.
rc=MFldrCallWPSFunc("wpQueryAttr", theObject);
:exmp.


:h2 res=483
x=30%
width=70% height=35%
group=3
hide
.wpQueryAttr - Return value
:p.
:hp2.rc:ehp2. numeric
:p.
The return value is a bit mask of the file attributes.


.******************************************************

:h2 res=490
width=30%
.wpQueryConcurrentView
:link reftype=hd res=491
auto dependent group=2.
:p.
:link reftype=hd res=491.Syntax:elink.
.br
:link reftype=hd res=493 dependent.Returns:elink.
.*.br
.*:link reftype=hd res=314 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=315.Usage:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h2 res=491
x=30%
width=70%
hide
group=2
.wpQueryConcurrentView - Syntax
:xmp.
/* Query the concurrent view behavior of an object */

theObject         /* The WPS-object to be checked */
:p.
ulView=MFldrCallWPSFunc("wpQueryConcurrentView", 
                         theObject);
:exmp.


:h2 res=493
x=30%
width=70% height=55%
group=3
hide
.wpQueryConcurrentView - Return value
:p.
:hp2.ulView:ehp2. numeric
:p.
The return value is one of the following:

:ul.
:li.CCVIEW_DEFAULT: 0
.br
Concurrent view is set to system default.
:li.CCVIEW_ON:      1
.br
Concurrent view is off.
:li.CCVIEW_OFF:     2
.br
Concurrent view is off.
:eul.



.******************************************************

:h2 res=500
width=30%
.wpQueryConfirmations
:link reftype=hd res=501
auto dependent group=2.
:p.
:link reftype=hd res=501.Syntax:elink.
.br
:link reftype=hd res=503 dependent.Returns:elink.
.*.br
.*:link reftype=hd res=314 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=315.Usage:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h2 res=501
x=30%
width=70%
hide
group=2
.wpQueryConfirmations - Syntax
:xmp.
/* Query which confirmations are set for this
   object */

theObject         /* The WPS-object on which the method is invoked */
:p.
ulFlag=MFldrCallWPSFunc("wpQueryConfirmations", theObject);
:exmp.


:h2 res=503
x=30%
width=70% height=55%
group=3
hide
.wpQueryConfirmations - Return value
:p.
:hp2.ulFlag:ehp2. numeric
:p.
The return value is one of the following.
:ul.
:li.0
.br
No confirmation set.
:li.CONFIRM_DELETE             0x0001
.br
Prompt for all objects.
:li.CONFIRM_DELETEFOLDER       0x0002
.br
Prompt for folders only.
:li.CONFIRM_RENAMEFILESWITHEXT 0x0004
.br
Prompt when renaming files with extension.
:li.CONFIRM_KEEPASSOC          0x0008
.br
Prompt if file to be renamed has an association depending on the extension.
:li.CONFIRM_ACTION             0x0010
.br
Prompt when moving, copying or creating shadows.
:li.CONFIRM_PROGRESS           0x0020
.br
Display a dialog box when performing a copy, move etc.
:li.CONFIRM_ALLOWFORCEDELETE   0x0040
.br
Allow forced delete. Must be set when CONFIRM_FORCEDELETE is set.
:li.CONFIRM_FORCEDELETE        0x0080
.br
Prompt for confirmation of forced deletion.
:eul.

The values are returned as decimal numbers.


.******************************************************

:h2 res=510
width=30%
.wpQueryContent
:link reftype=hd res=511
auto dependent group=2.
:p.
:link reftype=hd res=511.Syntax:elink.
.br
:link reftype=hd res=513 dependent.Returns:elink.
.br
:link reftype=hd res=514 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=315.Usage:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h2 res=511
x=30%
width=70%
hide
group=2
.wpQueryContent - Syntax
:xmp.
/* Query the contents of a folder */

theFolder         /* The folder to be checked */
theObject         /* Previous object or NULL */
option            /* option, which object to query 
                     (QC_FIRST, QC_NEXT, QC_LAST)
:p.
object=MFldrCallWPSFunc("wpQueryContent", 
                         theObject, option);
:exmp.


:h2 res=513
x=30%
width=70% height=55%
group=3
hide
.wpQueryContent - Return value
:p.
:hp2.object:ehp2. numeric
:p.
Pointer to the requested object.


:h2 res=514
x=30%
width=70% 
group=2
hide
.wpQueryContent - Remarks
:p.
Specify QC_FIRST (0) to get the first object in the folder, QC_LAST (2) for the last. When
using QC_NEXT (1) then :hp2.theObject:ehp2. points to the previous object otherwise this
parameter is ignored.
:p.
The order of the objects returned is the order in the internal list of the folder. It's no
necessarily the order in which the objects appear in the container.  



.******************************************************

:h2 res=520
width=30%
.wpQueryDefaultView
:link reftype=hd res=521
auto dependent group=2.
:p.
:link reftype=hd res=521.Syntax:elink.
.br
:link reftype=hd res=523 dependent.Returns:elink.
.*.br
.*:link reftype=hd res=314 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=315.Usage:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h2 res=521
x=30%
width=70%
hide
group=2
.wpQueryDefaultView - Syntax
:xmp.
/* Query the default view of an object */

theObject         /* The WPS-object to be checked */
:p.
ulView=MFldrCallWPSFunc("wpQueryDefaultView", 
                         theObject);
:exmp.


:h2 res=523
x=30%
width=70% height=55%
group=3
hide
.wpQueryDefaultView - Return value
:p.
:hp2.ulView:ehp2. numeric
:p.
The return value is one of the following:

:ul.
:li.OPEN_CONTENTS: 1
.br
Open in contents view (icon).
:li.OPEN_DEFAULT:  0
.br
Same as doubleclick.
:li.OPEN_DETAILS:  102
.br
Details view.
:li.OPEN_HELP:     3  
.br
Display the help panel.
:li.OPEN_RUNNING:  4  
.br
Execute the object.
:li.OPEN_SETTINGS: 2  
.br
Open the settings of the object.
:li.OPEN_TREE:     101
.br
Open the tree view.
:li.OPEN_UNKNOWN:  -1  
.br
Unknown view.
:li.OPEN_USER:     0x6500 or higher
.br
A user view introduced by a subclass.
:eul.



.******************************************************

:h2 res=530
width=30%
.wpQueryDisk
:link reftype=hd res=531
auto dependent group=2.
:p.
:link reftype=hd res=531.Syntax:elink.
.br
:link reftype=hd res=533 dependent.Returns:elink.
.*.br
.*:link reftype=hd res=314 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=315.Usage:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h2 res=531
x=30%
width=70%
hide
group=2
.wpQueryDisk - Syntax
:xmp.
/* Query the disk object a file system
   object is located on */

theObject         /* The file system object to be checked */
:p.
Disk=MFldrCallWPSFunc("wpQueryDisk", 
                         theObject);
:exmp.


:h2 res=533
x=30%
width=70% height=55%
group=3
hide
.wpQueryDisk - Return value
:p.
:hp2.Disk:ehp2. numeric
:p.
The return value is a pointer to the disk object of the drive the file system object is
located on.

.******************************************************

:h2 res=540
width=30%
.wpQueryEASize
:link reftype=hd res=541
auto dependent group=2.
:p.
:link reftype=hd res=541.Syntax:elink.
.br
:link reftype=hd res=543 dependent.Returns:elink.
.*.br
.*:link reftype=hd res=314 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=315.Usage:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h2 res=541
x=30%
width=70%
hide
group=2
.wpQueryEASize - Syntax
:xmp.
/* Query the size of the EAs of a file system
   object */

theObject         /* The file system object  */
:p.
eaSize=MFldrCallWPSFunc("wpQueryEASize", 
                         theObject);
:exmp.


:h2 res=543
x=30%
width=70% height=55%
group=3
hide
.wpQueryEASize - Return value
:p.
:hp2.eaSize:ehp2. numeric
:p.
The return value is the size of the extended attributes of a file.


.******************************************************

:h2 res=550
width=30%
.wpQueryFileSize
:link reftype=hd res=551
auto dependent group=2.
:p.
:link reftype=hd res=551.Syntax:elink.
.br
:link reftype=hd res=553 dependent.Returns:elink.
.*.br
.*:link reftype=hd res=314 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=315.Usage:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h2 res=551
x=30%
width=70%
hide
group=2
.wpQueryFileSize - Syntax
:xmp.
/* Query the size file */

theObject         /* The file system object  */
:p.
size=MFldrCallWPSFunc("wpQueryFileSize", 
                         theObject);
:exmp.


:h2 res=553
x=30%
width=70% height=55%
group=3
hide
.wpQueryFileSize - Return value
:p.
:hp2.size:ehp2. numeric
:p.
The return value is the size of the file.

.******************************************************

:h2 res=560
width=30%
.wpQueryFldrFlags
:link reftype=hd res=561
auto dependent group=2.
:p.
:link reftype=hd res=561.Syntax:elink.
.br
:link reftype=hd res=563 dependent.Returns:elink.
.*.br
.*:link reftype=hd res=314 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=315.Usage:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h2 res=561
x=30%
width=70%
hide
group=2
.wpQueryFldrFlags - Syntax
:xmp.
/* Query the flags of a folder */

theFolder         /* The folder object  */
:p.
flags=MFldrCallWPSFunc("wpQueryFldrFlags", 
                         theFolder);
:exmp.


:h2 res=563
x=30%
width=70% height=55%
group=3
hide
.wpQueryFldrFlags - Return value
:p.
:hp2.flags:ehp2. numeric
:p.
The return value is a bitmask of the following.

:ul.
:li.FOI_POPULATEDWITHALL     0x0001
.br
The folder is populated with all objects.
:li.FOI_POPULATEDWITHFOLDERS 0x0002
.br
The folder is only populated with folders it contains.
:li.FOI_WORKAREA             0x0004
.br
The folder is a workarea folder.
:eul.

.******************************************************

:h2 res=570
width=30%
.wpQueryFldrFont
:link reftype=hd res=571
auto dependent group=2.
:p.
:link reftype=hd res=571.Syntax:elink.
.br
:link reftype=hd res=573 dependent.Returns:elink.
.br
:link reftype=hd res=574 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=315.Usage:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h2 res=571
x=30%
width=70%
hide
group=2
.wpQueryFldrFont - Syntax
:xmp.
/* Query the font of a folder view */

thefolder         /* The folder on which the method is invoked */
fldrView          /* One of OPEN_CONTENTS (1), OPEN_DETAILS (102) or OPEN_TREE (101) */
:p.
font=MFldrCallWPSFunc("wpQueryFldrFont", 
                         theFolder, fldrView);
:exmp.


:h2 res=573
x=30%
width=70% height=55%
group=3
hide
.wpQueryFldrFont - Return value
:p.
:hp2.font:ehp2. string
:p.
The return value is font of the specified folder view for example :hp2.10.Helv:ehp2..


:h2 res=574
x=30%
width=70%
hide
group=2
.wpQueryFldrFont - Remarks
:p.
There's only one font per view. All concurrent views share the same font.


.******************************************************

:h2 res=580
width=30%
.wpQueryFolder
:link reftype=hd res=581
auto dependent group=2.
:p.
:link reftype=hd res=581.Syntax:elink.
.br
:link reftype=hd res=583 dependent.Returns:elink.
.*.br
.*:link reftype=hd res=314 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=315.Usage:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h2 res=581
x=30%
width=70%
hide
group=2
.wpQueryFolder - Syntax
:xmp.
/* Query the folder an  objects resides in */

theObject         /* A WPS object  */
:p.
theFolder=MFldrCallWPSFunc("wpQueryFolder", 
                         theObject);
:exmp.


:h2 res=583
x=30%
width=70% height=55%
group=3
hide
.wpQueryFolder - Return value
:p.
:hp2.theFolder:ehp2. numeric
:p.
The return value is the folder a given object resides in. The pointer
may be used with other methods.


.******************************************************

:h2 res=590
width=30%
.wpQueryHandle
:link reftype=hd res=591
auto dependent group=2.
:p.
:link reftype=hd res=591.Syntax:elink.
.br
:link reftype=hd res=593 dependent.Returns:elink.
.*.br
.*:link reftype=hd res=314 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=315.Usage:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h2 res=591
x=30%
width=70%
hide
group=2
.wpQueryHandle - Syntax
:xmp.
/* Query the persistent handle of an object */

theObject         /* A WPS object  */
:p.
handle=MFldrCallWPSFunc("wpQueryHandle", 
                         theObject);
:exmp.


:h2 res=593
x=30%
width=70% height=55%
group=3
hide
.wpQueryHandle - Return value
:p.
:hp2.handle:ehp2. numeric
:p.
The return value is a persistent handle to the given object. This
handle is unique on the system.


.******************************************************

:h2 res=600
width=30%
.wpQueryObjectID
:link reftype=hd res=601
auto dependent group=2.
:p.
:link reftype=hd res=601.Syntax:elink.
.br
:link reftype=hd res=603 dependent.Returns:elink.
.*.br
.*:link reftype=hd res=314 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=315.Usage:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h2 res=601
x=30%
width=70%
hide
group=2
.wpQueryObjectID - Syntax
:xmp.
/* Query the ID of an object */

theObject         /* A WPS object  */
:p.
id=MFldrCallWPSFunc("wpQueryObjectID", 
                         theObject);
:exmp.


:h2 res=603
x=30%
width=70% height=55%
group=3
hide
.wpQueryObjectID - Return value
:p.
:hp2.id:ehp2. string
:p.
The return value is the ID of the given object or NULL. This
ID is unique on the system for example <WP_DESKTOP>.

.******************************************************

:h2 res=610
width=30%
.wpQueryShadowedObject
:link reftype=hd res=611
auto dependent group=2.
:p.
:link reftype=hd res=611.Syntax:elink.
.br
:link reftype=hd res=613 dependent.Returns:elink.
.br
:link reftype=hd res=614 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=315.Usage:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h2 res=611
x=30%
width=70%
hide
group=2
.wpQueryShadowedObject - Syntax
:xmp.
/* Query the object a shadow links to */

theShadow         /* A shadow object  */
fLock             /* lock flag 0/1*/
:p.
object=MFldrCallWPSFunc("wpQueryShadowedObject", 
                         theShadow, fLock);
:exmp.


:h2 res=613
x=30%
width=70% height=55%
group=3
hide
.wpQueryShadowedObject - Return value
:p.
:hp2.object:ehp2. numeric
:p.
The return value is the object the shadow links to.


:h2 res=614
x=30%
width=70%
hide
group=2
.wpQueryShadowedObject - Remarks
:p.
If fLock is 0 the queried object will go dormant when the object and the folder
the object resides in are closed. Otherwise the object stays awake until a 
wpUnlockObject call is made. Awake objects stay in memory while dormant objects release
their resources when not longer accessed.


.******************************************************

:h2 res=620
width=30%
.wpQueryStyle
:link reftype=hd res=621
auto dependent group=2.
:p.
:link reftype=hd res=621.Syntax:elink.
.br
:link reftype=hd res=623 dependent.Returns:elink.
.*.br
.*:link reftype=hd res=614 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=315.Usage:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h2 res=621
x=30%
width=70%
hide
group=2
.wpQueryStyle - Syntax
:xmp.
/* Query the style of an object */

theObject         /* An object  */
:p.
style=MFldrCallWPSFunc("wpQueryStyle", 
                         theObject);
:exmp.


:h2 res=623
x=30%
width=70% height=55%
group=3
hide
.wpQueryStyle - Return value
:p.
:hp2.style:ehp2. numeric
:p.
The return value is a bitmask of the style flags of an object.
:ul.
:li.OBJSTYLE_NOMOVE          2
.br
Object cannot be moved.
:li.OBJSTYLE_NOLINK          4
.br
Shadows can't be created
:li.OBJSTYLE_NOCOPY          8
.br
Object cannot be copied.
:li.OBJSTYLE_TEMPLATE        32
.br
The object is a template.
:li.OBJSTYLE_NODELETE        64
.br
The object cannot be deleted.
:li.OBJSTYLE_NOPRINT         128
.br
The object cannot be printed.
:li.OBJSTYLE_NODRAG          256
.br
The object cannot be dragged.
:li.OBJSTYLE_NOTVISIBLE      512
.br
The object is hidden.
:li.OBJSTYLE_NOSETTINGS      1024
.br
There're not settings for this object.
:li.OBJSTYLE_NORENAME        2048
.br
The object cannot be renamed.
:li.OBJSTYLE_NODROPON        8192
.br
Other objects can't be dropped on this object.
:li.OBJSTYLE_CUSTOMICON      16384
.br
Icon is destroyed when object goes dormant. 
:eul.

.******************************************************

:h2 res=630
width=30%
.wpQueryTitle
:link reftype=hd res=631
auto dependent group=2.
:p.
:link reftype=hd res=631.Syntax:elink.
.br
:link reftype=hd res=633 dependent.Returns:elink.
.br
:link reftype=hd res=634 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=315.Usage:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h2 res=631
x=30%
width=70%
hide
group=2
.wpQueryTitle - Syntax
:xmp.
/* Query the title of an object */

theObject         /* An object  */
:p.
title=MFldrCallWPSFunc("wpQueryTitle", 
                         theObject);
:exmp.


:h2 res=633
x=30%
width=70% height=55%
group=3
hide
.wpQueryTitle - Return value
:p.
:hp2.title:ehp2. string
:p.
This call returns the title of the object as a string.

:h2 res=634
x=30%
width=70%
hide
group=2
.wpQueryTitle - Remarks
:p.
The title may differ from the real name of an object. For example
FAT-drives don't support long file names but the WPS may take
them from the EAs.


.******************************************************

:h2 res=640
width=30%
.wpQueryType
:link reftype=hd res=641
auto dependent group=2.
:p.
:link reftype=hd res=641.Syntax:elink.
.br
:link reftype=hd res=643 dependent.Returns:elink.
.*.br
.*:link reftype=hd res=634 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=315.Usage:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h2 res=641
x=30%
width=70%
hide
group=2
.wpQueryType - Syntax
:xmp.
/* Query the type of an object */

theObject         /* An object  */
:p.
type=MFldrCallWPSFunc("wpQueryType", 
                         theObject);
:exmp.


:h2 res=643
x=30%
width=70% height=55%
group=3
hide
.wpQueryType - Return value
:p.
:hp2.type:ehp2. string
:p.
The return string contains a list of types delimited with linefeed characters.
For example :hp2.Plain Text\nC Code:ehp2..

.******************************************************

:h2 res=650
width=30%
.wpRefresh
:link reftype=hd res=651
auto dependent group=2.
:p.
:link reftype=hd res=651.Syntax:elink.
.br
:link reftype=hd res=653 dependent.Returns:elink.
.*.br
.*:link reftype=hd res=634 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=315.Usage:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h2 res=651
x=30%
width=70%
hide
group=2
.wpRefresh - Syntax
:xmp.
/* Force a file system object to reread its information */

theObject         /* A file system object */
view              /* The view to be refreshed (OPEN_CONTENTS, OPEN_TREE, OPEN_DETAILS or any other defined view) */
reserved          /* reserved parameter, must be 0 */
:p.
rc=MFldrCallWPSFunc("wpRefresh", 
                         theObject, view, reserved);
:exmp.


:h2 res=653
x=30%
width=70% height=55%
group=3
hide
.wpRefresh - Return value
:p.
:hp2.rc:ehp2. numeric
:p.
Returns 0 if an error occurs.


.******************************************************

:h2 res=660
width=30%
.wpSaveDeferred
:link reftype=hd res=661
auto dependent group=2.
:p.
:link reftype=hd res=661.Syntax:elink.
.br
:link reftype=hd res=663 dependent.Returns:elink.
.br
:link reftype=hd res=664 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=315.Usage:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h2 res=661
x=30%
width=70%
hide
group=2
.wpSaveDeferred - Syntax
:xmp.
/* Save the instance data of an object asynchronously */

theObject         /* A WPS object */
:p.
rc=MFldrCallWPSFunc("wpSaveDeferred", 
                         theObject);
:exmp.


:h2 res=663
x=30%
width=70% height=55%
group=3
hide
.wpSaveDeferred - Return value
:p.
:hp2.rc:ehp2. numeric
:p.
Returns 0 if an error occurs.


:h2 res=664
x=30%
width=70%
hide
group=2
.wpSaveDeferred - Remarks
:p.
The instance data is saved on a separate thread in the background when the system is idle.
If it's necessary to save critical data immediately use wpSaveImmediate.



:h2 res=670
width=30%
.wpSaveImmediate
:link reftype=hd res=671
auto dependent group=2.
:p.
:link reftype=hd res=671.Syntax:elink.
.br
:link reftype=hd res=673 dependent.Returns:elink.
.br
:link reftype=hd res=674 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=315.Usage:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h2 res=671
x=30%
width=70%
hide
group=2
.wpSaveImmediate - Syntax
:xmp.
/* Save the instance data of an object synchronously */

theObject         /* A WPS object */
:p.
rc=MFldrCallWPSFunc("wpSaveImmediate", 
                         theObject);
:exmp.


:h2 res=673
x=30%
width=70% height=55%
group=3
hide
.wpSaveImmediate - Return value
:p.
:hp2.rc:ehp2. numeric
:p.
Returns 0 if an error occurs.


:h2 res=674
x=30%
width=70%
hide
group=2
.wpSaveImmediate - Remarks
:p.
The instance data is saved immediate when calling this method. In general
wpSaveDeferred should be used because when using that method the data is written
when the system is idle.

.******************************************************

:h2 res=680
width=30%
.wpSetDefaultView
:link reftype=hd res=681
auto dependent group=2.
:p.
:link reftype=hd res=681.Syntax:elink.
.br
:link reftype=hd res=683 dependent.Returns:elink.
.br
:link reftype=hd res=684 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=315.Usage:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h2 res=681
x=30%
width=70%
hide
group=2
.wpSetDefaultView - Syntax
:xmp.
/* Set the default view of an object */

theObject         /* A WPS object */#
view              /* The new default view */
:p.
rc=MFldrCallWPSFunc("wpSetDefaultView", 
                         theObject, view);
:exmp.


:h2 res=683
x=30%
width=70% height=55%
group=3
hide
.wpSetDefaultView - Return value
:p.
:hp2.rc:ehp2. numeric
:p.
Returns 0 if an error occurs.


:h2 res=684
x=30%
width=70%
hide
group=2
.wpSetDefaultView - Remarks
:p.
The object may be set to one of the following views.

:ul.
:li.OPEN_CONTENTS: 1
.br
Open in contents view (icon).
:li.OPEN_DEFAULT:  0
.br
Same as doubleclick.
:li.OPEN_DETAILS:  102
.br
Details view.
:li.OPEN_HELP:     3  
.br
Display the help panel.
:li.OPEN_RUNNING:  4  
.br
Execute the object.
:li.OPEN_SETTINGS: 2  
.br
Open the settings of the object.
:li.OPEN_TREE:     101
.br
Unknown view.
:li.OPEN_USER:     0x6500 or higher
.br
A user view introduced by a subclass.
:eul.


.******************************************************

:h2 res=690
width=30%
.wpSetLinkToObject
:link reftype=hd res=681
auto dependent group=2.
:p.
:link reftype=hd res=691.Syntax:elink.
.br
:link reftype=hd res=693 dependent.Returns:elink.
.*.br
.*:link reftype=hd res=684 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=315.Usage:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h2 res=691
x=30%
width=70%
hide
group=2
.wpSetLinkToObject - Syntax
:xmp.
/* This method links the shadow to 
   the specified object */

theShadow
theObject         /* A WPS object to link to */
:p.
rc=MFldrCallWPSFunc("wpSetLinkToObject", 
                     theShadow, theObject);
:exmp.


:h2 res=693
x=30%
width=70% height=55%
group=3
hide
.wpSetLinkToObject - Return value
:p.
:hp2.rc:ehp2. numeric
:p.
Returns 0 if an error occurs.


.******************************************************

:h2 res=700
width=30%
.wpSetStyle
:link reftype=hd res=701
auto dependent group=2.
:p.
:link reftype=hd res=701.Syntax:elink.
.br
:link reftype=hd res=703 dependent.Returns:elink.
.br
:link reftype=hd res=704 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=315.Usage:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h2 res=701
x=30%
width=70%
hide
group=2
.wpSetStyle - Syntax
:xmp.
/* Set the style of an object */

theObject     /* A WPS object */
newStyle      /* The new style of the object */
:p.
rc=MFldrCallWPSFunc("wpSetStyle", 
                     theObject, newStyle);
:exmp.


:h2 res=703
x=30%
width=70% height=55%
group=3
hide
.wpSetStyle - Return value
:p.
:hp2.rc:ehp2. numeric
:p.
Returns 0 if an error occurs.

:h2 res=704
x=30%
width=70%
hide
group=2
.wpSetStyle - Remarks
:p.
The following flags may be ored together.
:ul.
:li.OBJSTYLE_NOMOVE          2
.br
Object cannot be moved.
:li.OBJSTYLE_NOLINK          4
.br
Shadows can't be created
:li.OBJSTYLE_NOCOPY          8
.br
Object cannot be copied.
:li.OBJSTYLE_TEMPLATE        32
.br
The object is a template.
:li.OBJSTYLE_NODELETE        64
.br
The object cannot be deleted.
:li.OBJSTYLE_NOPRINT         128
.br
The object cannot be printed.
:li.OBJSTYLE_NODRAG          256
.br
The object cannot be dragged.
:li.OBJSTYLE_NOTVISIBLE      512
.br
The object is hidden.
:li.OBJSTYLE_NOSETTINGS      1024
.br
There're not settings for this object.
:li.OBJSTYLE_NORENAME        2048
.br
The object cannot be renamed.
:li.OBJSTYLE_NODROPON        8192
.br
Other objects can't be dropped on this object.
:li.OBJSTYLE_CUSTOMICON      16384
.br
Icon is destroyed when object goes dormant. 
:eul.

