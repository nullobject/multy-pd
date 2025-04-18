# Makefile to build class 'helloworld' for Pure Data.
# Needs Makefile.pdlibbuilder as helper makefile for platform-dependent build
# settings and rules.

# library name
lib.name = multy

# input source file (class name == source file basename)
class.sources = multy.c
shared.sources = grid.c

# all extra files to be included in binary distribution of the library
datafiles = README.md

cflags = $(shell pkg-config --cflags SDL2_gfx) -mmacosx-version-min=10.7
ldlibs = $(shell pkg-config --libs SDL2_gfx)

# include Makefile.pdlibbuilder from submodule directory 'pd-lib-builder'
PDLIBBUILDER_DIR=.
include $(PDLIBBUILDER_DIR)/Makefile.pdlibbuilder
