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
	int cc = 0;
	char line[MY_BUFSIZE];
	char* l;

	while (fgets(line, MY_BUFSIZE, cardfile) != NULL) {

		l = line;

		if (*l == '#' || *l == '\n')
			continue;

		if ((l = strchr(line, ':')) == NULL)
			continue;

		strncpy(cards[cc].side2, strtok(line, ":"), CARD_STR_MAX - 1);

		cards[cc].side2[CARD_STR_MAX - 1] = '\0';

		while (isblank(*(++l)))
			;

		strncpy(cards[cc].side1, l, CARD_STR_MAX - 1);

		if (strchr(cards[cc].side1, '\n') != NULL)
			*(strchr(cards[cc].side1, '\n')) = '\0';
		else
			cards[cc].side1[CARD_STR_MAX - 1] = '\0';

		cc++;
	}

	fclose(cardfile);

	if (cc == 0)
		return -1;

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
