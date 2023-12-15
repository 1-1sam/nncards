#include <stdio.h>
#include <unistd.h>
#include <getopt.h>

#include "tui.h"
#include "card-parse.h"

#define NNC_NEXT 0
#define NNC_PREV 1
#define NNC_FLIP 2
#define NNC_LAST 3
#define NNC_FRST 4
#define NNC_QUIT 5

#ifndef NNC_VERSION
#define NNC_VERSION "2.0"
#endif

typedef int flag_t;

struct nncards {
	flag_t run;
	char* cardfile;
	enum { TERM, DEFINITION } first_side;
	flag_t random;
	int initcard;
};

static void
_print_help(void) {

	printf("nncards - %s\n", NNC_VERSION);
	printf("Usage: nncards [options] file\n\n");
	printf("Options:\n");
	printf("	-r             --random        Randomize the order that the cards are shown in.\n");
	printf("	-t             --terms-first   Show terms first rather than definitions.\n");
	printf("	-c <number>    --at=<number>   Start at the nth card.\n");
	printf("	-h             --help          Print this help message.\n");
	printf("	-v             --version       Print program version.\n");

}

static void
_print_version(void) {

	printf("nncards - %s\n", NNC_VERSION);

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
		case TB_KEY_ARROW_RIGHT:  return NNC_NEXT;
		case TB_KEY_ARROW_LEFT:   return NNC_PREV;
		case TB_KEY_ARROW_UP:     return NNC_FLIP;
		case TB_KEY_ARROW_DOWN:   return NNC_FLIP;
		case TB_KEY_PGDN:         return NNC_LAST;
		case TB_KEY_PGUP:         return NNC_FRST;
		case TB_KEY_ESC:          return NNC_QUIT;
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

	int c;
	struct nncards nnc_return = {
		.run = 1,
		.cardfile = NULL,
		.first_side = DEFINITION,
		.random = 0,
		.initcard = 0,
	};
	struct option long_options[] = {
		{ "at", required_argument, 0, 'c' },
		{ "random", no_argument, 0, 'r' },
		{ "terms-first", no_argument, 0, 't' },
		{ "help", no_argument, 0, 'h' },
		{ "version", no_argument, 0, 'v' },
		{ 0, 0, 0, 0 }
	};

	while ((c = getopt_long(argc, argv, "c:rthv", long_options, NULL)) != -1 ) {

		switch (c) {
			case 'c':
				nnc_return.initcard = strtol(optarg, NULL, 10);
				break;
			case 'r':
				nnc_return.random = 1;
				break;
			case 't':
				nnc_return.first_side = TERM;
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
			default:
				fprintf(stderr, "Error parsing options\n");
				nnc_return.run = 0;
				return nnc_return;
		}
	}

	if (optind < argc) {
		nnc_return.cardfile = argv[optind];
	} else {
		_print_help();
		nnc_return.run = 0;
	}

	return nnc_return;
}

int
nnc_main_loop(struct nncards nncards) {

	int cardnum;
	struct card* deck;
	struct tb_event ev;
	int currcard;
	char* currstr;
	char* filename;

	if (access(nncards.cardfile, R_OK) != 0) {
		fprintf(stderr, "%s: Cannot be opened\n", nncards.cardfile);
		return -1;
	}

	/* cp_get_cardnum prints the error message itself */
	if ((cardnum = cp_get_cardnum(nncards.cardfile)) == -1)
		return -1;

	currcard = (nncards.initcard > 0 && nncards.initcard <= cardnum)
		? nncards.initcard -1 : 0;

	deck = cp_get_cards(nncards.cardfile, cardnum);

	if (deck == NULL)
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
	if ((filename = strrchr(nncards.cardfile, '/')) != NULL)
		filename++;
	else
		filename = nncards.cardfile;

	currstr = deck[currcard].side1;

	tb_init();

	for (;;) {

		tb_clear();

		tui_update_size();
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


