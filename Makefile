cflags := -std=c99 -Wall -g -O2 -D_XOPEN_SOURCE -D_DEFAULT_SOURCE $(CFLAGS)
PREFIX ?= /usr/local
BINDIR ?= /bin
DOCDIR ?= /doc/nncards-2.0
MANDIR ?= /man/man1

build:
	export cflags
	make -C src

install: src/nncards
	install -v -d $(DESTDIR)$(PREFIX)$(BINDIR)
	install -v -m 755 src/nncards $(DESTDIR)$(PREFIX)$(BINDIR)

install-doc: CHANGELOG LICENSE README.md example.cards man/nncards.1
	install -v -d $(DESTDIR)$(PREFIX)$(DOCDIR)
	install -v -m 644 -t $(DESTDIR)$(PREFIX)$(DOCDIR) CHANGELOG LICENSE README.md example.cards
	install -v -d $(DESTDIR)$(PREFIX)$(MANDIR)
	install -v -m 644 man/nncards.1 $(DESTDIR)$(PREFIX)$(MANDIR)

clean:
	rm -f src/nncards src/*.o
