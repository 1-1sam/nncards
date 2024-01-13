#include "run.h"

/* TODO: Return 1 if something went wrong. */
int
main(int argc, char** argv) {

	struct nncards nncards = nnc_init(argc, argv);

	if (nncards.run_state == ERROR)
		return 1;

	if (nncards.run_state == NORUN)
		return 0;

	return nnc_main_loop(nncards);

}
