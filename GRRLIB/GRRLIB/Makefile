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

MACHDEP		+=	-DHW_DOL

INSTALL_INC	:=	$(DEVKITPRO)/portlibs/gamecube/include
INSTALL_LIB	:=	$(DEVKITPRO)/portlibs/gamecube/lib
else
include $(DEVKITPPC)/wii_rules

MACHDEP		+=	-DHW_RVL

INSTALL_INC	:=	$(DEVKITPRO)/portlibs/wii/include
INSTALL_LIB	:=	$(DEVKITPRO)/portlibs/wii/lib
endif

NULLSTR :=
PWD     := $(subst $(NULLSTR) ,\ ,$(shell pwd))

INCLUDE	:= -I../lib/pngu -I$(PWD) -I$(LIBOGC_INC) -I$(DEVKITPRO)/portlibs/ppc/include -I$(DEVKITPRO)/portlibs/ppc/include/freetype2
CFLAGS  := -O2 -Wall $(MACHDEP) $(INCLUDE)

LIB		:= grrlib
CFILES	:= $(wildcard *.c)
OFILES	:= $(CFILES:.c=.o)
DEPENDS	:= $(OFILES:.o=.d)
ARC		:= lib$(LIB).a
HDR		:= $(LIB).h
INL		:= $(wildcard $(LIB)/*.h)

export DEPSDIR := $(CURDIR)

all : $(OFILES)
	$(AR) -r $(ARC) $(OFILES)

clean :
	rm -f $(OFILES) $(DEPENDS) $(ARC)

install :
	mkdir -p  $(INSTALL_LIB)  $(INSTALL_INC)  $(INSTALL_INC)/grrlib
	cp -f $(ARC) $(INSTALL_LIB)/
	cp -f $(HDR) $(INSTALL_INC)/
	cp -f $(INL) $(INSTALL_INC)/grrlib
