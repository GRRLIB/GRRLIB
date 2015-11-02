#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------

ifeq ($(strip $(DEVKITPPC)),)
$(error "Please set DEVKITPPC in your environment. export DEVKITPPC=<path to>devkitPPC)
endif

ifeq ($(strip $(DEVKITPRO)),)
$(error "Please set DEVKITPRO in your environment. export DEVKITPRO=<path to>devkitPro)
endif

PREFIX	:= $(DEVKITPPC)/bin/powerpc-eabi-
CC		:=	$(PREFIX)gcc
AR		:=	$(PREFIX)ar

LIBOGC_INC	:=	$(DEVKITPRO)/libogc/include
LIBOGC_LIB	:=	$(DEVKITPRO)/libogc/lib/wii

INCLUDE	:= 
MACHDEP := -DGEKKO -mrvl -mcpu=750 -meabi -mhard-float
CFLAGS  := -O2 -Wall $(MACHDEP) $(INCLUDE)

LIB 	:= zlib
CFILES	:= $(wildcard *.c)
OFILES	:= $(CFILES:.c=.o)
ARC 	:= libz.a
HDR 	:= $(LIB).h zconf.h

all : $(OFILES)
	$(AR) -r $(ARC) $(OFILES)

clean :
	rm -f $(OFILES) $(ARC)

install :
	mkdir -p  $(LIBOGC_LIB)  $(LIBOGC_INC)
	cp -f $(ARC) $(LIBOGC_LIB)/
	cp -f $(HDR) $(LIBOGC_INC)/

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@
