PREFIX ?= /usr/local
BINDIR ?= /bin
MANDIR ?= /man/man1

build:
	make -C src

install: src/nncards
	install -v -d $(DESTDIR)$(PREFIX)$(BINDIR)
	install -v -m 755 src/nncards $(DESTDIR)$(PREFIX)$(BINDIR)
	install -v -d $(DESTDIR)$(PREFIX)$(MANDIR)
	install -v -m 644 man/nncards.1 $(DESTDIR)$(PREFIX)$(MANDIR)

clean:
	make clean -C src
