VIGOAPP	:= QtGui

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
$(error QTDIR is undefined)
endif

### source files
SRCS	:= sim.cpp main.cpp qtwindow.cpp slots.cpp
MOCS	:= slots.hpp

### Vigo
VIGODIR	:= ../../Vigo

### Operating system specific
ifdef OS
OSTYPE	:= $(OS)
else
OSTYPE	:= $(shell uname)
endif
include $(VIGODIR)/makedef.$(OSTYPE)

### Toolkit
MOC	:= $(QTDIR)/bin/moc
TKDEFS	:= QT_CORE_LIB QT_GUI_LIB QT_OPENGL_LIB QT_SHARED

### preprocessor defines
DEFS	:= $(OSDEFS) $(TKDEFS)
ifeq ($(DEBUG),yes)
DEFS	+= _DEBUG
else
DEFS	+= NDEBUG
endif

### compiler
INCDIRS := $(VIGODIR)

CFLAGS	:= -pipe -W -Wall
#-Wno-long-double
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
OBJS	:= $(patsubst %.cpp,%.o,$(SRCS))
MOCOBJS := $(patsubst %.hpp,%.mo,$(MOCS))

########################################################################
# implicit rules

.SUFFIXES: .c .cpp .cc .o .mo

%.o:	%.cpp
	$(CXX) $(CFLAGS) -c -o $@ $<

%.o:	%.c
	$(CC) $(CFLAGS) -c -o $@ $<

%.mo:	%.cc
	$(CXX) $(CFLAGS) -c -o $@ $<

%.cc:	%.hpp
	$(MOC) $< -o $@

########################################################################
# targets

.PHONY: all clean make_vigo_always

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

$(APPTRG): $(LIBVIGO) $(OBJS) $(MOCOBJS)
	@mkdir -p $(APPDIR)
	$(CXX) $(CFLAGS) -o $@ $(OBJS) $(MOCOBJS) $(LFLAGS) $(LIBS)

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
	rm -f $(OBJS) $(MOCOBJS) $(DEPS)

########################################################################
# dependencies

DEPS	= $(foreach SRC,$(SRCS),$(SRC).d) $(foreach SRC,$(MOCS),$(SRC).dd)

.SUFFIXES: .cpp.d .cc.dd
%.cpp.d: %.cpp
	$(CXX) $(CFLAGS) -MM "$<" -MT "$(patsubst %.cpp,%.o,$<)" >$@

%.cc.dd: %.cc
	$(CXX) $(CFLAGS) -MM "$<" -MT "$(patsubst %.cc,%.mo,$<)" >$@

ifndef OMIT_DEPS
-include $(DEPS)
endif

# eof
