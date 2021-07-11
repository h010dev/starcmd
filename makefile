# source: https://prognotes.net/2015/07/gtk-3-glade-c-programming-template/

# change application name here (executable output name)
TARGET=starcmd

# compiler
CC=gcc
# debug
DEBUG=-g
# optimisation
OPT=-O0
# warnings
WARN=-Wall

PTHREAD=-pthread

CCFLAGS=$(DEBUG) $(OPT) $(WARN) $(PTHREAD) -pipe

GTKLIB=`pkg-config --cflags --libs gtk+-3.0`

# linker
LD=gcc
LDFLAGS=$(PTHREAD) $(GTKLIB) -export-dynamic

OBJS=main.o

all: $(OBJS)
	  $(LD) -o $(TARGET) $(OBJS) $(LDFLAGS)

main.o: src/main.c
	  $(CC) -c $(CCFLAGS) src/main.c $(GTKLIB) -o main.o

clean:
	  rm -f *.o $(TARGET)
