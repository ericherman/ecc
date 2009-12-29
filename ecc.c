#include "c_lib.h"
#include "ecc_lib.h"

int main(int argc, char *argv[]) {

	if (argc < 3) {
		err_msg("usage: ");
		err_msg(argv[0]);
		err_msg(" sourcefile exectuable\n");
		return 1;
	}

	compile(argv[1], argv[2]);

	return 0;
}
