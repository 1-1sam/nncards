#include <stdio.h>
#include <string.h>

#define TB_IMPL
#include "termbox2.h"

/* Maximum number of characters I've arbitrarily decided for the info bar. */
#define BARMAX 255

int h, w;

static void
_text_print(char* str) {

	int linelen = w - 6;
	int yoffset = -(strlen(str) / linelen) / 2;
	int yoffset_limit = (h - 3) - (h / 2);
	char* p = str;
	char line[linelen + 1];

	if (yoffset < 3 - (h / 2))
		yoffset = 3 - (h / 2);

	do {
		strncpy(line, p, linelen);
		line[linelen] = '\0';

		tb_printf((w / 2) - (strlen(line) / 2), (h / 2) + yoffset++,
				  0, 0, "%s", line);

		p += linelen;

	} while (line[linelen - 1] && yoffset != yoffset_limit);

}

void
tui_update_size(void) {

	h = tb_height();
	w = tb_width();

}

void
tui_draw_card(char* str) {

	/* Drawing corners. */
	tb_set_cell(1, 2, 0x250C, 0, 0);
	tb_set_cell(1, h - 3, 0x2514, 0, 0);
	tb_set_cell(w - 2, 2, 0x2510, 0, 0);
	tb_set_cell(w - 2, h - 3, 0x2518, 0, 0);

	/* Drawing top line. */
	for (int i = 2; i < w - 2; i++)
		tb_set_cell(i, 2, 0x2500, 0, 0);

	/* Draw bottom line. */
	for (int i = 2; i < w - 2; i++)
		tb_set_cell(i, h - 3, 0x2500, 0, 0);

	/* Drawing right line. */
	for (int i = 3; i < h - 3; i++)
		tb_set_cell(w - 2, i, 0x2502, 0, 0);

	/* Drawing left line. */
	for (int i = 3; i < h - 3; i++)
		tb_set_cell(1, i, 0x2502, 0, 0);

	_text_print(str);

	tb_present();

}

void
tui_draw_info(int currcard, int cardnum, char** files, int filenum) {

	char text[BARMAX + 1];

	snprintf(text, BARMAX, "[%d/%d] ", currcard + 1, cardnum);

	for (int i = 0; i < filenum; i++) {
		strncat(text, files[i], BARMAX - strlen(text) - 1);
		strcat(text, " ");
	}

	tb_printf(0, h - 1, 0, 0, "%s", text);

	tb_present();

}
