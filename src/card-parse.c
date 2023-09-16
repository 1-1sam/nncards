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

struct card*
cp_get_cards(char* filename, int cardnum) {

	struct card* cards;
	FILE* cardfile = fopen(filename, "r");
	int cc = 0;
	char line[MY_BUFSIZE];
	char* s1;
	char* s2;

	cards = malloc(sizeof(struct card) * cardnum);

	while (fgets(line, MY_BUFSIZE, cardfile) != NULL) {

		if (line[0] == '#' || line[0] == '\n')
			continue;

		/* The newline messes up the TUI rendering */
		*(strchr(line, '\n')) = '\0';

		/* Tabs mess up the TUI rendering, replace them with spaces */
		for (int i = 0; line[i]; i++)
			if (line[i] == '\t') line[i] = ' ';

		/* Getting side1 (the definition) */
		if ((s1 = strchr(line, ':')) == NULL)
			continue;

		while (isblank(*(++s1)))
			;

		cards[cc].side1 = strdup(s1);

		/* Getting side2 (the term) */
		s2 = line;

		*(strchr(s2, ':')) = '\0';

		cards[cc++].side2 = strdup(s2);

	}

	fclose(cardfile);
	return cards;

}

void
cp_card_shuffle(struct card* cards, int cardnum) {

	struct card* tmpcards;
	int r;

	srand(time(NULL));

	tmpcards = malloc(sizeof(struct card) * cardnum);
	memcpy(tmpcards, cards, sizeof(struct card) * cardnum);

	for (int i = 0, cardsleft = cardnum; cardsleft > 0; i++, cardsleft--) {

		r = rand() % cardsleft;

		cards[i] = tmpcards[r];
		tmpcards[r] = tmpcards[cardsleft - 1];
	}

	free(tmpcards);
}

void
cp_side_swap(struct card* cards, int cardnum) {

	char* tmpptr;

	for (int i = 0; i < cardnum; i++) {
		tmpptr = cards[i].side1;
		cards[i].side1 = cards[i].side2;
		cards[i].side2 = tmpptr;
	}
}
