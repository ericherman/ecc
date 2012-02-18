/* data.h */
#ifndef _CONTEXT_H_
#define _CONTEXT_H_

typedef struct context_t_ {

	void *data;

	const char *(*lex_look_ahead) (struct context_t_ * ctx);
	void (*lex_advance) (struct context_t_ * ctx, unsigned int chars);
	int (*lex_get_number) (struct context_t_ * ctx);
	const char *(*lex_get_name) (struct context_t_ * ctx);

	void (*stack_enter) (struct context_t_ * ctx);
	void (*stack_assign_name) (struct context_t_ * ctx, const char *name);
	unsigned int (*stack_name_pos) (struct context_t_ * ctx, const char *name);
	unsigned int (*stack_frame_size) (struct context_t_ * ctx);
	void (*stack_leave) (struct context_t_ * ctx);

	void (*output_term) (struct context_t_ * ctx, int number);
	void (*output_add) (struct context_t_ * ctx);
	void (*output_subtract) (struct context_t_ * ctx);
	void (*output_multiply) (struct context_t_ * ctx);
	void (*output_divide) (struct context_t_ * ctx);
	void (*output_stack_enter) (struct context_t_ * ctx);
	void (*output_stack_allocate) (struct context_t_ * ctx, unsigned int bytes);
	void (*output_stack_assign_int) (struct context_t_ * ctx, unsigned int depth, int number);
	void (*output_stack_leave) (struct context_t_ * ctx);
	void (*output_statements_complete) (struct context_t_ * ctx);

	void (*read_file) (struct context_t_ * ctx);
	void (*write_file) (struct context_t_ * ctx);

	void (*output_header) (struct context_t_ * ctx);
	void (*output_os_return) (struct context_t_ * ctx);

	void (*to_string) (struct context_t_ * ctx, char *buf,
			   unsigned int buf_size);

} context_t;

#endif /* _CONTEXT_H_ */
