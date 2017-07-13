Slide Show V1.00 23.10.2004
------------------------

This little program displays image files contained in one or more
folders as a slide show. Fullscreen and windowed mode is possible.
The images may be presented in a randomized order.


Installation
---------
There's not much about installation. Create an program object pointing to
the executable and you're done. This may be done using the supplied script
createob.cmd.


Usage
-----

Just start the program and you're welcomed by the main dialog where you
may change settings and select directories to be shown. Note: the directory
list is drag and drop enabled.

If you start the program with a directory as the parameter the contents is
displayed imediatly in a window.

You may switch to fullscreen using the <RETURN> key. Get back to windowed
mode using ESC.

The following keys are supported:

RETURN
	switch to fullscreen
ESC
	back to windowed mode/close dialog
RIGHT
	next image
LEFT
	previous image
SPACE
	Pause/Continue
s
	Show controls
h
	Hide controls


Source
------
The source can be found in the subdirectory "source". The program is
written using DrDialog a free tool which can be found as drdialog.zip
at many places.
The source to drctl017.dll can be found in the drctl017 distribution.
Search

	http://www.os2world.com/cdwriting

or 
	http://hobbes.nmsu.edu

for a package named drdialog-controls-0_1_7.zip.


National Languages
----------------
The program is coming with german and english language support.
If you want to have a translation to your language translate the
contents of en.txt or de.txt. Name the resulting file XY.txt with
XY being your language code (see LANG config.sys setting).

Translate the file using the script addnls.cmd, for example

  addnls.cmd de.txt

Be sure you don't have any spaces in the path to addnls.cmd or
XY.txt.
Copy the resulting INI file into the \lang subdirectory and on next
start the program should use your language.


Credits
------
Gbmsize.exe and gbm.dll are (c) Andy Key

 http://www.interalpha.net/customer/nyangau/

They're public domain.

The JPEG support is based on code provided by The Independent JPEG Group.


License
------
The Slide Show program is provided under the following license.

/*
 * Copyright (c) Chris Wohlgemuth 2001-2004 
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
Copyright Chris Wohlgemuth 2004

http://www.os2world.com/cdwriting
http://www.geocities.com/SiliconValley/Sector/5785/

