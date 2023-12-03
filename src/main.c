#include "run.h"

int
main(int argc, char** argv) {

	struct nncards nncards = nnc_init(argc, argv);

	if (!nncards.run)
		return 0;

	nnc_main_loop(nncards);

	return 0;

}
