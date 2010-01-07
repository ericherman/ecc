/* data.h */
#ifndef _CONTEXT_H_
#define _CONTEXT_H_

typedef struct context_t_ {

	void *data;

	char (*lex_look_ahead)(void *data);
	void (*lex_advance)(void *data, unsigned int chars);
	int (*lex_get_number)(void *data);

	void (*output_term)(void *data, int number);
	void (*output_add)(void *data);
	void (*output_subtract)(void *data);
	void (*output_multiply)(void *data);
	void (*output_divide)(void *data);
	void (*output_statements_complete)(void *data);

	void (*read_line)(void *data);
	void (*write_file)(void *data);

	void (*output_header)(void *data);
	void (*output_os_return)(void *data);

	void (*to_string)(void *data, char *buf, unsigned int buf_size);

} context_t;

#endif /* _CONTEXT_H_ */
