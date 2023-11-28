nncards_cflags := -O2 -std=c99 -D_GNU_SOURCE -Wall -Wextra -pedantic $(CFLAGS)
nncards_objects := $(patsubst %.c,%.o,$(wildcard *.c))
nncards_ldflags := $(LDFLAGS)
nncards_ldlibs := -lm

all: nncards

nncards: $(nncards_objects) termbox.h
	$(CC) $(nncards_cflags) $(nncards_objects) $(LDFLAGS) $(nncards_ldlibs) -o nncards

$(nncards_objects): %.o: %.c
	$(CC) -c $(nncards_cflags) $< -o $@

clean:
	rm -fr nncards $(nncards_objects)
