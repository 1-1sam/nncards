#include <stdio.h>
#include <unistd.h>

#include "tui.h"
#include "card-parse.h"

struct nncards {
	enum run { RUN, NORUN } run;
	char* cardfile;
	enum opt_first_side { TERM, DEFINITION } opt_first_side;
	int opt_random;
	int opt_initcard;
};

struct nncards nnc_init(int argc, char** argv);
int            nnc_main_loop(struct nncards nncards);
