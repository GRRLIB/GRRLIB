# Quick'n'dirty makefile [BC] v2

ifeq ($(strip $(DEVKITPPC)),)
  $(error "Use   export DEVKITPPC=<path to>devkitPPC   and try again")
endif

ifeq ($(strip $(DEVKITPRO)),)
  $(error "Use   export DEVKITPRO=<path to>devkitPRO   and try again")
endif

CC  := $(DEVKITPPC)/bin/powerpc-gekko-gcc
AR  := $(DEVKITPPC)/bin/powerpc-gekko-ar

OGC  := $(DEVKITPRO)/libogc
INCD := $(OGC)/include
LIBD := $(OGC)/lib/wii

MACHDEP := -DGEKKO -mrvl -mcpu=750 -meabi -mhard-float
CFLAGS  := -O2 -Wall $(MACHDEP) -I $(INCD)

LIB := pngu
SRC := $(shell ls *.c)
OBJ := $(patsubst %.c,%.o,$(SRC))
ARC := lib$(LIB).a
HDR := $(LIB).h

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
