/*
 * TESTING POTENTIAL W3M-IMG SUPPORT. THIS FILE IS NEVER USED IN NNCARDS.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

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

	char* w3mimg = _get_w3mimg_lib();
	struct image rtrn_img;

	if (w3mimg == NULL) {
		rtrn_img.status = -1;
		return rtrn_img;
	}

	char* w3m_get_img_size;
	FILE* w3mpipe;
	char w3mbuf[MY_BUFSIZE];
	enum dim { WDTH, HGHT };
		
	w3m_get_img_size = malloc(strlen(filename) +
							  strlen(w3mimg) +
							  strlen("echo -e '5;' | ") +
							  1);

	sprintf(w3m_get_img_size, "echo -e '5;%s' | %s", filename, w3mimg);

	/* Gouge out the width and height from w3mimgdisplay's output */
	w3mpipe = popen(w3m_get_img_size, "r");
	fgets(w3mbuf, MY_BUFSIZE, w3mpipe);
	pclose(w3mpipe);

	

	free(w3mimg);
	free(w3m_get_img_size);
	return rtrn_img;

}

int
main(int argc, char** argv) {

	_get_image_info(argv[1]);
	/*
	tb_init();

	int w = tb_width();
	int h = tb_height();
	*/

	return 0;

}
