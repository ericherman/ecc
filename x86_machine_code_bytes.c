/* x86_machine_code_bytes.c
   Copyright (C) 2011, 2012, 2018 Eric Herman <eric@freesa.org>

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
#include "x86_machine_code_bytes.h"

unsigned char addl_ops[] = {
	0x5a,			/* popl %edx */
	0x58,			/* popl %eax */
	0x01, 0xd0,		/* addl %edx, %eax */
	0x50,			/* pushl %eax */
};

unsigned int addl_ops_len = sizeof(addl_ops);

unsigned char subl_ops[] = {
	0x5a,			/* popl %edx */
	0x58,			/* popl %eax */
	0x29, 0xd0,		/* subl %edx, %eax */
	0x50,			/* pushl %eax */
};

unsigned int subl_ops_len = sizeof(subl_ops);

unsigned char imull_ops[] = {
	0x5a,			/* popl %edx */
	0x58,			/* popl %eax */
	0x0f, 0xaf, 0xc2,	/* imull %edx, %eax */
	0x50,			/* pushl %eax */
};

unsigned int imull_ops_len = sizeof(imull_ops);

unsigned char idiv_ops[] = {
	0xba, 0x00, 0x00, 0x00, 0x00,	/* movl $0, %edx */
	0x5b,			/* popl %ebx */
	0x58,			/* popl %eax */
	0xf7, 0xfb,		/* idiv %ebx */
	0x50,			/* pushl %eax */
};

unsigned int idiv_ops_len = sizeof(idiv_ops);

unsigned char return_ops[] = { 0x5b /* popl %ebx */  };

unsigned int return_ops_len = sizeof(return_ops);

unsigned char pushl_op = 0x68;	/* pushl $immediate_value */

unsigned char stack_enter_ops[] = {
	0x55,			/* pushl %ebp */
	0x89, 0xe5,		/* movl %esp, %ebp */
};

unsigned int stack_enter_ops_len = sizeof(stack_enter_ops);

unsigned char stack_leave_ops[] = {
	0x5d,			/* popl %ebp */
};

unsigned int stack_leave_ops_len = sizeof(stack_leave_ops);

unsigned char movl_ebp = 0xbd;	/* B8+ rd MOV r32,imm32 Move imm32 to r32 */

unsigned char *get_addl_ops(void)
{
	return addl_ops;
}

unsigned int get_addl_ops_len(void)
{
	return addl_ops_len;
}

unsigned char *get_subl_ops(void)
{
	return subl_ops;
}

unsigned int get_subl_ops_len(void)
{
	return subl_ops_len;
}

unsigned char *get_imull_ops(void)
{
	return imull_ops;
}

unsigned int get_imull_ops_len(void)
{
	return imull_ops_len;
}

unsigned char *get_idiv_ops(void)
{
	return idiv_ops;
}

unsigned int get_idiv_ops_len(void)
{
	return idiv_ops_len;
}

unsigned char *get_return_ops(void)
{
	return return_ops;
}

unsigned int get_return_ops_len(void)
{
	return return_ops_len;
}

unsigned char get_pushl_op(void)
{
	return pushl_op;
}

unsigned char *get_stack_enter_ops(void)
{
	return stack_enter_ops;
}

unsigned int get_stack_enter_ops_len(void)
{
	return stack_enter_ops_len;
}

unsigned char *get_stack_leave_ops(void)
{
	return stack_leave_ops;
}

unsigned int get_stack_leave_ops_len(void)
{
	return stack_leave_ops_len;
}

unsigned char get_movl_ebp_op(void)
{
	return movl_ebp;
}
