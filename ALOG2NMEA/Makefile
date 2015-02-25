# Makefile for Linux (to test with Ubuntu 10.10). 
# You might need to install C/C++ development tools by typing :
#    sudo apt-get install build-essential
# in a terminal.

PROGS = ALOG2NMEA

CC = gcc
#CFLAGS += -g
CFLAGS += -O3
CFLAGS += -Wall
#CFLAGS += -Wextra -Winline

#CFLAGS += -D _DEBUG 

CFLAGS += -I.

LDFLAGS += -lrt -lm

default: $(PROGS)

############################# PROGS #############################

Utils.o: Utils.c Utils.h
	$(CC) $(CFLAGS) -c $<

Main.o: Main.c Utils.o
	$(CC) $(CFLAGS) -c $<

ALOG2NMEA: Main.o Utils.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(PROGS) $(PROGS:%=%.elf) $(PROGS:%=%.exe) *.o *.obj core *.gch
