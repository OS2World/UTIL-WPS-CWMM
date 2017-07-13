# Makefile

include ../../make.inc

INC		= ../include
CURDIR		= .

.PHONY:		clean cleaner clean_nlv

all:	nlv
	cd ./common_res && make

nlv:
	@echo "[35;1;mCreating laguage files.[0;m"
	@for i in $(LANGUAGES); \
	do \
		echo "[33;1;m Creating "$$i"[0;m";  \
		cd $(CURDIR)/$$i && make LANGUAGE=_$$i && cd .. ; \
	done

clean_nlv:
	@for i in $(LANGUAGES); \
	do \
		cd $(CURDIR)/$$i && make LANGUAGE=_$$i clean && cd .. ; \
	done

clean:	clean_nlv
	-cd ./common_res && make clean

cleaner:

	
