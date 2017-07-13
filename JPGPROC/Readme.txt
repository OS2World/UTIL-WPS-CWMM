JPEG-IO procedure V0.1.0, 13.07.2002
------------------------------------

This procedure is a drop in replacement for
the JPEG procedure coming with MMOS/2. It supports
base JPEG image files (as the IO procedure provided
with MMOS/2) and also progressive ones. IO procedures
are used to load and write different image files. For
example the WPS uses IO procedures to load images for
folder backgrounds or for display in the image viewer.

The actual loading of the image file is done using
gbm.dll a free image manipulation library. See credits
for more information about it. The JPEG support is based
on work of the Independent JPEG Group. See credits for
more information.


Installation
------------

Make a copy of the JPEGIO.DLL in MMOS2\DLL and copy
JPEGIO.DLL, gbm.dll and JPEGIO.SYM to MMOS2\DLL. After
reboot the new IO procedure will be used by the system.
If the default DLL is locked you have to boot from a
maintenance partition or floppy disks.


How to compile
--------------

You need VAC++ V3.08 (or CSet/2). For your convenience the
source of GBM is included. There's no need to rebuild GBM if
you want to compile the IO procedure. See the documentation
for instructions for building GBM.


License
-------

/*
 * Copyright (c) Chris Wohlgemuth 2002
 * All rights reserved.
 *
 * http://www.geocities.com/SiliconValley/Sector/5785/
 * http://www.os2world.com/cdwriting
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The authors name may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */


Author
------

This IO procedure is

Copyright (c) Chris Wohlgemuth 2002

   http://www.os2world.com/cdwriting
   http://www.geocities.com/SiliconValley/Sector/5785/


Credits
-------

-GBM

GBM was written by Andy Key. 

The GBM packages are best obtained via the links on the home page :- 

     http://www.interalpha.net/customer/nyangau/

For more information about GBM see the files gbm.htm and gbmsrc.htm in the gbmsrc
directory.


-JPEG support

GBM is based in part on the work of the Independent JPEG Group. 

The Independant JPEG Group have generously allowed others to use their source code to their JPEG handling library.
The only condition of use is that the IJG must be credited for their work. Try looking for this library at :- 

     ftp://sun2.urz.uni-heidelberg.de/pub/simtel/graphics/jpegsr6a.zip. 
     ftp://ftp.simtel.net/pub/simtelnet/msdos/graphics/jpegsr6a.zip. 
     http://sunsite.doc.ic.ac.uk/computing/systems/ibmpc/simtel/msdos/graphics/jpegsr6a.zip. 

