typedef int flag_t;

struct nncards {
	enum { RUN, NORUN, ERROR } run_state;
	char** cardfile;
	int filenum;
	enum { TERM, DEFINITION } first_side;
	flag_t random;
};

struct nncards nnc_init(int argc, char** argv);
int            nnc_main_loop(struct nncards nncards);
