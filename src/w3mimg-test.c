/*
 * TESTING POTENTIAL W3M-IMG SUPPORT. THIS FILE IS NEVER USED IN NNCARDS.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

#include "tui.h"

#define MY_BUFSIZE 100

struct image {
	char* filename;
	int w;
	int h;
	int status;
};

struct termsize {
	int pixw;
	int pixh;
	int cols;
	int lines;
};

static char*
_get_w3mimgbin(void) {

	char* rtrn_str;
	char* w3mimgbins[] = {
		"/usr/lib/w3m/w3mimgdisplay",
		"/usr/libexec/w3m/w3mimgdisplay",
		"/usr/lib64/w3m/w3mimgdisplay",
		"/usr/libexec64/w3m/w3mimgdisplay",
		"/usr/local/libexec/w3m/w3mimgdisplay"
	};

	for (char** p = w3mimgbins; *p; p++) {
		if (fopen(*p, "r") != NULL) {
			rtrn_str = malloc(strlen(*p) + 1);
			strcpy(rtrn_str, *p);
			return rtrn_str;
		}
	}

	fprintf(stderr, "Could not find w3mimgdisplay\n");
	return NULL;
}

static struct image
_get_image_info(char* filename) {

	char* w3mingbin = _get_w3mimgbin();
	struct image rtrn_img;

	if (w3mingbin == NULL) {
		rtrn_img.status = -1;
		return rtrn_img;
	}

	char* w3m_get_img_size;
	FILE* w3mpipe;
	char w3mbuf[MY_BUFSIZE];
	char dimbuf[MY_BUFSIZE / 2];
	char* wb = w3mbuf;
	char* db;

	rtrn_img.filename = filename;

	w3m_get_img_size = malloc(strlen(filename) +
							  strlen(w3mingbin) +
							  strlen("echo -e '5;' | ") +
							  1);

	sprintf(w3m_get_img_size, "echo -e '5;%s' | %s", filename, w3mingbin);

	/* Gouge out the width and height from w3mimgdisplay's output */
	w3mpipe = popen(w3m_get_img_size, "r");
	fgets(w3mbuf, MY_BUFSIZE, w3mpipe);
	pclose(w3mpipe);

	if (*wb == '\n') {
		fprintf(stderr, "w3m could not read %s\n", filename);
		rtrn_img.status = -1;
		return rtrn_img;
	}

	for (db = dimbuf; isdigit(*wb); wb++, db++)
		*db = *wb;
	*db = '\0';
	rtrn_img.w = atoi(dimbuf);
	wb++;

	memset(dimbuf, 0, MY_BUFSIZE / 2);
	for (db = dimbuf; isdigit(*wb); wb++, db++)
		*db = *wb;
	*db = '\0';
	rtrn_img.h = atoi(dimbuf);

	rtrn_img.status = 0;
	
	free(w3mingbin);
	free(w3m_get_img_size);
	return rtrn_img;

}

static struct termsize
_get_termsize(void) {

	/* Use w3mimgdisplay -test to get terminal width and height */

	struct termsize rtrn_term;
	char* w3mimgbin = _get_w3mimgbin();
	char* w3mimg_test;
	FILE* w3mpipe;
	char w3mbuf[MY_BUFSIZE];
	char dimbuf[MY_BUFSIZE / 2];
	char* wb = w3mbuf;
	char* db;

	w3mimg_test = malloc(strlen(w3mimgbin) + strlen(" -test") + 1);
	sprintf(w3mimg_test, "%s -test", w3mimgbin);

	w3mpipe = popen(w3mimg_test, "r");
	fgets(w3mbuf, MY_BUFSIZE, w3mpipe);
	pclose(w3mpipe);

	for (db = dimbuf; isdigit(*wb); wb++, db++)
		*db = *wb;
	*db = '\0';
	rtrn_term.pixw = atoi(dimbuf);
	wb++;

	memset(dimbuf, 0, MY_BUFSIZE / 2);
	for (db = dimbuf; isdigit(*wb); wb++, db++)
		*db = *wb;
	*db = '\0';
	rtrn_term.pixh = atoi(dimbuf);	

	rtrn_term.cols = tb_width();
	rtrn_term.lines = tb_height();

	free(w3mimgbin);
	free(w3mimg_test);
	return rtrn_term;

}

static double
_get_w3m_draw_mult(struct image img) {

	struct termsize ts = _get_termsize();
	double rtrn;

	if (img.h >= img.w)
		rtrn = (double) (ts.pixh - (8 * (ts.pixh / ts.lines))) / img.h;
	else
		rtrn = (double) (ts.pixw - (6 * (ts.pixw / ts.cols))) / img.w;

	if (rtrn <= 0)
		return -1;

	return rtrn;

}

int
draw_w3mimg(char* filename) {

	struct image img = _get_image_info(filename);
	if (img.status == -1)
		return -1;
		
	double mult = _get_w3m_draw_mult(img);
	if (mult == -1) {
		fprintf(stderr, "Cannot draw image; terminal too small\n");
		return -1;
	}

	struct termsize ts = _get_termsize();
	char* w3mimgbin = _get_w3mimgbin();
	char* w3mimg_comm;
	char intbuf[MY_BUFSIZE];
	int spixw = img.w * mult;
	int spixh = img.h * mult;
	int xoff = (ts.pixw / 2) - (spixw / 2);
	int yoff = (ts.pixh / 2) - (spixh / 2);

	sprintf(intbuf, "%d%d%d%d", spixw, spixh, xoff, yoff);

	w3mimg_comm = malloc(strlen("echo -e '0;1;;;;;;;;;;\\n4;\\n3;' | ") +
						 strlen(intbuf) +
						 strlen(filename) +
						 strlen(w3mimgbin) +
						 1);

	sprintf(w3mimg_comm, "echo -e '0;1;%d;%d;%d;%d;;;;;%s\\n4;\\n3;' | %s",
			xoff, yoff, spixw, spixh, filename, w3mimgbin);

	system(w3mimg_comm);

	free(w3mimg_comm);
	free(w3mimgbin);
	return 0;
}

int
main(int argc, char** argv) {

	tb_init();

	struct tb_event ev;

	tui_draw_card("");
	draw_w3mimg(argv[1]);

	tb_poll_event(&ev);

	tb_shutdown();

	/*
	tb_init();

	int w = tb_width();
	int h = tb_height();
	*/

	return 0;

}
