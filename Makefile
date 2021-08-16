# Source: https://gitlab.gnome.org/NoraCodes/gdiceroller/-/blob/master/Makefile

# Install to /usr unless otherwise specified, such as `make PREFIX=/app`
PREFIX=/usr

# What to run to install various files
INSTALL=install
# Run to install the actual binary
INSTALL_PROGRAM=$(INSTALL)
# Run to install application data, with differing permissions
INSTALL_DATA=$(INSTALL) -m 644

# Directories into which to install the various files
bindir=$(DESTDIR)$(PREFIX)/bin
sharedir=$(DESTDIR)$(PREFIX)/share

# These targets have no associated build files
.PHONY: clean clean-all install uninstall

# App config
CC ?= gcc
PKGCONFIG = $(shell which pkg-config)
CFLAGS = $(shell $(PKGCONFIG) --cflags gtk+-3.0)
LIBS = $(shell $(PKGCONFIG) --libs gtk+-3.0)
LDFLAGS = -lsqlite3 -lcurl
INC = -I/usr/include/sqlite3
GLIB_COMPILE_RESOURCES = $(shell $(PKGCONFIG) --variable=glib_compile_resources gio-2.0)

SRC = src/main.c src/starcmd-app.c src/core/starcmd-db.c src/core/starcmd-image-client.c src/ui/starcmd-main-window.c src/ui/starcmd-command-window.c
BUILT_SRC = resources.c
DEPS = starcmd-app.h core/starcmd-db.h core/starcmd-image-client.h ui/starcmd-main-window.h ui/starcmd-command-window.h
OBJS = $(BUILT_SRC:.c=.o) $(SRC:.c=.o)

# Build the application
target/release/Starcmd: starcmd 

# Install onto the system
install: target/release/Starcmd 
	# Create bindir, if need be
	mkdir -p $(bindir)
	# Install binary
	$(INSTALL_PROGRAM) starcmd $(bindir)/org.h010dev.StarCMD
	# Create the sharedir and subfolders, if need be
	mkdir -p $(sharedir)/icons/hicolor/scalable/apps/
	mkdir -p $(sharedir)/applications/
	# mkdir -p $(sharedir)/metainfo/
	# Install icons
	$(INSTALL_DATA) data/icons/starcmd-icon-light.svg $(sharedir)/icons/hicolor/scalable/apps/org.h010dev.StarCMD.svg
	$(INSTALL_DATA) data/icons/starcmd-logo.png $(sharedir)/icons/hicolor/scalable/apps/org.h010dev.StarCMD-logo.png
	# Force icon cache refresh
	touch $(sharedir)/icons/hicolor
	# Install application meta-data
	# Install desktop file
	$(INSTALL_DATA) data/org.h010dev.StarCMD.desktop $(sharedir)/applications/org.h010dev.StarCMD.desktop

# Remove an existing install from the system
uninstall:
	# Remove the desktop file
	rm -f $(sharedir)/applications/org.h010dev.StarCMD.desktop
	# Remove the application meta-data
	# Remove the icon
	rm -f $(sharedir)/icons/hicolor/scalable/apps/org.h010dev.StarCMD.svg
	rm -f $(sharedir)/icons/hicolor/scalable/apps/org.h010dev.StarCMD-logo.png
	# Remove the binary
	rm -f $(bindir)/org.h010dev.StarCMD

clean-all: clean

starcmd: $(OBJS)
	$(CC) -rdynamic -o $(@F) $(OBJS) $(LIBS) $(LDFLAGS) $(INC)

resources.c: src/ui/resources/starcmd-main-window.glade src/ui/resources/starcmd-command-window.glade
	$(GLIB_COMPILE_RESOURCES) src/ui/resources/starcmd.gresource.xml --target=$@ --sourcedir=src/ui/resources --generate-source

%.o: %.c $(DEPS)
	$(CC) -c -o $(@F) $(CFLAGS) $<

clean:
	rm -f $(BUILT_SRC)
	rm -f $(OBJS)
	rm -f starcmd 
