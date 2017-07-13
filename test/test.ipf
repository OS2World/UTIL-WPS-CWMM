:userdoc.
:docprof.

:title.Test document

:h1 res=100.How to see the problem
:p.
Open the panel called :hp2.Test item (open this):ehp2. and read the explanation.
:p.

.********************************************

:h1 res=1000
width=30%
.Go to the next header
:p.
Open the following chapter by double clicking
:p.
.br
:link reftype=hd res=1010 dependent.Test item (open this):elink.
.br

.******************************************************

:h2 res=1010
width=30%
.Test item (open this)
:link reftype=hd res=1011
auto dependent group=2.
:p.
As you can see there is a panel on the left (this) and one on the right. The
right panel is opened automatically using a :hp2.&colon.link reftype=hd res=1011 auto dependent group=2.:ehp2. statement
when the left pane is opened. This does work as expected.
:p.
When using View.exe the right panel automatically closes when the left panel is closed. Using NewView
the right panel remains open.
:p.
:link reftype=hd res=1011.Right panel:elink.
.br
:link reftype=hd res=1013 dependent.Another panel on the right:elink.
.br

:h2 res=1011
x=30%
width=70%
hide
group=2
.Panel on the right
:p.
This panel remains open with Newview if the left panel is closed. Using View.exe this panel also
closes.
:xmp.


/* Here is some contents */
:exmp.


:h2 res=1013
x=30%
width=70% height=35%
group=3
hide
.Other panel on the right
:p.
This panel remains open with Newview if the left panel is closed. Using View.exe this panel also
closes.


:euserdoc.








