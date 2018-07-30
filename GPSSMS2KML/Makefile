PROGS = GPSSMS2KML

CC = gcc

#CFLAGS += -g -D _DEBUG
CFLAGS += -O3
CFLAGS += -Wall -Wno-unknown-pragmas -Wno-unused-parameter -Wextra
#CFLAGS += -Winline

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

Main.o: Main.c OSMisc.o
	$(CC) $(CFLAGS) -c $<

GPSSMS2KML: Main.o OSMisc.o OSTime.o OSCore.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(PROGS) $(PROGS:%=%.elf) $(PROGS:%=%.exe) *.o *.obj core *.gch
