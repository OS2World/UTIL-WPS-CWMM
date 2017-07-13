# Makefile for Audio-CD-Creator Data-CD-Creator

include ../../make.inc


RESDIR	=	..\res\$(LANGUAGE)
RESBASE2=	../res
RESDIR2	=	../res/$(LANGUAGE)

HELPSOURCE	=	$(RESDIR2)/ipf


all:	mediafldr.hlp
#mediafldr049.hlp mediafldr031.hlp
	
mediafldr.hlp:	mediafldr.ipf mediafldr049.ipf mediafldr031.ipf mediafldr039.ipf \
		mediafldr_ecs.ipf mediafldr049_ecs.ipf mediafldr031_ecs.ipf mediafldr039_ecs.ipf
ifdef    ECS_COMPILE
	ipfc mediafldr_ecs.ipf mediafldr.hlp
	ipfc mediafldr049_ecs.ipf mediafldr049.hlp
	ipfc mediafldr031_ecs.ipf mediafldr031.hlp
	ipfc mediafldr039_ecs.ipf mediafldr039.hlp
	ipfc mediafldr088_ecs.ipf mediafldr088.hlp
else
	ipfc mediafldr.ipf
	ipfc mediafldr049.ipf
	ipfc mediafldr031.ipf
	ipfc mediafldr039.ipf
	ipfc mediafldr088.ipf
endif


clean:
	-rm *.hlp

cleaner:
	-rm *.*~
	-rm *.flc

distger:

disteng:

distribution:
