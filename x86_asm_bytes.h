/* x86_asm_bytes.h */
#ifndef _X86_ASM_BYTES_H_
#define _X86_ASM_BYTES_H_

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

#endif /* _X86_ASM_BYTES_H_ */
