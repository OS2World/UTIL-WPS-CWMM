# List of languages to be built.
LANGUAGES	=	de en nl it

#define the next makro if compiling a release rather than a beta
#This will remove the build string from the about dialog
export CWMMRELEASE	=	YES

#define the next makro if compiling eCS. Undef if normal compile!
#That's used for createcd.exe until I find a decent documentation for make to do it better...
#Also used for including the right icons
#ECS_COMPILE	=	YES

ifdef    ECS_COMPILE
#         Don't use an empty string here!
ECS		= ECS
else
#         Don't use an empty string here!
ECS		= NO_ECS
endif

VERSION		=	0_2_9
VERSION_STRING	=	0.2.9
AUTHOR_STRING	=	"(C) Chris Wohlgemuth 2002-2005"

# Put version in environment for REXX skripts
export	MMCLSVERSION	=	$(VERSION_STRING)

ifdef    ECS_COMPILE
PKGNAME	= ecs-cw-mmclasses
else
PKGNAME	= cw-mmclasses
endif

ECSBASE = add2mmos2
ECSCLASSDIR = MMClass
ECSMFLDRDIR = MediaFld

#The base dir for additional functions for the MM helper programs
export COMMONSRC	=	H:/cwmm/common_functions


