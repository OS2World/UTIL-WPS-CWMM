# Makefile for Audio-CD-Creator Data-CD-Creator

include ../../make.inc


RESDIR	=	..\res\$(LANGUAGE)
RESBASE2=	../res
RESDIR2	=	../res/$(LANGUAGE)

HELPSOURCE	=	$(RESDIR2)/ipf


all:	mediafldrprog.inf
	
mediafldrprog.inf:	mediafldrprog.ipf hiddenfeatures.inc newmethods.inc wpsmethods.inc 
	ipfc -i mediafldrprog.ipf

clean:
	-rm *.inf

cleaner:
	-rm *.*~
	-rm *.flc

distger:

disteng:

distribution:
