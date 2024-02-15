enum nncards_commands {
	NEXT,
	PREV,
	FLIP,
	LAST,
	FIRST,
	QUIT,
};

/* Wrappers for respective tb_? functions */
void tui_init(void);
void tui_clear(void);
void tui_update_size(void);
void tui_shutdown(void);

enum nncards_commands tui_poll_event(void);

void tui_draw_card(char* str);
void tui_draw_info(int currcard, int cardnum, char** files, int filenum);
