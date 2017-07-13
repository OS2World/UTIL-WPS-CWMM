#
# Compiled with VAC++ 3.08
# (other compilers should do the job after minor tweaking)

CC           = icc
CPPFLAGS     = /Ge- /Gm+ /G4 /Gl  /O+ /Oc+ /Ol- /Q+ $(VACDEBUG)

all:	cddbpage.dll

cddbpage.dll:	cddbpage.obj  cddbpage.def
#	ILINK /NOFREE cddbpage.obj,cddbpage.dll,,,cddbpage.def;

cddbpage.obj:	cddbpage.c
	icc $(CPPFLAGS) cddbpage.c mmPM2.lib cddbpage.def
	
clean:
	-rm *.o
	-rm *.obj

cleaner:
	-rm *.*~
	-rm *.flc

distribution:
