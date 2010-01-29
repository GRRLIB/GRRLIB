# Quick'n'dirty makefile [BC] v2

ifeq ($(strip $(DEVKITPPC)),)
  $(error "Use   export DEVKITPPC=<path to>devkitPPC   and try again")
endif

ifeq ($(strip $(DEVKITPRO)),)
  $(error "Use   export DEVKITPRO=<path to>devkitPRO   and try again")
endif

PREFIX  := $(DEVKITPPC)/bin/powerpc-eabi-
CC   := $(PREFIX)gcc
AR   := $(PREFIX)ar

INCD := $(DEVKITPRO)/libogc/include
LIBD := $(DEVKITPRO)/libogc/lib/wii

MACHDEP := -DGEKKO -mrvl -mcpu=750 -meabi -mhard-float
CFLAGS  := -O2 $(MACHDEP) -I $(INCD)

LIB := jpeg
SRC := $(shell ls *.c)
OBJ := $(patsubst %.c,%.o,$(SRC))
ARC := lib$(LIB).a
HDR := jpeglib.h jconfig.h jmorecfg.h

all : $(OBJ)
	$(AR) -r $(ARC) $(OBJ)

clean :
	rm -f $(OBJ) $(ARC)

install :
	mkdir -p  $(LIBD)  $(INCD)
	cp -f $(ARC) $(LIBD)/
	cp -f $(HDR) $(INCD)/

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@
