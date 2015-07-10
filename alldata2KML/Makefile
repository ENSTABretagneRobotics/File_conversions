PROGS = alldata2KML

#CC = /home/user/armadeus-4.0/buildroot/output/build/staging_dir/usr/bin/arm-linux-gcc
#CC = arm-linux-gcc
CC = gcc
#CFLAGS += -g -fpermissive -D _DEBUG
CFLAGS += -O3 -fpermissive
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
