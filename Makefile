PREFIX ?= /usr/local
BINDIR ?= /bin
MANDIR ?= /man

build:
	$(MAKE) -C src

install: src/nncards
	install -v -d $(DESTDIR)$(PREFIX)$(BINDIR)
	install -v -m 755 src/nncards $(DESTDIR)$(PREFIX)$(BINDIR)

install-man: man/nncards.1
	install -v -d $(DESTDIR)$(PREFIX)$(MANDIR)/man1
	install -v -m 644 man/nncards.1 $(DESTDIR)$(PREFIX)$(MANDIR)/man1

clean:
	$(MAKE) clean -C src

.PHONY: build install install-man clean
