# BC's quick'n'dirty makefile - with annotations

#-------------------------------------------------------------------------------
# Check devkitpro is installed
#-------------------------------------------------------------------------------
ifeq ($(strip $(DEVKITPPC)),)
  $(error "Use   export DEVKITPPC=<path to>devkitPPC   and try again")
endif

ifeq ($(strip $(DEVKITPRO)),)
  $(error "Use   export DEVKITPRO=<path to>devkitPRO   and try again")
endif

#-------------------------------------------------------------------------------
# Application will be named after this directory
#-------------------------------------------------------------------------------
APP := $(notdir $(CURDIR))
ELF := $(APP).elf
DOL := $(APP).dol
MAP := $(APP).map

#-------------------------------------------------------------------------------
# Source code and binary resources
#-------------------------------------------------------------------------------
SRC    := $(wildcard *.c)
SRCOBJ := $(patsubst %.c,%.o,$(SRC))

RES    := $(wildcard *.png)
RESOBJ := $(patsubst %.png,%.o,$(RES))
RESC   := $(patsubst %.png,%.c,$(RES))
RESH   := $(patsubst %.png,%.h,$(RES))

OBJ  := $(RESOBJ) $(SRCOBJ)

#-------------------------------------------------------------------------------
# Include required libraries ...the order can-be/is critical!
#-------------------------------------------------------------------------------
LIBS := -lgrrlib -lpngu -lpng -ljpeg -lz -lfat
LIBS += -lwiiuse
#LIBS += -lmodplay -lasnd
LIBS += -lbte -logc
LIBS +=  -lm

#-------------------------------------------------------------------------------
# Compilers & utils which we will be using
#-------------------------------------------------------------------------------
BINDIR  := $(DEVKITPPC)/bin
PREFIX  := $(BINDIR)/powerpc-eabi-

CC      := $(PREFIX)gcc
CXX     := $(PREFIX)g++
AR      := $(PREFIX)ar
AS      := $(PREFIX)as
LD      := $(CC)
OBJCOPY := $(PREFIX)objcopy

ELF2DOL := $(BINDIR)/elf2dol
UPLOAD  := $(BINDIR)/wiiload

#-------------------------------------------------------------------------------
# libogc - the main Wii/GC libraries
#-------------------------------------------------------------------------------
OGC  := $(DEVKITPRO)/libogc
INCD := $(OGC)/include
LIBD := $(OGC)/lib/wii

#-------------------------------------------------------------------------------
# Wii specific compiler flags
#-------------------------------------------------------------------------------
MACHDEP := -DGEKKO -mrvl -mcpu=750 -meabi -mhard-float
CFLAGS  := -O2 -Wall $(MACHDEP) -I $(INCD)

#-------------------------------------------------------------------------------
# Information for the linker
#-------------------------------------------------------------------------------
LDFLAGS   = $(MACHDEP) -Wl,-Map,$(MAP)
LIBPATHS := -L$(DEVKITPRO)/libogc/lib/wii

#-------------------------------------------------------------------------------
# Build rules
#-------------------------------------------------------------------------------

# "make all" -> Build the DOL file
all : $(DOL)

# "make clean" -> Erase everything made by `make`
clean :
	rm -f $(OBJ) $(RESC) $(RESH) $(ELF) $(DOL) $(MAP)

# "make run" -> Send the DOL file to the Wii
run : $(DOL)
	$(UPLOAD) $(DOL)

# The DOL file is built from the ELF file
$(DOL) : $(ELF)
	@echo Converting to: $@
	@$(ELF2DOL) $< $@

# The ELF is built from the OBJects
$(ELF) : $(OBJ)
	@echo Linking as: $@
	@$(CC) $^ $(LDFLAGS) $(LIBPATHS) $(LIBS) -o $@

# Objects (.o) are made from C files (.c)
%.o : %.c
	@echo Compiling: $<
	@$(CC) $(CFLAGS) -c $< -o $@

# C files (.c) can be made from PNG images
.PRECIOUS : %.c
%.c : %.png
	@echo Converting resource: $<
	@./raw2c.exe $< 2>null
