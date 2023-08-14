#include <stdio.h>
#include <unistd.h>

#include "tui.h"
#include "card-parse.h"

struct nncards {
	enum run { RUN, NORUN } run;
	char* cardfile;
	enum opt_first_side { TERM, DEFINITION } opt_first_side;
	int opt_random;
};

static void
_print_help(void) {

	printf("nncards - 1.0\n");
	printf("Usage: nncards [-trhv] file\n");
	printf("\n");
	printf("Options:\n");
	printf("	-t	Show terms first rather than definitions.\n");
	printf("	-r	Randomize the order that the cards are shown in.\n");
	printf("	-h	Print this help message.\n");
	printf("	-v	Print program version.\n");

}

static void
_print_version(void) {

	printf("nncards - 1.0\n");

}

struct nncards
nnc_init(int argc, char** argv) {

	struct nncards nnc_return = {
		.run = RUN,
		.cardfile = NULL,
		.opt_first_side = DEFINITION,
		.opt_random = 0
	};

	int c;

	while ((c = getopt(argc, argv, "trhv")) != -1) {
		switch (c) {
			case 't':
				nnc_return.opt_first_side = TERM;
				break;
			case 'r':
				nnc_return.opt_random = 1;
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

	if (cp_open_test(nncards.cardfile)) {
		fprintf(stderr, "%s: Cannot be opened\n", nncards.cardfile);
		return -1;
	}

	int cardnum = cp_get_cardnum(nncards.cardfile);
	struct card cards[cardnum];
	struct tb_event ev;
	int currcard = 0;
	char* currstr;

	if (cp_get_cards(cards, nncards.cardfile) == -1)
		return -1;

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

		switch (ev.ch) {
			/* Next card */
			case '?':
			case 'c':
				if (currcard < cardnum - 1)
					currstr = cards[++currcard].side1;
				break;
			/* Previous card */
			case 'z':
				if (currcard > 0)
					currstr = cards[--currcard].side1;
				break;
			/* Flip card */
			case 'x':
				currstr = (currstr == cards[currcard].side1)
					? cards[currcard].side2 : cards[currcard].side1;
				break;
			/* Go to beginning card */
			case 'a':
				currstr = cards[currcard = 0].side1;
				break;
			/* Go to final card */
			case 'd':
				currstr = cards[currcard = cardnum - 1].side1;
				break;
			/* Quit */
			case 'q':
				tb_shutdown();
				return 0;
		}
	}
}

