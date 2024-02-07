#include <stdio.h>
#include <string.h>

#define TB_IMPL
#include "termbox2.h"

#include "tui.h"

/* Maximum number of characters I've arbitrarily decided for the info bar. */
#define BARMAX 255

#define BOX_UR_CORNER 0x250C
#define BOX_BR_CORNER 0x2514
#define BOX_UL_CORNER 0x2510
#define BOX_BL_CORNER 0x2518
#define BOX_HORZ_LINE 0x2500
#define BOX_VERT_LINE 0x2502

static int h, w;

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
tui_init(void) {

	tb_init();

}

void
tui_clear(void) {

	tb_clear();

}

void
tui_update_size(void) {

	h = tb_height();
	w = tb_width();

}

enum nncards_commands
tui_poll_event(void) {

	struct tb_event ev;

	tb_poll_event(&ev);

	switch (ev.ch) {
		case 'c': return NEXT;
		case 'l': return NEXT;
		case 'z': return PREV;
		case 'h': return PREV;
		case 'x': return FLIP;
		case ' ': return FLIP;
		case 'd': return LAST;
		case 'a': return FIRST;
		case 'q': return QUIT;
		default: break;
	}

	switch (ev.key) {
		case TB_KEY_ARROW_RIGHT: return NEXT;
		case TB_KEY_ARROW_LEFT:  return PREV;
		case TB_KEY_ARROW_UP:    return FLIP;
		case TB_KEY_ARROW_DOWN:  return FLIP;
		case TB_KEY_PGUP:        return LAST;
		case TB_KEY_PGDN:        return FIRST;
		case TB_KEY_ESC:         return QUIT;
		default: break;
	}

	return -1;

}

void
tui_shutdown(void) {

	tb_shutdown();

}

void
tui_draw_card(char* str) {

	/* Drawing corners. */
	tb_set_cell(1, 2, BOX_UR_CORNER, 0, 0);
	tb_set_cell(1, h - 3, BOX_BR_CORNER, 0, 0);
	tb_set_cell(w - 2, 2, BOX_UL_CORNER, 0, 0);
	tb_set_cell(w - 2, h - 3, BOX_BL_CORNER, 0, 0);

	/* Drawing top and bottom line. */
	for (int i = 2; i < w - 2; i++) {
		tb_set_cell(i, 2, BOX_HORZ_LINE, 0, 0);
		tb_set_cell(i, h - 3, BOX_HORZ_LINE, 0, 0);
	}

	/* Drawing right and left line. */
	for (int i = 3; i < h - 3; i++) {
		tb_set_cell(w - 2, i, BOX_VERT_LINE, 0, 0);
		tb_set_cell(1, i, BOX_VERT_LINE, 0, 0);
	}

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
