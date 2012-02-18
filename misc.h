/* misc.h */
#ifndef _MISC_H_
#define _MISC_H_

int is_whitespace(char c);
int is_number(char c);
int is_alpha(char c);

void str_cpy_offset(const char *src, char *buf, unsigned int buf_size,
		    unsigned int *buf_pos);

char *str_ncpy(char *dest, const char *src, unsigned int limit);

unsigned int str_nlen(const char *s, unsigned int limit);

int str_ncmp(const char *left, unsigned int left_size,
	     const char *right, unsigned int right_size);

int a_to_i(const char *str, unsigned int max_len);

void write_int(unsigned char *buf, int value);
void write_bytes(const char *name,
		 unsigned char *buf, unsigned int buf_size,
		 unsigned int *bytes_written,
		 const unsigned char *to_write, unsigned int count);

#endif /* _MISC_H_ */
