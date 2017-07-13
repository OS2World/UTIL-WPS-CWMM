:userdoc.

:docprof.

:title.WPS undocumented features

:h1 res=100.Introduction
:p.
This document describes features and data structures of the WPS not documented in the official reference manuals. The
information was gathered from usegroups, public source code or by means of reverse engineering.
:p.


:h1 res=3000
x=left y=bottom..CLASSINFO
:p.
The .CLASSINFO extended attribute of filesystem objects contains all the settings of the specific object you
may change using the settings notebook. This EA tells the WPS which folder class a folder is an instance of and it
contains all the objects instance data saved using _wpSaveXXX().
:nt.
All data is stored in INTEL byte order in the EA, this means lowByte-HighByte.
See examples.
:ent.

:h2 res=3100
x=left width=100%
group=2
.General data structures

:xmp.
#define INSTANCEDATA_TYPE_LONG      0x0002
#define INSTANCEDATA_TYPE_STRING    0x0003
#define INSTANCEDATA_TYPE_BINARY    0x0004
:exmp.

:xmp.
/* Every block of instance data starts with this struct.
   The data follows directly after this header.          */
typedef struct _InstanceDataHead
{
 USHORT usType;     /* One of the INSTANCEDATA_TYPE_* constants */
 USHORT usKey;      /* key used in wpSaveXXX()                  */
 USHORT usLength;   /* size of data following                   */
}INSTANCEDATAHEAD;
:exmp.

:xmp.
/* The following structs represent the different types
   of instance data */
typedef struct _wpInstanceDataLong
{
 USHORT usType;      /* 0x0002 */
 USHORT usKey;       /* key used in wpSaveLong() */
 USHORT usLength;    /* size of value, always 0x0004 */
 LONG   lValue;      /* Data */
}INSTANCEDATALONG;


typedef struct _wpInstanceDataString
{
 USHORT usType;         /* 0x0003 */
 USHORT usKey;          /* key used in wpSaveString() */
 USHORT usLength;       /* String length including terminating 0 */
 UCHAR  ucData[];       /* The string including a terminating 0 */
}INSTANCEDATASTRING;


typedef struct _wpInstanceDataBinary
{
 USHORT usType;         /* 0x0004 */
 USHORT usKey;          /* key used in wpSaveData() */
 USHORT usLength;       /* Size of data block */
 BYTE   data[];         /* Data */
}INSTANCEDATABINARY;
:exmp.

:xmp.
/* Instance data block for a class. For every class in the chain of classes
   there may be one of these blocks. The current class is the first, the parent class
   instance data follows, then the grand parent class instance data etc. Last one is
   always WPObject. Directly after this header the data follows. Each data entity consists 
   of an INSTANCEDATAHEAD followed by the associated data as described above. */
typedef struct _InstanceDataBlockHead
{
 USHORT usClassNameLength;  /* Length of class name including terminating 0 */
 USHORT usDataSize;         /* Size of data block starting after this structure */
 UCHAR  ucClassName[];      /* Array holding name with terminating 0 used during saving.
                               Usually the classname but that's not enforced. */
 
}IDATABLOCKHEAD;
:exmp.

:xmp.
/* Structure of the .CLASSINFO attribute */
struct CLASSINFO
{
 const  USHORT usEAType;      /* Always EAT_BINARY: 0xFFFE */
 USHORT usSize;               /* Size of data starting with usUnknown1 */
 USHORT usFlags1;             /* 0x0000: data files (?) */
                              /* 0x0080: folder  (?)   */
                              /* 0x0090: folder with custom icon (set using settings notebook) */
                              /* 0x00C8: desktop folder (?) */
                              /* 0x0BD2: WPVault            */
 USHORT usFlags2;             /* 0x0000: ??   */
                              /* 0x0001: ??   */
 USHORT usDataSize;           /* Data size following the class name (starting with usOffset) */
 USHORT usUnknown3;
 UCHAR  ucClassName[];        /* The class of this object including the terminating 0, e.g. WPDataFile */
 USHORT usOffset;             /* Offset to WPObject instance data block starting at usOffset */
 USHORT usUnknown4;           /* */
 USHORT usUnknown5;           /* */
 USHORT usUnknown6;           /* Always the same as usUnknown4 */
 USHORT usSizeInstanceData;   /* Size of all INSTANCEDATABLOCKs + sizeof(usSizeInstanceData2) + sizeof(usUnknown6)
                                 + sizeof(usUnknown7) + sizeof(usUnknown8) + sizeof(usSizeInstanceData) */
 USHORT usUnknown7;           /* Always the same as usUnknown4 */
 USHORT usSizeInstanceData2;  /* Size of all INSTANCEDATABLOCKs including 6 terminating zero bytes.
                                 Each INSTANCEDATABLOCK structure is followed by a data block of variable length. */
 USHORT usUnknown8;           /* Seems to be alway 0x0000 (?) */
INSTANCEDATABLOCK instanceDataBlocks[];  /* One or more instance data blocks. At least the WPObject data block
                                 is present. */
                              /* The WPObject instance data block is always the last one. Attention: the instance
                                 data blocks may have different sizes! */
 const UCHAR ucZero[6];       /* 6 terminating zero bytes */
}
:exmp.

:h2 res=3200
x=left width=100%
group=2
.Instance data
:p.

:h3 res=3300
x=left width=100%
group=2
.Key 0x0b73 (2931), IDKEY_FDRLONGARRAY; WPFolder

:xmp.
Type    0x0004 (binary)
Size    0x54 (84)
:exmp.
The data can be represented by the following structure (items with ??? are taken from the WPTool source by H. Kelder and are not checked):
:xmp.
typedef struct _WPFolderLongArray
{
 ULONG ulIconView;
 ULONG ulTreeView;
 ULONG ulDetailsView; (???)
 ULONG ulFolderFlag;  (???)
 ULONG ulUnknown1;
 ULONG ulUnknown2;
 BYTE  rgbTextBkgnd[4];
 BYTE  Filler1[4];
 BYTE  Filler2[4]; 
 BYTE  rgbIconTextColor[4];
 BYTE  Filler3[4];
 BYTE  rgbTreeTextColor[4];
 BYTE  rgbDetailsTextColor[4];
 BYTE  Filler5[4];
 BYTE  Filler6[4];
 BYTE  Filler7[4];
 BYTE  Filler8[4];
 ULONG ulMenuFlag; (???)
 BYTE  rgbIconShadowTextColor[4];
 BYTE  rgbTreeShadowTextColor[4];
 BYTE  rgbDetailsShadowTextColor[4];

} WPFOLDLONGARRAY;
:exmp.

:xmp.
Example:

00 00 04 00 73 0B 54 00 80 00 30 00 64 00 50 00  
      ^^^^^ ^^^^^ ^^^^^ ^^          ^^    ^^
      |     |     |     |           |     |
      |     |     |     |           |     View (2. Tree): 0x50: with lines
      |     |     |     |           |                     0x10: without lines
      |     |     |     |           Tree Icon: 0x64: small
      |     |     |     |                      0x44: normal
      |     |     |     |                      0x41: invisible
      |     |     |     |
      |     |     |     View (first view page): 22h: Non flowed, single column, small icons  
      |     |     |                             02h: Non flowed, single column, normal icons
      |     |     |                             01h: Non flowed, single column, invisible icons
      |     |     |                             32h: Non flowed, multiple columns, small icons
      |     |     |                             12h: Non flowed, multiple columns, normal icons
      |     |     |                             11h: Non flowed, multiple columns, invisible icons
      |     |     Size (84 dec)                 04h: Flowed as placed normal icons
      |     |                                   24h: Flowed as placed small icons 
      |     Key                                 80h: Gridded 
      Binary

08 80 30 00 03 00 00 00 00 00 00 00 FF FF FF FF

55 55 80 40 EF FF FF FF EF FF FF FF DD FF FF FF <--- 0xFF if not changed
^^ ^^ ^^ ^^                         ^^ ^^ ^^ ^^--> Icon text color
|  |  |  |                          
|  |  |  0x40 if color never set, after setting 0x00. If 0xff apparently use system default.
|  |  R  |
|  G  |  |
B  |  |  |
|  |  |  |
------------> Text background for every view. If set to transparency: 0xFFFFFF01
|
01 if Transparent

EF FF FF FF EF FF FF FF DD FF FF FF 00 00 00 40  
            ^^ ^^ ^^ ^^ 
            |_________|
                      |
                      Tree text color (init: 0xFFFFFFEF)

00 00 00 40 00 00 00 40 00 00 00 00 02 00 00 00  


D0 FF FF FF D0 FF FF FF D0 FF FF FF 04 00 74 0B  
^^ ^^ ^^ ^^ ^^ ^^ ^^ ^^ ^^ ^^ ^^ ^^
|  |  |  |  |  |  |  |  |_________|--Details shadow text color
|  |  |  |  |  |  |  |            |
|  |  |  |  |  |  |  |            Last byte
|  |  |  |  |  |  |  |            
|  |  |  |  |  |  |  0xFF if not changed
|  |  |  |  |  |  R       
|  |  |  |  |  G                  
|  |  |  |  B                     
|  |  |  |  |________|-- Tree shadow text color
|  |  |  |                        
|  |  |  |                        
|  |  |  0xFF if not changed
|  |  R          
|  G                      
B        |
|________|-- Icon shadow text color
:exmp.


:h3 res=3600
x=left width=100%
group=2
.Key 0x04 (4), WPObject
:p.
This key is always at the end of the .CLASSINFO data. It is followed by the 6 terminating zeros.
:xmp.
Type   0x0004
Size   0x08
struct _WPObjectKey04
{
 BYTE bytes[8]; /* Seems to be most of the time 0xFFFFFFFF FFFFFFFF
                   but sometimes has indeed another value. */
}
:exmp.

:h3 res=3400
x=left width=100%
group=2
.Key 0x0b (11), WPObject data
:p.
This info is from the WPTOOL source by H. Kelder. Items with (??) are not checked for accuracy.
:xmp.
Type   0x0004
Size   0x20 (32)

struct _WPObjectData
{
LONG  lDefaultView;   /* As set in the settings notebook e.g 102: Details view (OPEN_DETAILS) */
ULONG ulHelpPanel;    (??)
ULONG ulUnknown3;     (??)
ULONG ulObjectStyle;  /* Different OBJSTYLE_* ored together */
ULONG ulMinWin;       (??)
ULONG ulConcurrent;   (??)
ULONG ulViewButton;   (??)
ULONG ulMenuFlag;     /* Short menus: 0x00000000, normal (long): 0x00000001 */
} WPOBJDATA;
:exmp.

:h3 res=3500
x=left width=100%
group=2
.Key 0x0c (12), WPObject
:p.
Contains the ID if set and additional data, probably the object handle.
:xmp.
Type   0x0004
Size   0x02 when empty, data is 0xFFFF
       0xXX when data saved.

struct _WPObjectDataKey0C_1
{
 USHORT usType;   /* 0x0001 */
 UCHAR  ucID[];   /* ID with terminating 0 */
}
:exmp.
:p.
This structure may be followed (but isn't always) by another one. Or the second one is the only
structure if the object doesn't has an ID but a handle. 
:p.
:xmp.
struct _WPObjectDataKey0C_2
{
 USHORT usType;      /* 0x0002 */
 UCHAR  ucHandle[];  /* Apparently handle(s) in decimal followed by an ID value,
                        terminated with 0. e.g. 206526@10. Several handles with ID
                        separated by ',' may be found. The handle is always the same
                        (of course) but the ID different, e.g. 251239@10,251239@20.
                        The ID probably specifies icon/details/tree view. I guess
                        this is added when a folder position record is added to the INI.
                        XXXXXX@YY is the key in the INI then. 
                        @1010: tree view 
                        @10:
                        @20: */
}
:exmp.
:p.
The structure(s) are always followed by 0xFFFF. If the key is 'empty' only this
value is stored. The 0xFFFF is included in the size.
:p.
:xmp.
USHORT usPad[2];     /* Always 0xFFFF. Always appended. */
:exmp.

.********************** methods ************************
:h1 res=4000
x=left y=bottom width=100%.Methods


:h2 res=4010
x=left y=bottom width=100%.wpMakeDormant

:xmp.
This can fail if the object is locked by system for some reason e.g. running executables.

BOOL wpDestroyObject();


:exmp.
:xmp.
BOOL wpMakeDormant(in BOOL fSave)

// finally, this calls wpMakeDormant, which destroys the SOM object
                brc = _wpMakeDormant(somSelf, 0);

/*
 *@@sourcefile object.c
 *      implementation code for XFldObject class.
 *
 *      This file is ALL new with V0.9.0.
 *
 *      This looks like a good place for general explanations
 *      about how the WPS maintains Desktop objects as SOM objects
 *      in memory.
 *
 *      The basic object life cycle is like this:
 *
 +        wpclsNew
 +        wpCopyObject
 +        wpCreateFromTemplate
 +              ³
 +              ³      ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
 +              ³      ³ awake object ³
 +              ÀÄÄÄÄ> ³ (physical    ³ <ÄÄ wpclsMakeAwake ÄÄÄ¿
 +                     ³ and memory)  ³                       ³
 +                     ÀÄÄÄÄÄÂÄÂÄÄÄÄÄÄÙ                       ³
 +                           ³ ³                              ³
 +          \ /              ³ ³                   ÚÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄ¿
 +           X   <ÄÄ wpFree ÄÙ ÀÄ wpMakeDormant Ä> ³ dormant object ³
 +          / \                                    ³ (physical only)³
 +                                                 ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
 *
 *      The basic rule is that there is the concept of a
 *      "dormant object". This is an object that currently
 *      exists only in storage, but not as a SOM object in
 *      memory. "Storage" is disk for file-system objects
 *      and OS2.INI for abstract objects.
 *
 *      By contrast, an object is said to be "awake" if it
 *      also exists as a SOM object in memory. Per definition,
 *      an awake object also has a physical representation.
 *      Only WPTransients have no physical storage at all;
 *      as a consequence, there is really no such thing as
 *      a dormant transient object.
 *
 *      1)  Objects are most frequently made awake when a folder
 *          is populated (mostly on folder open). Of course this
 *          does not physically create objects... they are only
 *          instantiated in memory then from their physical
 *          storage.
 *
 *          There are many ways to awake objects. To awake a
 *          single object, call M_WPObject&colon.&colon.wpclsMakeAwake.
 *          This is a bit difficult to manage, so
 *          WPFileSystem&colon.&colon.wpclsQueryObjectFromPath is easier
 *          for waking up FS objects (which calls wpclsMakeAwake
 *          internally). wpclsMakeAwake also gets called from
 *          within WPFolder&colon.&colon.wpPopulate.
 *
 *      2)  Even though this isn't documented anywhere, the
 *          WPS also supports the reverse concept of making
 *          the object dormant again. This will destroy the
 *          SOM object in memory, but not the physical representation.
 *
 *          I suspect this was not documented because you can
 *          never know whether some code still needs the
 *          SOM pointer to the object somehow. Anyway, the
 *          WPS _does_ make objects dormant again, e.g. when
 *          their folders are closed and they are not referenced
 *          from anywhere else. You can prevent the WPS from doing
 *          this by calling WPObject&colon.&colon.wpLock.
 *
 *          The interesting thing is that there is an undocumented
 *          method for destroying the SOM object.
 *          WPObject&colon.&colon.wpMakeDormant does exactly this.
 *          Actually, this does a lot of things:
 *
 *          --  It removes the object from all containers,
 *
 *          --  closes all views (wpClose),
 *
 *          --  frees all associated memory allocated thru wpAllocMem.
 *
 *          --  In addition, if the object has called _wpSaveDeferred
 *              and a _wpSaveImmediate is thus pending, _wpSaveImmediate
 *              also gets called. (See XFldObject&colon.&colon.wpSaveDeferred
 *              for more about this.)
 *
 *          --  Finally, wpMakeDormant calls wpUnInitData, which
 *              should clean up the object.
 *
 *      3)  An object is physically created through either
 *          M_WPObject&colon.&colon.wpclsNew or WPObject&colon.&colon.wpCopyObject or
 *          WPObject&colon.&colon.wpCreateAnother or WPObject&colon.&colon.wpCreateFromTemplate.
 *          These are the "object factories" of the WPS. Depending
 *          on which class the method is invoked on, the new object
 *          will be of that class.
 *
 *          Depending on the object's class, wpclsNew will create
 *          a physical representation (e.g. file, folder) of the
 *          object _and_ a SOM object. So calling, for example,
 *          M_WPFolder&colon.&colon.wpclsNew will create a new folder on disk
 *          and return a SOM object that represents it.
 *
 *      4)  Deleting an object can really be done in two ways:
 *
 *          --  WPObject&colon.&colon.wpDelete looks like the most natural
 *              way. However this really only displays a
 *              confirmation and then invokes WPObject&colon.&colon.wpFree.
 *
 *          --  WPObject&colon.&colon.wpFree is the most direct way to
 *              delete an object. This does not display any
 *              more confirmations (in theory), but deletes
 *              the object right away.
 *
 *              Interestingly, wpFree in turn calls another
 *              undocumented method -- WPObject&colon.&colon.wpDestroyObject.
 *              From my testing this is responsible for destroying
 *              the physical representation (file, folder, INI data).
 *
 *              After that, wpFree also calls wpMakeDormant
 *              to free the SOM object.
 *
 *      wpDestroyObject is a bit obscure. I believe it is this
 *      method which was supposed to do the object cleanup.
 *      It is introduced by WPObject and overridden by the
 *      following classes:
 *
 *      --  WPFileSystem&colon. apparently, this then does DosDelete.
 *          Unfortunately, this one has a real nasty bug... it
 *          displays a message box if deleting the object fails.
 *          This is really annoying when calling wpFree in a loop
 *          on a bunch of objects. That's why we override this
 *          method in WPDataFile&colon.&colon.wpDestroyObject and similar
 *          methods (V0.9.20).
 *
 *      --  WPAbstract&colon. this probably removes the INI entries
 *          associated with the abstract object.
 *
 *      --  WPProgram.
 *
 *      --  WPProgramFile.
 *
 *      --  WPTransient.
 *
 *      If folder auto-refresh is replaced by XWP, we must override
 *      wpFree in order to suppress calling this method. The message
 *      box bug is not acceptable for file-system objects, so we have
 *      introduced WPDataFile&colon.&colon.wpDestroyObject instead.
 *
 *      The destruction call sequence thus is&colon.&colon.
 *
 +          wpDelete (display confirmation, if applicable)
 +             |
 +             +-- wpFree (really delete the object now)
 +                   |
 +                   +-- wpDestroyObject (delete physical storage)
 +                   |
 +                   +-- wpMakeDormant (delete SOM object in memory)
 +                         |
 +                         +-- (lots of cleanup: wpClose, etc.)
 +                         |
 +                         +-- wpSaveImmediate (if "dirty")
 +                         |
 +                         +-- wpUnInitData
 *
 *      Function prefix for this file:
 *      --  obj*
 *
 *@@added V0.9.0 [umoeller]
 *@@header "filesys\object.h"
 */
:exmp.


:h2 res=4020
x=left y=bottom width=100%.wpRefresh
:p.
If the menu item "refresh" is selected, wpRefresh(ULONG ulView, PVOID pReserved) is called with
ulView==0.
:p.
If any of the "change to" menu items is selected (details, contents...), ulView will be the selected view.


.********************** Setup Strings ************************
:h1 res=2000
x=left y=bottom width=30%.Setup strings
:p.
The following setup strings are more or less undocumented. 
:ul.
:li.:link reftype=hd res=2010.SAVEALLICONPOS:elink.

:eul.

:h2 res=2010
x=30% width=70%
group=2
.SAVEALLICONPOS
:p.
Force a folder to save the icon positions of all it's objects. Seems to work at least with the desktop. May
also work with every other folder class.
:xmp.
   SysSetObjectData("<WP_DESKTOP>"," SAVEALLICONPOS=YES")
:exmp.


.********************** random stuff ************************
:h1 res=5000
x=left y=bottom width=100%.Other observations
:p.
:ul.
:li.wpQueryDetailsData() is always called before wpObjectReady()
:li.wpRestoreData() is called before wpQueryDetailsData()

:eul.
:euserdoc.








