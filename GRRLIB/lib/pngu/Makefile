#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------

ifeq ($(strip $(DEVKITPPC)),)
$(error "Please set DEVKITPPC in your environment. export DEVKITPPC=<path to>devkitPPC)
endif

ifeq ($(strip $(DEVKITPRO)),)
$(error "Please set DEVKITPRO in your environment. export DEVKITPRO=<path to>devkitPro)
endif

include $(DEVKITPPC)/base_rules
MACHDEP =  -DGEKKO -mcpu=750 -meabi -mhard-float

INSTALL_INC	:=	$(DEVKITPRO)/portlibs/ppc/include
INSTALL_LIB	:=	$(DEVKITPRO)/portlibs/ppc/lib

INCLUDE	:= -I$(DEVKITPRO)/portlibs/ppc/include
CFLAGS  := -O2 -Wall $(MACHDEP) $(INCLUDE)

LIB 	:= pngu
CFILES	:= $(wildcard *.c)
OFILES	:= $(CFILES:.c=.o)
DEPENDS	:= $(OFILES:.o=.d)
ARC 	:= lib$(LIB).a
HDR 	:= $(LIB).h

export DEPSDIR := $(CURDIR)

all : $(OFILES)
	$(AR) -r $(ARC) $(OFILES)

clean :
	rm -f $(OFILES) $(DEPENDS) $(ARC)

install :
	mkdir -p  $(INSTALL_LIB)  $(INSTALL_INC)
	cp -f $(ARC) $(INSTALL_LIB)/
	cp -f $(HDR) $(INSTALL_INC)
