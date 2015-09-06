/* c_lib.h */
#ifndef C_LIB_H
#define C_LIB_H

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
void die();

void read_file(const char *in_file,
	       char *file_buf, unsigned int buf_size, unsigned int *chars_read);

void write_file(const char *out_file,
		unsigned char *byte_buffer, unsigned int buf_size);

#endif /* C_LIB_H */
