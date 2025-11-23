###############################################################################
# Copyright (c) 2006 by ....                            ....@cpsc.ucalgary.ca #
#-----------------------------------------------------------------------------#
# $Id: $
###############################################################################

VIGOAPP	:= template

### debug build: yes or no
ifndef DEBUG        # if DEBUG was not specified, e.g., 'make DEBUG=yes'
ifdef VIGO_DEBUG    # then set DEBUG based on VIGO_DEBUG
DEBUG	:= yes
else
DEBUG	:= no
endif
endif

### QTDIR
ifndef QTDIR
$(error QTDIR is not defined)
endif

### source directories
SRCDIRS += ./ 

### source files
SRCS	:= $(foreach DIR,$(SRCDIRS),$(wildcard $(DIR)*.c) $(wildcard $(DIR)*.cpp))

### Vigo
VIGODIR	:= ../Vigo

### include directories
INCDIRS	:= $(VIGODIR)

### Operating system specific
ifdef OS
OSTYPE	:= $(OS)
else
OSTYPE	:= $(shell uname)
endif
include $(VIGODIR)/makedef.$(OSTYPE)

### preprocessor defines
DEFS	:= $(OSDEFS)
ifeq ($(DEBUG),yes)
DEFS	+= _DEBUG
else
DEFS	+= NDEBUG
endif

### compiler flags
CFLAGS	:= -pipe -W -Wall
ifeq ($(DEBUG),yes)
CFLAGS	+= -O0 -g
else
CFLAGS	+= -Os
endif

CFLAGS	+= $(foreach DEF,$(DEFS),-D$(DEF)) $(foreach DIR,$(INCDIRS),-I$(DIR))

CFLAGS	+= $(OSCFLGS)

### linker
LIBDIRS := $(OSLIBD) $(VIGODIR)

LFLAGS	:= $(OSLFLGS)
LFLAGS	+= $(foreach DIR,$(LIBDIRS),-L$(DIR))

### object files
OBJS	:= $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SRCS)))

########################################################################
# implicit rules

.SUFFIXES: .c .cpp .o

%.o:	%.cpp
	$(CXX) $(CFLAGS) -c -o $@ $<

%.o:	%.c
	$(CC) $(CFLAGS) -c -o $@ $<

########################################################################
# targets

.PHONY: all clean make_vigo_always

LIBVIGO := $(VIGODIR)/libvigo.a

ifeq ($(OSTYPE),Darwin)
APPDIR	:= $(VIGOAPP).app/Contents/MacOS/
PKGTRG	:= $(VIGOAPP).app/PkgInfo
else
APPDIR	:= ./
PKGTRG	:=
endif

APPTRG	:= $(APPDIR)$(VIGOAPP)

###
all:	$(APPTRG) $(PKGTRG) 

$(APPTRG): $(LIBVIGO) $(OBJS)
	@mkdir -p $(APPDIR)
	$(CXX) $(CFLAGS) -o $@ $(OBJS) $(LFLAGS) $(LIBS)

$(LIBVIGO):	make_vigo_always
	cd $(VIGODIR) && make vigo

$(PKGTRG): 
	@echo "APPL????" >$@

clean:
ifeq ($(OSTYPE),Darwin)
	rm -rf $(VIGOAPP).app
else
	rm -f $(VIGOAPP)
endif
	rm -f $(OBJS) $(DEPS)

########################################################################
# dependencies

DEPS	= $(foreach SRC,$(SRCS),$(SRC).d)

.SUFFIXES: .cpp.d
%.cpp.d: %.cpp
	$(CXX) $(CFLAGS) -MM "$<" -MT "$(patsubst %.cpp,%.o,$<)" >$@

ifndef OMIT_DEPS
-include $(DEPS)
endif

# eof
########################################################################
