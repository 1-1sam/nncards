/*
 * TESTING POTENTIAL W3M-IMG SUPPORT. THIS FILE IS NEVER USED IN NNCARDS.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

#define TB_IMPL
#include "termbox.h"

#define MY_BUFSIZE 100

struct image {
	char* filename;
	int w;
	int h;
	int status;
};

static char*
_get_w3mimg_lib() {

	char* rtrn_str;
	char* w3mimg_libs[5] = {
		"/usr/lib/w3m/w3mimgdisplay",
    	"/usr/libexec/w3m/w3mimgdisplay",
    	"/usr/lib64/w3m/w3mimgdisplay",
    	"/usr/libexec64/w3m/w3mimgdisplay",
    	"/usr/local/libexec/w3m/w3mimgdisplay"
	};

	for (int i = 0; i < 5; i++) {
		if (fopen(w3mimg_libs[i], "r") != NULL) {
			rtrn_str = malloc(strlen(w3mimg_libs[i]) + 1);
			strcpy(rtrn_str, w3mimg_libs[i]);
			return rtrn_str;
		}
	}

	fprintf(stderr, "Could not find w3mimgdisplay\n");
	return NULL;
}

static struct image
_get_image_info(char* filename) {

	char* w3mimglib = _get_w3mimg_lib();
	struct image rtrn_img;

	if (w3mimglib == NULL) {
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
							  strlen(w3mimglib) +
							  strlen("echo -e '5;' | ") +
							  1);

	sprintf(w3m_get_img_size, "echo -e '5;%s' | %s", filename, w3mimglib);

	/* Gouge out the width and height from w3mimgdisplay's output */
	w3mpipe = popen(w3m_get_img_size, "r");
	fgets(w3mbuf, MY_BUFSIZE, w3mpipe);
	pclose(w3mpipe);

	if (*wb == '\n') {
		fprintf(stderr, "w3m could not read %s\n", filename);
		rtrn_img.status = -1;
		return rtrn_img;
	}

	/* Get width */
	for (db = dimbuf; isdigit(*wb); wb++, db++)
		*db = *wb;
	*db = '\0';
	rtrn_img.w = atoi(dimbuf);
	wb++;

	/* Get height */
	memset(dimbuf, 0, MY_BUFSIZE / 2);
	for (db = dimbuf; isdigit(*wb); wb++, db++)
		*db = *wb;
	*db = '\0';
	rtrn_img.h = atoi(dimbuf);

	rtrn_img.status = 0;
	
	free(w3mimglib);
	free(w3m_get_img_size);
	return rtrn_img;

}

static double
_get_scale_mult(struct image image) {

	double rtrn_mult;
	int tw = tb_width();
	int th = tb_height();

	if (tw >= th)
		rtrn_mult = (th - 8) / image.h;	
	else if (tw < th)
		rtrn_mult = (tw - 4) / image.w;

	return rtrn_mult;

}

int
draw_w3mimg(char* filename) {

	struct image img = _get_image_info(filename);

	if (img.status == -1)
		return -1;

	char* w3mimglib = _get_w3mimg_lib();
	char* w3mimg_draw;
	double scale_mult = _get_scale_mult(img);

	free(w3mimglib);
	return 0;
}

int
main(int argc, char** argv) {

	struct image img = _get_image_info(argv[1]);

	tb_init();

	tb_shutdown();

	/*
	tb_init();

	int w = tb_width();
	int h = tb_height();
	*/

	return 0;

}
