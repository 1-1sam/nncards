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

struct nncards {
	enum run { RUN, NORUN } run;
	char* cardfile;
	enum opt_first_side { TERM, DEFINITION } opt_first_side;
	int opt_random;
	int opt_initcard;
};

static void
_print_help(void) {

	printf("nncards - 1.0\n");
	printf("Usage: nncards [-trhv] [-c n] file\n");
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

struct nncards
nnc_init(int argc, char** argv) {

	struct nncards nnc_return = {
		.run = RUN,
		.cardfile = NULL,
		.opt_first_side = DEFINITION,
		.opt_random = 0,
		.opt_initcard = 0
	};

	int c;

	while ((c = getopt(argc, argv, "rtc:hv")) != -1) {
		switch (c) {
			case 'r':
				nnc_return.opt_random = 1;
				break;
			case 't':
				nnc_return.opt_first_side = TERM;
				break;
			case 'c':
				nnc_return.opt_initcard = atoi(optarg);
				break;
			case 'h':
				nnc_return.run = NORUN;
				_print_help();
				return nnc_return;
			case 'v':
				nnc_return.run = NORUN;
				_print_version();
				return nnc_return;
			case '?':
				nnc_return.run = NORUN;
				return nnc_return;
		}
	}

	if ((nnc_return.cardfile = argv[optind]) == NULL) {
		fprintf(stderr, "No argument was passed\n");
		nnc_return.run = NORUN;
	}

	return nnc_return;
}

int
nnc_main_loop(struct nncards nncards) {

	if (access(nncards.cardfile, R_OK)) {
		fprintf(stderr, "%s: Cannot be opened\n", nncards.cardfile);
		return -1;
	}

	int cardnum = cp_get_cardnum(nncards.cardfile);
	struct card cards[cardnum];
	struct tb_event ev;
	int currcard = (nncards.opt_initcard > 0 && nncards.opt_initcard <= cardnum)
		? nncards.opt_initcard - 1 : 0;
	char* currstr;

	if (cp_get_cards(cards, nncards.cardfile) == -1) {
		fprintf(stderr, "%s: Not a valid card file\n", nncards.cardfile);
		return -1;
	}

	if (nncards.opt_first_side == TERM)
		cp_side_swap(cards, cardnum);

	if (nncards.opt_random)
		cp_card_shuffle(cards, cardnum);

	currstr = cards[currcard].side1;

	tb_init();

	for (;;) {

		tb_clear();
		tui_draw_card(currstr);
		tui_draw_info(nncards.cardfile, currcard + 1, cardnum);

		tb_poll_event(&ev);

		switch (_event_parse(ev)) {
			case NNC_NEXT:
				if (currcard < cardnum - 1)
					currstr = cards[++currcard].side1;
				break;
			case NNC_PREV:
				if (currcard > 0)
					currstr = cards[--currcard].side1;
				break;
			case NNC_FLIP:
				currstr = (currstr == cards[currcard].side1)
					? cards[currcard].side2 : cards[currcard].side1;
				break;
			case NNC_FRST:
				currstr = cards[currcard = 0].side1;
				break;
			case NNC_LAST:
				currstr = cards[currcard = cardnum - 1].side1;
				break;
			case NNC_QUIT:
				tb_shutdown();
				return 0;
			default: break;
		}
	}
}

