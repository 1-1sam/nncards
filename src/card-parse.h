struct card {
	char* side1;
	char* side2;
};

struct deck {
	struct card* cards;
	int cardnum;
};

int  cp_get_cards(struct deck* deck, char** files, int filenum);
int  cp_card_shuffle(struct deck deck);
void cp_side_swap(struct deck deck);
void cp_free_cards(struct deck deck);
