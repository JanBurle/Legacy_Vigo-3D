SRCS	:= tunnel.cpp ../common/socket.cpp ../common/async.cpp ../common/types.cpp
OBJS	:= tunnel.o ../common/socket.o ../common/async.o ../common/types.o

CFLAGS	:= -pipe -W -Wall -O0 -ggdb

.SUFFIXES: .cpp .o

%.o:	%.cpp
	$(CXX) $(CFLAGS) -D_DEBUG -c -o $@ $<

.PHONY: all clean

all:	tunnel

clean:
	rm -f tunnel $(OBJS)

tunnel: $(OBJS)
	$(CXX) $(CFLAGS) -o $@ $(OBJS)
