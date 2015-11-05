PROGS = LocalWaypoints2WGS

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

Utils.o: Utils.cpp Utils.h
	$(CC) $(CFLAGS) -c $<

Waypoints.o: Waypoints.cpp Utils.o
	$(CC) $(CFLAGS) -c $<

Globals.o: Globals.cpp Waypoints.o
	$(CC) $(CFLAGS) -c $<

Env.o: Env.cpp Globals.o
	$(CC) $(CFLAGS) -c $<

Main.o: Main.cpp Env.o
	$(CC) $(CFLAGS) -c $<

LocalWaypoints2WGS: Main.o Env.o Globals.o Waypoints.o Utils.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(PROGS) $(PROGS:%=%.elf) $(PROGS:%=%.exe) *.o *.obj core *.gch
