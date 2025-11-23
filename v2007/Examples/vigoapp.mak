###############################################################################
# This file is a part of Vigo::3D, a multi-agent simulation and visualization #
# system.                                                                     #
#                                                                             #
# Copyright (c) 2002--2006 by Ian G. Burleigh       burleigh@cpsc.ucalgary.ca #
# burleigh@cpsc.ucalgary.ca                         http://ard-galen.org/vigo #
#                                                                             #
# This program is free software; you can redistribute it and/or modify it     #
# under the terms of the GNU General Public License as published by the Free  #
# Software Foundation; either version 2 of the License, or (at your option)   #
# any later version.                                                          #
#                                                                             #
# This program is distributed in the hope that it will be useful, but WITHOUT #
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       #
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for   #
# more details.                                                               #
#                                                                             #
# You should have received a copy of the GNU General Public License along     #
# with this program; if not, write to the                                     #
#   Free Software Foundation, Inc.,                                           #
#   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.                 #
#-----------------------------------------------------------------------------#
# $Id: vigoapp.mak 235 2007-05-23 19:04:22Z ian_b $
###############################################################################

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
VIGODIR	:= ../../Vigo

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

.PHONY: all clean

LIBVIGO := $(VIGODIR)/libvigo.a

LIBS	:= -lvigo

ifeq ($(OSTYPE),Darwin)
APPDIR	:= $(VIGOAPP).app/Contents/MacOS/
PKGTRG	:= $(VIGOAPP).app/PkgInfo
LIBS	+= -framework QtCore -framework QtGui -framework QtOpenGL
LIBS	+= -framework OpenGL -framework GLUT
else
APPDIR	:= ./
PKGTRG	:=
LIBS	+= -lXext -lX11 -lm -lGL -lGLU -lpthread -ldl
LIBS	+= -lQtCore -lQtGui -lQtOpenGL
endif

APPTRG	:= $(APPDIR)$(VIGOAPP)

###
all:	$(APPTRG) $(PKGTRG) 

$(APPTRG): $(LIBVIGO) $(OBJS)
	@mkdir -p $(APPDIR)
	$(CXX) $(CFLAGS) -o $@ $(OBJS) $(LFLAGS) $(LIBS)

$(LIBVIGO):
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
