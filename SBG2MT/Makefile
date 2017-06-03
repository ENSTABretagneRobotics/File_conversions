PROGS = SBG2MT

CC = gcc

CFLAGS += -g
#CFLAGS += -O3
CFLAGS += -Wall
#CFLAGS += -Wextra -Winline

# For Windows/MinGW
#LDFLAGS += -lws2_32

LDFLAGS += -lm

default: $(PROGS)

OSCore.o: OSCore.c OSCore.h
	$(CC) $(CFLAGS) -c $<

OSTime.o: OSTime.c OSTime.h OSCore.o
	$(CC) $(CFLAGS) -c $<

OSMisc.o: OSMisc.c OSMisc.h OSTime.o
	$(CC) $(CFLAGS) -c $<

Main.o: Main.c OSMisc.o
	$(CC) $(CFLAGS) -c $<

SBG2MT: Main.o OSMisc.o OSTime.o OSCore.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(PROGS) $(PROGS:%=%.elf) $(PROGS:%=%.exe) *.o *.obj core *.gch
