/* std_context_t.h */
#ifndef _STD_CONTEXT_T_H_
#define _STD_CONTEXT_T_H_

#define LINE_MAX 1024
#define BBUF_MAX 0xFFFF

typedef struct std_context_t_ {
	/* need to investigate memory mapped files */
	const char * source_file;
	char buf[LINE_MAX];
	unsigned int buf_size;
	unsigned int buf_pos;

	const char * executable;
	unsigned char byte_buf[BBUF_MAX];
	unsigned int bytes_written;
} std_context_t;

#endif /* _STD_CONTEXT_T_H_ */
