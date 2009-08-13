# Quick'n'dirty makefile [BC] v2

ifeq ($(strip $(DEVKITPPC)),)
  $(error "Use   export DEVKITPPC=<path to>devkitPPC   and try again")
endif

ifeq ($(strip $(DEVKITPRO)),)
  $(error "Use   export DEVKITPRO=<path to>devkitPRO   and try again")
endif

APP := compose
ELF := $(APP).elf
DOL := $(APP).dol
MAP := $(notdir $(ELF)).map

LIBS := -lgrrlib -lpngu -lpng -ljpeg -lz
LIBS += -lwiiuse
#LIBS += -lmodplay -lasnd
LIBS += -lbte -logc
LIBS +=  -lm

BINDIR  := $(DEVKITPPC)/bin
PREFIX  := $(BINDIR)/powerpc-gekko-
CC      := $(PREFIX)gcc
CXX     := $(PREFIX)g++
AR      := $(PREFIX)ar
AS      := $(PREFIX)as
LD      := $(CC)
OBJCOPY := $(PREFIX)objcopy
ELF2DOL := $(BINDIR)/elf2dol
UPLOAD  := $(BINDIR)/wiiload

OGC  := $(DEVKITPRO)/libogc
INCD := $(OGC)/include
LIBD := $(OGC)/lib/wii

MACHDEP := -DGEKKO -mrvl -mcpu=750 -meabi -mhard-float
CFLAGS  := -O2 -Wall $(MACHDEP) -I $(INCD)

LDFLAGS   = $(MACHDEP) -Wl,-Map,$(MAP)
LIBPATHS := -L$(DEVKITPRO)/libogc/lib/wii

SRC    := $(shell ls *.c)
SRCOBJ := $(patsubst %.c,%.o,$(SRC))

RES    := $(shell ls *.png)
RESOBJ := $(patsubst %.png,%.o,$(RES))
RESC   := $(patsubst %.png,%.c,$(RES))
RESH   := $(patsubst %.png,%.h,$(RES))

OBJ  := $(RESOBJ) $(SRCOBJ)

all : $(DOL)

$(DOL) : $(ELF)
	@echo Converting to: $@
	@$(ELF2DOL) $< $@

$(ELF) : $(OBJ)
	@echo Linking as: $@
	@$(CC) $^ $(LDFLAGS) $(LIBPATHS) $(LIBS) -o $@

clean :
	rm -f $(OBJ) $(RESC) $(RESH) $(ELF) $(DOL) $(MAP)

run : $(DOL)
	$(UPLOAD) $(DOL)

%.o : %.c
	@echo Compiling: $<
	@$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS : %.c
%.c : %.png
	@echo Converting resource: $<
	@./raw2c.exe $<
