PROGS = MDM2CSV

CC = gcc
CXX = g++

#CFLAGS += -g -D _DEBUG
CFLAGS += -O3
CFLAGS += -Wall -Wno-unknown-pragmas -Wno-unused-parameter -Wextra
#CFLAGS += -Winline

CXXFLAGS += $(CFLAGS) -fpermissive

#LDFLAGS += -static-libgcc -static-libstdc++ -static

# For Windows/MinGW
#CFLAGS += ENABLE_GETTIMEOFDAY_WIN32
#LDFLAGS += -lws2_32
# For Linux
LDFLAGS += -lrt -lm

default: $(PROGS)

OSCore.o: OSCore.c OSCore.h
	$(CC) $(CFLAGS) -c $<

OSTime.o: OSTime.c OSTime.h OSCore.o
	$(CC) $(CFLAGS) -c $<

OSMisc.o: OSMisc.c OSMisc.h OSTime.o
	$(CC) $(CFLAGS) -c $<

Globals.o: Globals.cpp OSMisc.o
	$(CXX) $(CXXFLAGS) -c $<

Env.o: Env.cpp Globals.o
	$(CXX) $(CXXFLAGS) -c $<

Main.o: Main.cpp Env.o
	$(CXX) $(CXXFLAGS) -c $<

MDM2CSV: Main.o Env.o Globals.o OSMisc.o OSTime.o OSCore.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(PROGS) $(PROGS:%=%.elf) $(PROGS:%=%.exe) *.o *.obj core *.gch
