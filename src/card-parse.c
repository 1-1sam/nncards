#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#define MY_BUFSIZE 1024

struct card {
	char* side1;
	char* side2;
};

struct deck {
	struct card* cards;
	int cardnum;
};

static int
_get_cardnum(char* filename) {

	FILE* cardfile = fopen(filename, "r");
	int cardnum = 0;
	char* line = NULL;
	size_t linelen = 0;

	while (getline(&line, &linelen, cardfile) != -1) {

		if (line[0] == '#' || line[0] == '\n') {
			continue;
		}

		if (strchr(line, ':') == NULL) {
			fprintf(stderr, "Bad line: %d\n", cardnum + 1);
			free(line);
			fclose(cardfile);
			return -1;
		}

		cardnum++;

	}

	free(line);
	fclose(cardfile);

	return cardnum;

}

int
cp_get_cards(struct deck* deck, char** files, int filenum) {

	char* line = NULL;
	size_t linelen;
	int cc = 0;

	deck->cardnum = 0;

	for (int i = 0; i < filenum; i++) {

		int ccn;

		if ((ccn = _get_cardnum(files[i])) == -1) {
			return -1;
		}

		deck->cardnum += ccn;

	}

	deck->cards = malloc(sizeof(struct card) * deck->cardnum);

	if (deck->cards == NULL) {
		fprintf(stderr, "Could not allocate memory\n");
		return -1;
	}

	for (int i = 0; i < filenum; i++) {

		FILE* file = fopen(files[i], "r");
		char *d, *t;

		while (getline(&line, &linelen, file) != -1) {

			if (line[0] == '#' || line[0] == '\n')
				continue;

			*(strchr(line, '\n')) = '\0';

			/* Tabs mess up the TUI rendering, replace them with spaces */
			for (int i = 0; line[i] != '\0'; i++) {
				if (line[i] == '\t')
					line[i] = ' ';
			}

			/* Getting side1 (the definition) */
			if ((d = strchr(line, ':')) == NULL)
				continue;

			while (isblank(*(++d)))
				;

			 deck->cards[cc].side1 = strdup(d);

			if (deck->cards[cc].side1 == NULL) {
				fprintf(stderr, "Could not allocate memory\n");
				for (int i = 0; i < cc; i++) {
					free(deck->cards[i].side1);
					free(deck->cards[i].side2);
				}
				free(deck->cards);
				fclose(file);
				return -1;
			}

			/* Getting side2 (the term) */
			t = line;

			*(strchr(t, ':')) = '\0';

			deck->cards[cc++].side2 = strdup(t);

			if (deck->cards[cc - 1].side2 == NULL) {
				fprintf(stderr, "Could not allocate memory\n");
				for (int i = 0; i < cc - 1; i++) {
					free(deck->cards[i].side1);
					free(deck->cards[i].side2);
				}
				free(deck->cards[cc - 1].side1);
				free(deck->cards);
				fclose(file);
				return -1;
			}

		}

		fclose(file);

	}

	free(line);

	return 0;

}

int
cp_card_shuffle(struct deck deck) {

	struct card* tmpcards;
	int r;

	srand(time(NULL));

	tmpcards = malloc(sizeof(struct card) * deck.cardnum);

	if (tmpcards == NULL) {
		fprintf(stderr, "Failed to allocate memory\n");
		return -1;
	}

	memcpy(tmpcards, deck.cards, sizeof(struct card) * deck.cardnum);

	for (int i = 0, cardsleft = deck.cardnum; cardsleft > 0; i++, cardsleft--) {

		r = rand() % cardsleft;

		deck.cards[i] = tmpcards[r];
		tmpcards[r] = tmpcards[cardsleft - 1];
	}

	free(tmpcards);

	return 0;
}

void
cp_side_swap(struct deck deck) {

	char* p;

	for (int i = 0; i < deck.cardnum; i++) {
		p = deck.cards[i].side1;
		deck.cards[i].side1 = deck.cards[i].side2;
		deck.cards[i].side2 = p;
	}
}
