#include <stdio.h>
#include <string.h>

#define CARD_STR_MAX 255

struct card {
	char side1[CARD_STR_MAX];
	char side2[CARD_STR_MAX];
};

int  cp_get_cardnum(char* filename);
int  cp_get_cards(struct card* cards, char* filename);
int  cp_open_test(char* filename);
void cp_card_shuffle(struct card* cards, int cardnum);
void cp_side_swap(struct card* cards, int cardnum);
