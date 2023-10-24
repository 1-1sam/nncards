#include <stdio.h>
#include <unistd.h>

#include "tui.h"
#include "card-parse.h"

#define NNC_NEXT 0
#define NNC_PREV 1
#define NNC_FLIP 2
#define NNC_LAST 3
#define NNC_FRST 4
#define NNC_QUIT 5

typedef int flag_t;

struct nncards {
	flag_t run;
	char* cardfile;
	enum first_side { TERM, DEFINITION } first_side;
	flag_t random;
	int initcard;
};

static void
_print_help(void) {

	printf("nncards - 1.0\n");
	printf("Usage: nncards [-rthv] [-c n] file\n");
	printf("\n");
	printf("Options:\n");
	printf("	-r    Randomize the order that the cards are shown in.\n");
	printf("	-t    Show terms first rather than definitions.\n");
	printf("	-c n  Start at the nth card.\n");
	printf("	-h    Print this help message.\n");
	printf("	-v    Print program version.\n");

}

static void
_print_version(void) {

	printf("nncards - 1.0\n");

}

static int
_event_parse(struct tb_event ev) {

	switch (ev.ch) {
		case 'c': return NNC_NEXT;
		case 'l': return NNC_NEXT;
		case 'z': return NNC_PREV;
		case 'h': return NNC_PREV;
		case 'x': return NNC_FLIP;
		case ' ': return NNC_FLIP;
		case 'd': return NNC_LAST;
		case 'a': return NNC_FRST;
		case 'q': return NNC_QUIT;
		default: break;
	}

	switch (ev.key) {
		case TB_KEY_ARROW_RIGHT: return NNC_NEXT;
		case TB_KEY_ARROW_LEFT: return NNC_PREV;
		case TB_KEY_ARROW_UP: return NNC_FLIP;
		case TB_KEY_ARROW_DOWN: return NNC_FLIP;
		case TB_KEY_PGDN: return NNC_LAST;
		case TB_KEY_PGUP: return NNC_FRST;
		case TB_KEY_ESC: return NNC_QUIT;
		default: break;
	}

	return -1;

}

static void
_burn_deck(struct card* deck, int cardnum) {

	for (int i = 0; i < cardnum; i++) {
		free(deck[i].side1);
		free(deck[i].side2);
	}

	free(deck);

}

struct nncards
nnc_init(int argc, char** argv) {

	struct nncards nnc_return = {
		.run = 1,
		.cardfile = NULL,
		.first_side = DEFINITION,
		.random = 0,
		.initcard = 0,
	};

	int c;

	while ((c = getopt(argc, argv, "rtc:hv")) != -1) {
		switch (c) {
			case 'r':
				nnc_return.random = 1;
				break;
			case 't':
				nnc_return.first_side = TERM;
				break;
			case 'c':
				nnc_return.initcard = atoi(optarg);
				break;
			case 'h':
				nnc_return.run = 0;
				_print_help();
				return nnc_return;
			case 'v':
				nnc_return.run = 0;
				_print_version();
				return nnc_return;
			case '?':
				nnc_return.run = 0;
				return nnc_return;
		}
	}

	if (!(nnc_return.cardfile = argv[optind])) {
		fprintf(stderr, "No argument was passed\n");
		nnc_return.run = 0;
	}

	return nnc_return;
}

int
nnc_main_loop(struct nncards nncards) {

	if (access(nncards.cardfile, R_OK)) {
		fprintf(stderr, "%s: Cannot be opened\n", nncards.cardfile);
		return -1;
	}

	int cardnum;
	struct card* deck;
	struct tb_event ev;
	int currcard;
	char* currstr;
	char* filename;

	/* cp_get_cardnum prints the error message itself */
	if ((cardnum = cp_get_cardnum(nncards.cardfile)) == -1)
		return -1;

	currcard = (nncards.initcard > 0 && nncards.initcard <= cardnum)
		? nncards.initcard -1 : 0;

	deck = cp_get_cards(nncards.cardfile, cardnum);

	if (!deck)
		return -1;

	if (nncards.first_side == TERM)
		cp_side_swap(deck, cardnum);

	if (nncards.random) {
		if (cp_card_shuffle(deck, cardnum) == -1)
			return -1;
	}

	/*
	 * Get a pointer to the character after the last slash, which should point
	 * to file name.
	 */
	if ((filename = strrchr(nncards.cardfile, '/')))
		filename++;
	else
		filename = nncards.cardfile;

	currstr = deck[currcard].side1;

	tb_init();

	for (;;) {

		tb_clear();
		tui_draw_card(currstr);
		tui_draw_info(filename, currcard + 1, cardnum);

		tb_poll_event(&ev);

		switch (_event_parse(ev)) {
			case NNC_NEXT:
				if (currcard < cardnum - 1)
					currstr = deck[++currcard].side1;
				break;
			case NNC_PREV:
				if (currcard > 0)
					currstr = deck[--currcard].side1;
				break;
			case NNC_FLIP:
				currstr = (currstr == deck[currcard].side1)
					? deck[currcard].side2 : deck[currcard].side1;
				break;
			case NNC_FRST:
				currstr = deck[currcard = 0].side1;
				break;
			case NNC_LAST:
				currstr = deck[currcard = cardnum - 1].side1;
				break;
			case NNC_QUIT:
				tb_shutdown();
				_burn_deck(deck, cardnum);
				return 0;
			default: break;
		}
	}
}


