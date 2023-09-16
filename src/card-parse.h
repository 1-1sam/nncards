#include <stdio.h>
#include <string.h>

struct card {
	char* side1;
	char* side2;
};

int          cp_get_cardnum(char* filename);
struct card* cp_get_cards(char* filename, int cardnum);
void         cp_card_shuffle(struct card* cards, int cardnum);
void         cp_side_swap(struct card* cards, int cardnum);
