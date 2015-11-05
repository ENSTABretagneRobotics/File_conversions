PROGS = LocalBoxes2KML

CC = g++
CFLAGS += -g -fpermissive
#CFLAGS += -O3 -fpermissive
CFLAGS += -Wall
#CFLAGS += -Wextra -Winline

# For Windows/MinGW
#LDFLAGS += -lws2_32
# For Linux
#LDFLAGS += -lrt -lm

default: $(PROGS)

OSCore.o: OSCore.c OSCore.h
	$(CC) $(CFLAGS) -c $<

OSTime.o: OSTime.c OSTime.h OSCore.o
	$(CC) $(CFLAGS) -c $<

OSMisc.o: OSMisc.c OSMisc.h OSTime.o
	$(CC) $(CFLAGS) -c $<

Globals.o: Globals.cpp OSMisc.o
	$(CC) $(CFLAGS) -c $<

Env.o: Env.cpp Globals.o
	$(CC) $(CFLAGS) -c $<

Main.o: Main.cpp Env.o
	$(CC) $(CFLAGS) -c $<

LocalBoxes2KML: Main.o Env.o Globals.o OSMisc.o OSTime.o OSCore.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(PROGS) $(PROGS:%=%.elf) $(PROGS:%=%.exe) *.o *.obj core *.gch
