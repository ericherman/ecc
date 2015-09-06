/* std_context_t.h */
#ifndef STD_CONTEXT_T_H
#define STD_CONTEXT_T_H

#define FILE_MAX 1024
#define TOKEN_MAX 100
#define BBUF_MAX 0xFFFF

#include "names_stack.h"

typedef struct std_context_t_ {
	/* need to investigate memory mapped files */
	const char *source_file;
	char buf[FILE_MAX];
	unsigned int buf_size;
	unsigned int buf_pos;

	char next_token[TOKEN_MAX];

	names_stack_t *names_stack;

	const char *out_file;
	unsigned char byte_buf[BBUF_MAX];
	unsigned int bytes_written;
} std_context_t;

#endif /* STD_CONTEXT_T_H */
