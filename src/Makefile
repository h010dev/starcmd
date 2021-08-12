CC ?= gcc
PKGCONFIG = $(shell which pkg-config)
CFLAGS = $(shell $(PKGCONFIG) --cflags gtk+-3.0)
LIBS = $(shell $(PKGCONFIG) --libs gtk+-3.0)
LDFLAGS = -lsqlite3 
INC = -I/usr/include/sqlite3
GLIB_COMPILE_RESOURCES = $(shell $(PKGCONFIG) --variable=glib_compile_resources gio-2.0)

SRC = main.c starcmd-app.c core/starcmd-db.c ui/starcmd-main-window.c ui/starcmd-command-view.c ui/starcmd-command-window.c ui/starcmd-image-search-window.c
BUILT_SRC = resources.c

OBJS = $(BUILT_SRC:.c=.o) $(SRC:.c=.o)

all: starcmd 

resources.c: ui/resources/starcmd-main-window.glade ui/resources/starcmd-command-view.glade ui/resources/starcmd-command-window.glade ui/resources/starcmd-image-search-window.glade
	$(GLIB_COMPILE_RESOURCES) ui/resources/starcmd.gresource.xml --target=$@ --sourcedir=ui/resources --generate-source

%.o: %.c
	$(CC) -c -o $(@F) $(CFLAGS) $<

starcmd: $(OBJS)
	mv starcmd-db.o core/
	mv starcmd-main-window.o starcmd-command-view.o starcmd-command-window.o starcmd-image-search-window.o ui/
	$(CC) -rdynamic -o $(@F) $(OBJS) $(LIBS) $(LDFLAGS) $(INC)

clean:
	rm -f $(BUILT_SRC)
	rm -f $(OBJS)
	rm -f starcmd 
