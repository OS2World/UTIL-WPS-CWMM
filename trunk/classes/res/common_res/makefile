# Makefile

include ../../../make.inc

INC		= 	..\..\inc

all: cwaudio.res


cwaudio.res:	cwaudio.rc cwimage.rc cwvideo.rc $(INC)\mmres.h
	rc -r -n -D $(ECS) -I $(INC) cwaudio.rc

clean:
	-rm cwaudio.res

cleaner:
	-rm *.*~


