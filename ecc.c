#include "c_lib.h"
#include "std_context.h"
#include "compiler.h"

int main(int argc, char *argv[])
{
	context_t *ctx;

	if (argc < 3) {
		err_msg("usage: ");
		err_msg(argv[0]);
		err_msg(" sourcefile executable\n");
		return 1;
	}

	ctx = alloc_std_context(argv[1], argv[2]);

	compile(ctx);

	free_std_context(ctx);

	return 0;
}
