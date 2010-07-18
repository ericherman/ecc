/* misc.h */
#ifndef _MISC_H_
#define _MISC_H_

int is_whitespace(char c);
int is_number(char c);
int is_alpha(char c);

void str_cpy_offset(const char *src, char *buf, unsigned int buf_size,
		unsigned int *buf_pos);

char *str_lcpy(char *dest, const char *src, unsigned int limit);

unsigned int str_len(const char *s);

#endif /* _MISC_H_ */
