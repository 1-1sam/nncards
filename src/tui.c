#include <stdio.h>
#include <string.h>

#define TB_IMPL
#include "termbox.h"

static void
_card_print(char* str) {

	int w = tb_width();
	int h = tb_height();
	int linelen = w - 6;
	int yoffset = -(strlen(str) / linelen) / 2;
	int str_i = 0;

	char line[linelen + 1];

	do {
		memset(line, 0, linelen);
		
		for (int i = 0; i < linelen && str[str_i]; i++, str_i++)
			line[i] = str[str_i];
		line[linelen] = '\0';

		tb_printf((w / 2) - (strlen(line) / 2), (h / 2) + yoffset++, 0, 0, "%s", line);

	} while (line[linelen - 1]);

}

void
tui_draw_card(char* str) {

	int w = tb_width();
	int h = tb_height();

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

	_card_print(str);	

	tb_present();

}

void
tui_draw_info(char* filename, int currcard, int cardnum) {

	int h = tb_height();

	tb_printf(0, h - 1, 0, 0, "[%d/%d] %s", currcard, cardnum, filename);

	tb_present();

}
