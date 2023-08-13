#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#define MY_BUFSIZE 1024
#define CARD_STR_MAX 255

struct card {
	char side1[CARD_STR_MAX]; /* By default, these are definitions */
	char side2[CARD_STR_MAX]; /* By default, these are terms */
};

int
cp_get_cardnum(char* filename) {

	FILE* cardfile = fopen(filename, "r");
	int cardnum = 0;
	char line[MY_BUFSIZE];

	while (fgets(line, MY_BUFSIZE, cardfile) != NULL) {

		if (line[0] == '#' || line[0] == '\n')
			continue;

		if (strchr(line, ':'))
			cardnum++;
	}

	fclose(cardfile);
	return cardnum;

}

int
cp_get_cards(struct card* cards, char* filename) {

	FILE* cardfile = fopen(filename, "r");
	int card_count = 0;
	char line[MY_BUFSIZE];
	enum line_state { TERM, DEFINITION, WHITESPACE } line_state;

	while (fgets(line, MY_BUFSIZE, cardfile) != NULL) {

		char *l = line;
		line_state = TERM;
		int ti = 0;
		int di = 0;

		if (*l == '#' || *l == '\n')
			continue;

		for (; *l; l++) {

			if (*l == '\n' && (line_state == TERM || line_state == WHITESPACE)) {
				fprintf(stderr, "%s: Cannot be parsed, verify the file is formatted correctly\n", filename);
				return -1;
			}

			if (di >= CARD_STR_MAX || ti >= CARD_STR_MAX) {
				fprintf(stderr, "%s: Term or definition exceeded 254 character limit\n", filename);
				return -1;
			}

			if (*l == ':' && line_state == TERM) {
				cards[card_count].side2[ti] = '\0';
				line_state = WHITESPACE;
			} else if (!isblank(*l) && line_state == WHITESPACE) {
				line_state = DEFINITION;
			} else if (*l == '\n') {
				cards[card_count].side1[di] = '\0';
				break;
			}

			if (line_state == TERM)
				cards[card_count].side2[ti++] = *l;
			else if (line_state == DEFINITION)
				cards[card_count].side1[di++] = *l;
		}
		card_count++;
	}

	fclose(cardfile);

	return 0;
}

int
cp_open_test(char* filename) {

	FILE* file = fopen(filename, "r");

	if (file == NULL)
		return -1;

	return 0;

}

void
cp_card_shuffle(struct card* cards, int cardnum) {

	srand(time(NULL));

	struct card tmpcards[cardnum];
	memcpy(tmpcards, cards, sizeof(struct card) * cardnum);

	int r;

	for (int i = 0, cardsleft = cardnum; cardsleft > 0; i++, cardsleft--) {
		r = rand() % cardsleft;

		cards[i] = tmpcards[r];
		tmpcards[r] = tmpcards[cardsleft - 1];
	}
}

void
cp_side_swap(struct card* cards, int cardnum) {

	char tmpstr[CARD_STR_MAX];

	for (int i = 0; i < cardnum; i++) {

		memset(tmpstr, 0, CARD_STR_MAX);
		memcpy(tmpstr, cards[i].side2, CARD_STR_MAX);

		memset(cards[i].side2, 0, CARD_STR_MAX);
		memcpy(cards[i].side2, cards[i].side1, CARD_STR_MAX);

		memset(cards[i].side1, 0, CARD_STR_MAX);
		memcpy(cards[i].side1, tmpstr, CARD_STR_MAX);

	}

}
