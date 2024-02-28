#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

#include "card-parse.h"
#include "run.h"
#include "tui.h"

#ifndef NNC_VERSION
#  define NNC_VERSION "3.1"
#endif

static void
_print_help(void) {

	printf("nncards - %s\n", NNC_VERSION);
	printf("Usage: nncards [-trhuv] FILE...\n\n");
	printf("Options:\n");
	printf(" -r    --random        Randomize the order that the cards are shown in.\n");
	printf(" -t    --terms-first   Show terms first rather than definitions.\n");
	printf(" -h    --help          Print this help message.\n");
	printf(" -u    --usage         Print program usage message.\n");
	printf(" -v    --version       Print program version.\n");

}

static void
_print_usage(void) {

	printf("Usage: nncards [-trhuv] FILE...\n");

}

static void
_print_version(void) {

	printf("nncards - %s\n", NNC_VERSION);

}

struct nncards
nnc_init(int argc, char** argv) {

	int c;
	struct nncards nncards = {
		.run_state = RUN,
		.cardfiles = NULL,
		.filenum = 0,
		.first_side = DEFINITION,
		.random = 0,
	};

	struct option long_options[] = {
		{ "random", no_argument, 0, 'r' },
		{ "terms-first", no_argument, 0, 't' },
		{ "help", no_argument, 0, 'h' },
		{ "usage", no_argument, 0, 'u' },
		{ "version", no_argument, 0, 'v' },
		{ 0, 0, 0, 0 }
	};

	while ((c = getopt_long(argc, argv, "rthuv", long_options, NULL)) != -1 ) {

		switch (c) {
			case 'r':
				nncards.random = 1;
				break;
			case 't':
				nncards.first_side = TERM;
				break;
			case 'h':
				nncards.run_state = NORUN;
				_print_help();
				return nncards;
			case 'u':
				nncards.run_state = NORUN;
				_print_usage();
				return nncards;
			case 'v':
				nncards.run_state = NORUN;
				_print_version();
				return nncards;
			case '?':
				nncards.run_state = ERROR;
				return nncards;
			default:
				fprintf(stderr, "Error parsing options\n");
				nncards.run_state = ERROR;
				return nncards;
		}
	}

	/* No file arguments were given */
	if (optind == argc) {
		_print_help();
		nncards.run_state = ERROR;
		return nncards;
	}

	nncards.filenum = argc - optind;
	nncards.cardfiles = &argv[optind];

	return nncards;
}

int
nnc_main_loop(struct nncards nncards) {

	struct deck deck;
	int currcard = 0;
	char* currstr;

	for (int i = 0; i < nncards.filenum; i++) {
		if (access(nncards.cardfiles[i], R_OK) != 0) {
			fprintf(stderr, "%s: Cannot be opened\n", nncards.cardfiles[i]);
			return 1;
		}
	}

	if (cp_get_cards(&deck, nncards.cardfiles, nncards.filenum) == -1) {
		return 1;
	}

	if (nncards.first_side == TERM) {
		cp_side_swap(deck);
	}

	if (nncards.random) {
		if (cp_card_shuffle(deck) == -1) {
			return 1;
		}
	}

	currstr = deck.cards[0].side1;

	tui_init();

	for (;;) {

		tui_clear();

		tui_update_size();
		tui_draw_card(currstr);
		tui_draw_info(currcard, deck.cardnum, nncards.cardfiles,
			nncards.filenum);

		switch (tui_poll_event()) {
			case NEXT:
				if (currcard < deck.cardnum - 1)
					currstr = deck.cards[++currcard].side1;
				break;
			case PREV:
				if (currcard > 0)
					currstr = deck.cards[--currcard].side1;
				break;
			case FLIP:
				currstr = (currstr == deck.cards[currcard].side1)
					? deck.cards[currcard].side2 : deck.cards[currcard].side1;
				break;
			case FIRST:
				currstr = deck.cards[currcard = 0].side1;
				break;
			case LAST:
				currstr = deck.cards[currcard = deck.cardnum - 1].side1;
				break;
			case QUIT:
				tui_shutdown();
				cp_free_cards(deck);
				return 0;
			default: break;
		}
	}
}


