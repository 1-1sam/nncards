cflags := -std=c99 -Wall -g -O2 -D_XOPEN_SOURCE -D_DEFAULT_SOURCE $(CFLAGS)
PREFIX ?= /usr
BINDIR ?= /bin

export cflags

build:
	make -C src

install: src/nncards
	install -v -d $(DESTDIR)$(PREFIX)$(BINDIR)
	install -v -m 755 src/nncards $(DESTDIR)$(PREFIX)$(BINDIR)

clean:
	rm -f src/nncards src/*.o
