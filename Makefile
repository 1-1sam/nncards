PREFIX ?= /usr/local
BINDIR ?= /bin

build:
	make -C src

install: src/nncards
	install -v -d $(DESTDIR)$(PREFIX)$(BINDIR)
	install -v -m 755 src/nncards $(DESTDIR)$(PREFIX)$(BINDIR)

clean:
	make clean -C src
