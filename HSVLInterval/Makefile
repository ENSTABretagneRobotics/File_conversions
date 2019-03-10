# Makefile for Linux, tested with Ubuntu 16.04. 
# You might need to install C/C++ development tools by typing :
#    sudo apt-get install build-essential
# in a terminal.
# Additionally, you need to install OpenCV 2.4.13.

PROGS = HSVLInterval

CC = g++
CFLAGS += -g
#CFLAGS += -O3
CFLAGS += -Wall
CFLAGS += -Wextra -Winline

CFLAGS += -D _DEBUG

CFLAGS += -I.

LDFLAGS += -lopencv_core -lopencv_imgproc -lopencv_calib3d -lopencv_video -lopencv_features2d -lopencv_ml -lopencv_highgui -lopencv_objdetect -lopencv_contrib -lopencv_legacy
#LDFLAGS += -lm
#LDFLAGS += -lpthread -lrt

default: $(PROGS)

############################# PROGS #############################

HSVLInterval: Main.cpp
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(PROGS) $(PROGS:%=%.elf) $(PROGS:%=%.exe) *.o *.obj core *.gch
