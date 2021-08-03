CC ?= gcc
PKGCONFIG = $(shell which pkg-config)
CFLAGS = $(shell $(PKGCONFIG) --cflags gtk+-3.0)
LIBS = $(shell $(PKGCONFIG) --libs gtk+-3.0)
LDFLAGS = -lsqlite3 
INC = -I/usr/include/sqlite3
GLIB_COMPILE_RESOURCES = $(shell $(PKGCONFIG) --variable=glib_compile_resources gio-2.0)

SRC = main.c starcmd-app.c starcmd-main-window.c starcmd-command-view.c starcmd-command-window.c
BUILT_SRC = resources.c

OBJS = $(BUILT_SRC:.c=.o) $(SRC:.c=.o)

all: starcmd 

resources.c: starcmd.gresource.xml starcmd-main-window.glade starcmd-command-view.glade starcmd-command-window.glade
	$(GLIB_COMPILE_RESOURCES) starcmd.gresource.xml --target=$@ --sourcedir=. --generate-source

%.o: %.c
	$(CC) -c -o $(@F) $(CFLAGS) $<

starcmd: $(OBJS)
	$(CC) -rdynamic -o $(@F) $(OBJS) $(LIBS) $(LDFLAGS) $(INC)

clean:
	rm -f $(BUILT_SRC)
	rm -f $(OBJS)
	rm -f starcmd 
