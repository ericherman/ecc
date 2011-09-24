/* lex.h */
#ifndef _LEX_H_
#define _LEX_H_

int lex_get_number(const char *input, unsigned int input_size,
		   unsigned int *len);

void lex_look_ahead(const char *input, unsigned int input_size,
		    unsigned int *pos, char *output, unsigned int output_size);

void lex_look_ahead_n(const char *input, unsigned int input_size,
		      unsigned int *pos, unsigned int number_ahead,
		      char *output, unsigned int output_size);

#endif /* _LEX_H_ */
