# MM-prog specific makefile include

CC           	=	 gcc
CPPFLAGS     	=	 -O -Wall -Zomf -Zmt -Zsys -s
#CPPFLAGS     	= -O -Zomf -Zcrtdll -Zmtd -s

INC		= ../include -I ../../../classes/inc -I $(COMMONSRC)/include
LIB_DIR      	= $d:/emx/lib 
TOOLDIR		=	 .\..
LIBDIR		=	 ../lib
DEFS         	= 
PROGS        	=  
ODIR		=
CPPDIR		=

$(LIBDIR)/%.obj:	$(COMMONSRC)/%.c 
	$(CC) -c -Wall -o$@ -Zomf -I$(INC) $<  

%.obj:	%.c
	$(CC) -c -Wall -o$@ -Zomf -I$(INC) $<  


#distribution:
	
