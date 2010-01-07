/* c_lib.h */
#ifndef _C_LIB_H_
#define _C_LIB_H_

/*
 * This is stuff needed from libc.
 * since a compiler is needed to build libc,
 * we may need to implement these ourselves for the esssential calls
 * or implement them as no-ops for the non-essential calls.
 *
 * This serves as a place to centralize thinking about these.
 */

void *heap_malloc(unsigned int size);
void heap_free(void *ptr);

void err_msg(const char *msg);

void read_line(const char *source_file,
		char *line_buf, unsigned int buf_size,
		unsigned int *chars_read);

void write_file(const char *executable,
		unsigned char *byte_buffer, unsigned int buf_size);

unsigned int str_len(const char *s);

char *str_ncpy(char *dest, const char *src, unsigned int n);

#endif /* _C_LIB_H_ */

