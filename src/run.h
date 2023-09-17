#include <stdio.h>
#include <unistd.h>

#include "tui.h"
#include "card-parse.h"

typedef int flag_t;

struct nncards {
	flag_t run;
	char* cardfile;
	enum first_side { TERM, DEFINITION } first_side;
	flag_t random;
	int initcard;
	flag_t die_on_error;
};

struct nncards nnc_init(int argc, char** argv);
int            nnc_main_loop(struct nncards nncards);
