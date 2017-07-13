.******************************************************
:h2 res=2050
width=30%
.winFileDlg
:link reftype=hd res=2051
auto dependent group=2.
:p.
:link reftype=hd res=2051.Syntax:elink.
.br
:link reftype=hd res=2053 dependent.Returns:elink.
.br
:link reftype=hd res=2054 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=315.Usage:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h2 res=2051
x=30%
width=70%
hide
group=2
.winFileDlg - Syntax
:xmp.
/* Display a file dialog. It may be an 'open'- or
   a 'Save as' dialog */

flag          /* 0: open dialog, 1: SaveAs dialog */
title         /* Title of the file dialog         */
fileMask      /* The file mask for example *.exe  */

:p.
fileName=MFldrCallWinFunc("winFileDlg", 
                    flag, title, fileMask);
:exmp.


:h2 res=2053
x=30%
width=70% height=35%
group=3
hide
.winFileDlg - Return value
:p.
:hp2.fileName:ehp2. string
:p.
Selected file name
:p.
In case of error :hp2.ERROR::ehp2. is returned.


:h2 res=2054
x=30%
width=70%
hide
group=2
.winFileDlg - Remarks
:p.
The frame handle of the media folder is given to the skript as a parameter
when started.


.******************************************************
:h2 res=2010
width=30%
.winQueryContainerHWND
:link reftype=hd res=2011
auto dependent group=2.
:p.
:link reftype=hd res=2011.Syntax:elink.
.br
:link reftype=hd res=2013 dependent.Returns:elink.
.br
:link reftype=hd res=2014 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=315.Usage:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h2 res=2011
x=30%
width=70%
hide
group=2
.winQueryContainerHWND - Syntax
:xmp.
/* This function returns the container handle from
    the frame handle of a folder */

frameHandle   /* Handle of the folder frame  */
:p.
containerHandle=MFldrCallWinFunc("winQueryContainerHWND", 
                    frameHandle);
:exmp.


:h2 res=2013
x=30%
width=70% height=35%
group=3
hide
.winQueryContainerHWND - Return value
:p.
:hp2.containerHandle:ehp2. numeric
:p.
Handle to the container of a folder.


:h2 res=2014
x=30%
width=70%
hide
group=2
.winQueryContainerHWND - Remarks
:p.
The frame handle of the media folder is given to the skript as a parameter
when started.


.******************************************************
:h2 res=2020
width=30%
.winQueryContainerRecord
:link reftype=hd res=2021
auto dependent group=2.
:p.
:link reftype=hd res=2021.Syntax:elink.
.br
:link reftype=hd res=2023 dependent.Returns:elink.
.br
:link reftype=hd res=2024 dependent.Remarks:elink.
.br
:link reftype=hd res=2025.Parameters:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h2 res=2021
x=30%
width=70%
hide
group=2
.winQueryContainerRecord - Syntax
:xmp.
/* This function returns the requested item from
    the specified container handle from */

containerHandle  /* Handle to the container              */
pRec             /* Pointer to previous item or 0        */
cmd              /* Value specifying which item to query */
fsSearch         /* Enumeration order                    */
:p.
ptrRec=MFldrCallWinFunc("winQueryContainerRecord", 
                         containerHandle, pRec,
                         cmd, fsSearch);
:exmp.


:h2 res=2023
x=30%
width=70% height=35%
group=3
hide
.winQueryContainerRecord - Return value
:p.
:hp2.ptrRec:ehp2. numeric
:p.
Pointer to a container item.


:h2 res=2024
x=30%
width=70%
hide
group=2
.winQueryContainerRecord - Remarks
:p.
The frame handle of the media folder is given to the skript as a parameter
when started.

:h2 res=2025
x=30%
width=70%
hide
group=2
.winQueryContainerRecord - Parameters
:p.
:ul.
:li.:hp2.containerHandle:ehp2.
.br
Handle to a container.
:li.:hp2.pRec:ehp2.
.br
Pointer to previous item or 0. This parameter is ignored if CMA_FIRST or CMA_LAST is specified.
:li.:hp2.cmd:ehp2.
.br
Command value specifying which container item to retrieve.
:sl.
:li.CMA_FIRST  0x0010
.br
Get first container record.
:li.CMA_LAST   0x0020
.br
Get last container record.
:li.CMA_PREV   0x0080
.br
Get previous container record of :hp1.pRec:ehp1..
:li.CMA_NEXT   0x0100
.br
Get next container record after :hp1.pRec:ehp1..
:esl.
:li.:hp2.fsSearch:ehp2.
.br
Enumeration order.
:sl.
:li.CMA_ITEMORDER  1
.br
Query records in item order.
:li.CMA_ZORDER     8
.br
Query records in z-order. The last record is the last to be drawn. This flag
is only valid in icon view.
:esl.

:eul.

.******************************************************
:h2 res=2040
width=30%
.winQueryContRecEmphasis
:link reftype=hd res=2041
auto dependent group=2.
:p.
:link reftype=hd res=2041.Syntax:elink.
.br
:link reftype=hd res=2043 dependent.Returns:elink.
.br
:link reftype=hd res=2044 dependent.Remarks:elink.
.br
:link reftype=hd res=2045.Parameters:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h2 res=2041
x=30%
width=70%
hide
group=2
.winQueryContRecEmphasis - Syntax
:xmp.
/* This function returns a record with a certain 
   emphasis from the specified container handle */

containerHandle  /* Handle to the container       */
pRec             /* Pointer to previous item or 0 */
fEmphasis        /* Emphasis mask                 */
:p.
ptrRec=MFldrCallWinFunc("winQueryContainerRecord", 
                         containerHandle, pRec,
                         fEmphasis);
:exmp.


:h2 res=2043
x=30%
width=70% height=35%
group=3
hide
.winQueryContRecEmphasis - Return value
:p.
:hp2.ptrRec:ehp2. numeric
:p.
Pointer to a container record or 0. In case of an error -1 is returned.


:h2 res=2044
x=30%
width=70%
hide
group=2
.winQueryContRecEmphasis - Remarks
:p.
The frame handle of the media folder is given to the skript as a parameter
when started.

:h2 res=2045
x=30%
width=70%
hide
group=2
.winQueryContRecEmphasis - Parameters
:p.
:ul.
:li.:hp2.containerHandle:ehp2.
.br
Handle to a container.
:li.:hp2.pRec:ehp2.
.br
Pointer to previous item or CMA_FIRST (0x0010).
:li.:hp2.fEmphasis:ehp2.
.br
Enumeration order.
:sl.
:li.CRA_SELECTED   1
.br
Selected records.
:li.CRA_CURSORED   4
.br
Records drawn with a selection cursor.
:li.CRA_INUSE      8
.br
Records with in-use emphasis
:li.CRA_FILTERED   16
.br
Filtered records that means hidden from view.
:li.CRA_SOURCE     0x4000
.br
Records drawn with source emphasis.
:li.CRA_DISABLED   0x1000
.br
Disabled records.
:esl.
:eul.

.******************************************************
:h2 res=2030
width=30%
.winObjectFromPRec
:link reftype=hd res=2031
auto dependent group=2.
:p.
:link reftype=hd res=2031.Syntax:elink.
.br
:link reftype=hd res=2033 dependent.Returns:elink.
.br
:link reftype=hd res=2034 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=2025.Parameters:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h2 res=2031
x=30%
width=70%
hide
group=2
.winObjectFromPRec - Syntax
:xmp.
/* This function returns the object pointer
   from a container record. */

pRec             /* Pointer to a WPS container record */
:p.
pObject=MFldrCallWinFunc("winObjectFromPRec", 
                         pRec)

:exmp.


:h2 res=2033
x=30%
width=70% height=35%
group=3
hide
.winObjectFromPRec - Return value
:p.
:hp2.ptrRec:ehp2. numeric
:p.
Pointer to an object.


:h2 res=2034
x=30%
width=70%
hide
group=2
.winObjectFromPRec - Remarks
:p.
The record pointer must be queried from a WPS container that means a container
created by a WPS object and filled with WPS objects.


