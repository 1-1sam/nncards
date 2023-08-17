CFLAGS := -std=c99 -Wall -g -O2 -D_XOPEN_SOURCE -D_DEFAULT_SOURCE $(CFLAGS)

nncards: main.o card-parse.o tui.o run.o
	$(CC) $(CFLAGS) main.o card-parse.o tui.o run.o -o nncards
main.o: main.c
	$(CC) $(CFLAGS) -c main.c
run.o: run.c
	$(CC) $(CFLAGS) -c run.c
card-parse.o: card-parse.c
	$(CC) $(CFLAGS) -c card-parse.c
tui.o: tui.c
	$(CC) $(CFLAGS) -c tui.c

clean:
	rm -f nncards *.o
