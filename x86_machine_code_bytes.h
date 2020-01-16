/* x86_machine_code_bytes.h
   Copyright (C) 2011, 2012, 2015, 2018 Eric Herman <eric@freesa.org>

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

	https://www.gnu.org/licenses/gpl-3.0.txt
 */
/* x86_machine_code_bytes.h */
#ifndef X86_MACHINE_CODE_BYTES_H
#define X86_MACHINE_CODE_BYTES_H

unsigned char *get_addl_ops(void);
unsigned int get_addl_ops_len(void);

unsigned char *get_subl_ops(void);
unsigned int get_subl_ops_len(void);

unsigned char *get_imull_ops(void);
unsigned int get_imull_ops_len(void);

unsigned char *get_idiv_ops(void);
unsigned int get_idiv_ops_len(void);

unsigned char *get_return_ops(void);
unsigned int get_return_ops_len(void);

unsigned char get_pushl_op(void);

unsigned char *get_stack_enter_ops(void);
unsigned int get_stack_enter_ops_len(void);

unsigned char *get_stack_leave_ops(void);
unsigned int get_stack_leave_ops_len(void);

unsigned char get_movl_ebp_op(void);

#endif /* X86_MACHINE_CODE_BYTES_H */
