#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------

ifeq ($(strip $(DEVKITPPC)),)
$(error "Please set DEVKITPPC in your environment. export DEVKITPPC=<path to>devkitPPC)
endif

ifeq ($(strip $(DEVKITPRO)),)
$(error "Please set DEVKITPRO in your environment. export DEVKITPRO=<path to>devkitPro)
endif

ifeq ($(PLATFORM),cube)
include $(DEVKITPPC)/gamecube_rules
else
include $(DEVKITPPC)/wii_rules
endif

NULLSTR :=
PWD     := $(subst $(NULLSTR) ,\ ,$(shell pwd))

INSTALL_INC	:=	$(DEVKITPRO)/portlibs/ppc/include
INSTALL_LIB	:=	$(DEVKITPRO)/portlibs/ppc/lib

INCLUDE	:= -I../lib/pngu -I$(PWD) -I$(LIBOGC_INC) -I$(DEVKITPRO)/portlibs/ppc/include -I$(DEVKITPRO)/portlibs/ppc/include/freetype2
CFLAGS  := -O2 -Wall $(MACHDEP) $(INCLUDE)

LIB		:= grrlib
CFILES	:= $(wildcard *.c)
OFILES	:= $(CFILES:.c=.o)
ARC		:= lib$(LIB).a
HDR		:= $(LIB).h
INL		:= $(wildcard $(LIB)/*.h)

all : $(OFILES)
	$(AR) -r $(ARC) $(OFILES)

clean :
	rm -f $(OFILES) $(ARC)

install :
	mkdir -p  $(INSTALL_LIB)  $(INSTALL_INC)  $(INSTALL_INC)/grrlib
	cp -f $(ARC) $(INSTALL_LIB)/
	cp -f $(HDR) $(INSTALL_INC)/
	cp -f $(INL) $(INSTALL_INC)/grrlib
