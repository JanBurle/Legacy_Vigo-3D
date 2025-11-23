### debug build: yes or no
ifndef DEBUG        # if DEBUG was not specified, e.g., 'make DEBUG=yes'
ifdef VIGO_DEBUG    # then set DEBUG based on VIGO_DEBUG
DEBUG	:= yes
else
DEBUG	:= no
endif
endif

### QTDIR: issue warning (QTDIR is not required for every target)
ifndef QTDIR
$(warning QTDIR is undefined)
endif

### source directories
# normal
SRCDIRS := common/ extras/ space/ vigo/ osc/
# Qt toolkit-related
TKSRCDS := QtVigo/
# unit tests (check)
CHKDIRS	:= check/ check/common/ check/osc/

### source files
SRCS	:= $(foreach DIR,$(SRCDIRS),$(wildcard $(DIR)*.c) $(wildcard $(DIR)*.cpp))
TKSRCS	:= $(foreach DIR,$(TKSRCDS),$(wildcard $(DIR)*.cpp))
MOCS	:= $(patsubst %.hpp,%.cc,$(wildcard $(TKSRCDS)*.hpp))

### check source files
CHKS	:= $(foreach DIR,$(CHKDIRS),$(wildcard $(DIR)*.cpp))

### Operating system specific
ifdef OS
OSTYPE	:= $(OS)
else
OSTYPE	:= $(shell uname)
endif
include makedef.$(OSTYPE)

### Toolkit
MOC	:= $(QTDIR)/bin/moc
TKDEFS	:= QT_CORE_LIB QT_GUI_LIB QT_SHARED

### preprocessor defines
DEFS	:= $(OSDEFS) $(TKDEFS)
ifeq ($(DEBUG),yes)
DEFS	+= _DEBUG
else
DEFS	+= NDEBUG
endif

### compiler
INCDIRS :=

CFLAGS	:= -pipe -W -Wall
ifeq ($(DEBUG),yes)
CFLAGS	+= -O0 -g
else
CFLAGS	+= -Os
endif

CFLAGS	+= $(foreach DEF,$(DEFS),-D$(DEF)) $(foreach DIR,$(INCDIRS),-I$(DIR))

CFLAGS	+= $(OSCFLGS)

### linker
LIBDIRS := $(OSLIBD)

LFLAGS	:= $(OSLFLGS)
LFLAGS	+= $(foreach DIR,$(LIBDIRS),-L$(DIR))

### object files
OBJS	:= $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SRCS)))
CHKOBJS := $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(CHKS)))
TKOBJS	:= $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(TKSRCS)))
MOCOBJS := $(patsubst %.hpp,%.mo,$(wildcard $(TKSRCDS)*.hpp))

### all files
OURSRCS	:= $(SRCS) $(TKSRCS) $(CHKS)
ALLSRCS	:= $(OURSRCS) $(MOCS)
OUROBJS	:= $(OBJS) $(TKOBJS) $(CHKOBJS)
ALLOBJS	:= $(OUROBJS) $(MOCOBJS)

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

.PHONY: all vigo check clean

LIBVIGO := libvigo.a
CHECKAPP:= check/check

all:	vigo

vigo:	$(LIBVIGO)
check:	$(CHECKAPP)

$(LIBVIGO): $(OBJS) $(TKOBJS) $(MOCOBJS)
	$(AR) rcs $(LIBVIGO) $(OBJS) $(TKOBJS) $(MOCOBJS)

$(CHECKAPP): $(LIBVIGO) $(CHKOBJS)
	$(CXX) -o $@ $(CHKOBJS) $(LFLAGS) -L. -lvigo -lcppunit -lpthread -ldl

clean:
	rm -f $(ALLOBJS) $(MOCS) $(LIBVIGO) $(CHECKAPP) $(DEPS)
	rm -rf docs/dox/html

help:
	echo "usage: make [all|vigo|check|clean]"

########################################################################
# dependencies

DEPS	:= $(foreach SRC,$(OURSRCS),$(SRC).d) $(foreach SRC,$(MOCS),$(SRC).dd)

.SUFFIXES: .cpp.d .c.d .cc.dd
%.cpp.d: %.cpp
	$(CXX) $(CFLAGS) -MM "$<" -MT "$(patsubst %.cpp,%.o,$<)" >$@
%.c.d:	%.c
	$(CC)  $(CFLAGS) -MM "$<" -MT "$(patsubst %.c,%.o,$<)" >$@

%.cc.dd: %.cc
	$(CXX) $(CFLAGS) -MM "$<" -MT "$(patsubst %.cc,%.mo,$<)" >$@

ifndef OMIT_DEPS
-include $(DEPS)
endif

# eof
