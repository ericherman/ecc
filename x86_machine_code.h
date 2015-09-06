/* x86_machine_code.h */
#ifndef X86_MACHINE_CODE_H
#define X86_MACHINE_CODE_H

void output_term(int number,
		 unsigned char *buf, unsigned int buf_size,
		 unsigned int *bytes_written);

void output_statements_complete(unsigned char *buf, unsigned int buf_size,
				unsigned int *bytes_written);

void output_add(unsigned char *buf, unsigned int buf_size,
		unsigned int *bytes_written);

void output_subtract(unsigned char *buf, unsigned int buf_size,
		     unsigned int *bytes_written);

void output_multiply(unsigned char *buf, unsigned int buf_size,
		     unsigned int *bytes_written);

void output_divide(unsigned char *buf, unsigned int buf_size,
		   unsigned int *bytes_written);

void output_stack_enter(unsigned char *buf, unsigned int buf_size,
			unsigned int *bytes_written);

void output_stack_allocate(unsigned int bytes,
			   unsigned char *buf, unsigned int buf_size,
			   unsigned int *bytes_written);

void output_stack_assign_int(unsigned int depth, int number,
			     unsigned char *buf, unsigned int buf_size,
			     unsigned int *bytes_written);

void output_stack_leave(unsigned char *buf, unsigned int buf_size,
			unsigned int *bytes_written);

#endif /* X86_MACHINE_CODE_H */
