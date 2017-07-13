# Makefile

include ../../make.inc

CURDIR		= .

.PHONY:		clean cleaner clean_nlv

all:	nlv common_res

nlv:
	@echo "[35;1;mCreating laguage resources.[0;m"
	@for i in $(LANGUAGES); \
	do \
		echo "[33;1;m Creating "$$i"[0;m";  \
		cd $(CURDIR)/$$i && make LANGUAGE=_$$i && cd .. ; \
	done

common_res:
	cd ./common_res && make

clean_nlv:
	@for i in $(LANGUAGES); \
	do \
		cd $(CURDIR)/$$i && make LANGUAGE=_$$i clean && cd .. ; \
	done

clean:	clean_nlv
	-cd ./common_res && make clean

cleaner:

	
