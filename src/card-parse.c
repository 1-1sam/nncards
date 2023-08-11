#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

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
	int ti, di;

	while (fgets(line, MY_BUFSIZE, cardfile) != NULL) {

		line_state = TERM;
		ti = 0;
		di = 0;

		if (line[0] == '#' || line[0] == '\n')
			continue;

		for (int i = 0; line[i]; i++) {

			if (line[i] == '\n' && (line_state == TERM || line_state == WHITESPACE)) {
				fprintf(stderr, "%s: Cannot be parsed, verify the file is formatted correctly\n", filename);
				return -1;
			}

			if (di >= CARD_STR_MAX || ti >= CARD_STR_MAX) {
				fprintf(stderr, "%s: Term or definition exceeded 254 character limit\n", filename);	
				return -1;
			}

			if (line[i] == ':' && line_state == TERM) {
				cards[card_count].side2[ti] = '\0';
				line_state = WHITESPACE;	
			} else if (line[i] != ' ' && line[i] != '\t' && line_state == WHITESPACE) {
				line_state = DEFINITION;
			} else if (line[i] == '\n') {
				cards[card_count].side1[di] = '\0';
				break;
			}

			if (line_state == TERM) {
				cards[card_count].side2[ti++] = line[i];
			} else if (line_state == DEFINITION) {
				cards[card_count].side1[di++] = line[i];
			}


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
	memset(cards, 0, sizeof(struct card) * cardnum);
	int r;

	for (int i = 0, cardsleft = cardnum; cardsleft > 0; i++, cardsleft--) {
		r = rand() % cardsleft;
		
		memcpy(cards[i].side1, tmpcards[r].side1, CARD_STR_MAX);
		memcpy(cards[i].side2, tmpcards[r].side2, CARD_STR_MAX);

		memcpy(tmpcards[r].side1, tmpcards[cardsleft - 1].side1, CARD_STR_MAX);
		memcpy(tmpcards[r].side2, tmpcards[cardsleft - 1].side2, CARD_STR_MAX);

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
