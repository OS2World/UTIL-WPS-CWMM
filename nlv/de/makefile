
all:
	@echo "[33;1;m Creating $(LANGUAGE) [0;m"
	-@mkdir $(NLVBASE)/out/_$(LANGUAGE) 2>nul 1>nul
	@cd $(CURDIR)/classes/res && make LANGUAGE=_$(LANGUAGE) && cd .. 
