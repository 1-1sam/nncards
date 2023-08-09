
#include "run.h"

int
main(int argc, char** argv) {

	struct nncards nncards = nnc_init(argc, argv);

	/* Die if norun flag is set (help/version message, no file passed). */
	if (nncards.run == NORUN)
		return 0;

	nnc_main_loop(nncards);

	return 0;

}
