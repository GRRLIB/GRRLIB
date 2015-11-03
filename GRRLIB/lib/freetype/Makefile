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

# FreeType Makefile
PROJECT       := freetype
PROJECT_TITLE := FreeType
TOP_DIR ?= .
OBJ_DIR ?= $(TOP_DIR)
include $(TOP_DIR)/builds/toplevel.mk

clean :
	rm -f lib$(PROJECT).a ftmodule.h

install :
	mkdir -p  $(LIBOGC_LIB)  $(LIBOGC_INC)/freetype/config/
	cp -rf  $(shell ls include/*.h)                     $(LIBOGC_INC)/
	cp -rf  $(shell ls include/freetype/*.h)            $(LIBOGC_INC)/freetype/
	cp -rf  $(shell ls include/freetype/config/*.h)     $(LIBOGC_INC)/freetype/config/
	cp -rf  lib$(PROJECT).a                             $(LIBOGC_LIB)/