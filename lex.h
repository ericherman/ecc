/* lex.h */
#ifndef LEX_H
#define LEX_H

void lex_look_ahead(const char *input, unsigned int input_size,
		    unsigned int *pos, char *output, unsigned int output_size);

void lex_look_ahead_n(const char *input, unsigned int input_size,
		      unsigned int *pos, unsigned int number_ahead,
		      char *output, unsigned int output_size);

#endif /* LEX_H */
