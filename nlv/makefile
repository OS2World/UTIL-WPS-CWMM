# Makefile

include ../make.inc

#######################################
#
# Define *.hlp files.
#
#######################################
HLP_FILES	=	./Help/de/cwmm049.hlp \
			./Help/en/cwmm.hlp \
			./Help/nl/cwmm031.hlp \
			./Help/it/cwmm039.hlp

###################################
#
#  Rules for generating the NLV
#  ressources.
#
###################################
nlv:
	-@mkdir $(NLVBASE)/out
	@echo "[35;1;mCreating laguage resources for MM base classes.[0;m"
	@for i in $(LANGUAGES); \
	do \
		cd $(CURDIR)/$$i && make LANGUAGE=$$i && cd .. ; \
	done
	

###################################
#
#  Rules for generating online help
#
###################################
# German
./Help/de/cwmm049.hlp:	./Help/de/cwmm049.ipf 
	cd ./Help/de && make 	

# English
./Help/en/cwmm.hlp:	./Help/en/cwmm.ipf
	cd ./Help/en && make

# Dutch
./Help/nl/cwmm031.hlp:	./Help/nl/cwmm031.ipf
	cd ./Help/nl && make

# Italian
./Help/it/cwmm039.hlp:	./Help/it/cwmm039.ipf
	cd ./Help/it && make


###################################
#
#  Rules for cleaning the tree
#
###################################

clean_nlv:
	@for i in $(LANGUAGES); \
	do \
		cd $(CURDIR)/$$i && make LANGUAGE=_$$i clean && cd .. ; \
	done

clean:	clean_nlv
	-cd ./common_res && make clean

clean2:
	-cd $(OBJDIR) && rm *.obj
	-cd ./include && rm *.ih
	-cd ./include && rm *.h
	-cd ./include && rm *.hh
	-cd $(LIBDIR) && rm *.obj
	-rm *.dll
	-rm *.map
	-rm *.sym
	-cd ./Help/de && make clean
	-cd ./Help/en && make clean
	-cd ./Help/nl && make clean
	-cd ./Help/it && make clean
	-cd ./res && make clean
	-cd ./mm-progs && make clean


cleaner:
	-cd ./c && make clean
	-cd ./common_functions && make cleaner
	-cd ./mm-progs && make cleaner
	-cd ./Help/de && make cleaner
	-cd ./Help/en && make cleaner
	-cd ./Help/nl && make cleaner
	-cd ./Help/it && make cleaner
	-cd ./res && make cleaner
	-cd ./mm-progs && make cleaner
	-cd ./include && rm *.*~
	-cd ./inc && rm *.*~
	-cd $(IDLDIR)/idl_audio && rm *.*~
	-cd $(IDLDIR)/idl_image && rm *.*~
	-cd $(IDLDIR)/idl_mm && rm *.*~
	-cd $(IDLDIR)/idl_video && rm *.*~

distribution:
	make

