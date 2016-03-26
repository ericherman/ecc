/* ecc_std_context.h */
#ifndef ECC_STD_CONTEXT_TYPE_H
#define ECC_STD_CONTEXT_TYPE_H

#define FILE_MAX 1024
#define TOKEN_MAX 100
#define BBUF_MAX 0xFFFF

#include "names_stack.h"

typedef struct ecc_std_context_s {
	/* need to investigate memory mapped files */
	const char *source_file;
	char buf[FILE_MAX];
	unsigned int buf_size;
	unsigned int buf_pos;

	char next_token[TOKEN_MAX];

	names_stack *names_stack;

	const char *out_file;
	unsigned char byte_buf[BBUF_MAX];
	unsigned int bytes_written;
} ecc_std_context;

#endif /* ECC_STD_CONTEXT_TYPE_H */
