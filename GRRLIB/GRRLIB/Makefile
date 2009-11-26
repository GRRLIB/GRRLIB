# Quick'n'dirty makefile [BC] v2

ifeq ($(strip $(DEVKITPPC)),)
  $(error "Use   export DEVKITPPC=<path to>devkitPPC   and try again")
endif

ifeq ($(strip $(DEVKITPRO)),)
  $(error "Use   export DEVKITPRO=<path to>devkitPRO   and try again")
endif

NULLSTR :=
PWD     := $(subst $(NULLSTR) ,\ ,$(shell pwd))

PREFIX  := $(DEVKITPPC)/bin/powerpc-eabi-
CC   := $(PREFIX)gcc
AR   := $(PREFIX)ar

INCD := $(DEVKITPRO)/libogc/include
LIBD := $(DEVKITPRO)/libogc/lib/wii

MACHDEP := -DGEKKO -mrvl -mcpu=750 -meabi -mhard-float
CFLAGS  := -O2 -Wall $(MACHDEP) -I$(PWD) -I$(INCD)

LIB := grrlib
SRC := $(wildcard *.c)
OBJ := $(patsubst %.c,%.o,$(SRC))
ARC := lib$(LIB).a
HDR := $(LIB).h
INL := $(wildcard $(LIB)/*.h)

all : $(OBJ)
	$(AR) -r $(ARC) $(OBJ)

clean :
	rm -f $(OBJ) $(ARC)

install :
	mkdir -p  $(LIBD)  $(INCD)  $(INCD)/grrlib
	cp -f $(ARC) $(LIBD)/
	cp -f $(HDR) $(INCD)/
	cp -f $(INL) $(INCD)/grrlib

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@
