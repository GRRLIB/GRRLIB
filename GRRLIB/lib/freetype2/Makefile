# Quick'n'dirty makefile [BC]

ifeq ($(strip $(DEVKITPPC)),)
  $(error "Use   export DEVKITPPC=<path to>devkitPPC   and try again")
endif

ifeq ($(strip $(DEVKITPRO)),)
  $(error "Use   export DEVKITPRO=<path to>devkitPRO   and try again")
endif

# FreeType Makefile
PROJECT       := freetype
PROJECT_TITLE := FreeType
TOP_DIR ?= .
OBJ_DIR ?= $(TOP_DIR)
include $(TOP_DIR)/builds/toplevel.mk


INCD := $(DEVKITPRO)/libogc/include
LIBD := $(DEVKITPRO)/libogc/lib/wii

clean :
	rm -f lib$(PROJECT).a ftmodule.h

install :
	mkdir -p  $(LIBD)  $(INCD)/freetype/config/
	cp -rf  $(shell ls include/*.h)                     $(INCD)/
	cp -rf  $(shell ls include/freetype/*.h)            $(INCD)/freetype/
	cp -rf  $(shell ls include/freetype/config/*.h)     $(INCD)/freetype/config/
	cp -rf  lib$(PROJECT).a                             $(LIBD)/