PROGS = alldata2KML

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

Utils.o: Utils.c Utils.h
	$(CC) $(CFLAGS) -c $<

Globals.o: Globals.c Globals.h Utils.o
	$(CC) $(CFLAGS) -c $<

Main.o: Main.c Globals.o 
	$(CC) $(CFLAGS) -c $<

alldata2KML: Main.o Globals.o Utils.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(PROGS) $(PROGS).elf $(PROGS).exe *.o *.obj core *.gch
