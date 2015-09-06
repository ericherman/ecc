/* x86_machine_code_bytes.h */
#ifndef X86_MACHINE_CODE_BYTES_H
#define X86_MACHINE_CODE_BYTES_H

unsigned char *get_addl_ops();
unsigned int get_addl_ops_len();

unsigned char *get_subl_ops();
unsigned int get_subl_ops_len();

unsigned char *get_imull_ops();
unsigned int get_imull_ops_len();

unsigned char *get_idiv_ops();
unsigned int get_idiv_ops_len();

unsigned char *get_return_ops();
unsigned int get_return_ops_len();

unsigned char get_pushl_op();

unsigned char *get_stack_enter_ops();
unsigned int get_stack_enter_ops_len();

unsigned char *get_stack_leave_ops();
unsigned int get_stack_leave_ops_len();

unsigned char get_movl_ebp_op();

#endif /* X86_MACHINE_CODE_BYTES_H */
