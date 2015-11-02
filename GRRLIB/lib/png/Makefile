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

LIBOGC_INC	:=	$(DEVKITPRO)/libogc/include
LIBOGC_LIB	:=	$(DEVKITPRO)/libogc/lib/wii

INCLUDE	:=  -I../zlib -I$(LIBOGC_INC)
MACHDEP := -DGEKKO -mrvl -mcpu=750 -meabi -mhard-float
CFLAGS  := -O2 -Wall $(MACHDEP) $(INCLUDE)

LIB 	:= png
CFILES	:= $(wildcard *.c)
OFILES	:= $(CFILES:.c=.o)
ARC 	:= lib$(LIB).a
HDR 	:= $(LIB).h $(LIB)conf.h

all : $(OFILES)
	$(AR) -r $(ARC) $(OFILES)

clean :
	rm -f $(OFILES) $(ARC)

install :
	mkdir -p  $(LIBOGC_LIB)  $(LIBOGC_INC)
	cp -f $(ARC) $(LIBOGC_LIB)/
	cp -f $(HDR) $(LIBOGC_INC)/
