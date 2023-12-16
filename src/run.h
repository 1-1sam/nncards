typedef int flag_t;

struct nncards {
	flag_t run;
	char* cardfile;
	enum { TERM, DEFINITION } first_side;
	flag_t random;
	int initcard;
};

struct nncards nnc_init(int argc, char** argv);
int            nnc_main_loop(struct nncards nncards);
