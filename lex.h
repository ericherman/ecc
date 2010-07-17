/* lex.h */
#ifndef _LEX_H_
#define _LEX_H_

unsigned int lex_get_name_length(const char *input, unsigned int input_size);

int lex_get_number(const char *input, unsigned int input_size,
		unsigned int *len);

char lex_look_ahead(const char *input, unsigned int input_len,
		unsigned int *pos);

#endif /* _LEX_H_ */

