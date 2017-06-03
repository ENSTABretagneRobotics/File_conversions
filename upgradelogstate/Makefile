PROGS = upgradelogstate

CC = gcc

#CFLAGS += -g -D _DEBUG
CFLAGS += -O3
CFLAGS += -Wall
CFLAGS += -Wextra

#CFLAGS += -I.
#LDFLAGS += -lpthread
#LDFLAGS += -lrt
LDFLAGS += -lm

default: $(PROGS)

OSCore.o: OSCore.c OSCore.h
	$(CC) $(CFLAGS) -c $<

OSMisc.o: OSMisc.c OSMisc.h OSTime.o
	$(CC) $(CFLAGS) -c $<

OSTime.o: OSTime.c OSTime.h OSCore.o
	$(CC) $(CFLAGS) -c $<

Main.o: Main.c OSMisc.o OSTime.o OSCore.o 
	$(CC) $(CFLAGS) -c $<

upgradelogstate: Main.o OSMisc.o OSTime.o OSCore.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(PROGS) $(PROGS).elf $(PROGS).exe *.o *.obj core *.gch
