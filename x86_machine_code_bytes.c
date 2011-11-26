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

unsigned char *get_addl_ops()
{
	return addl_ops;
}

unsigned int get_addl_ops_len()
{
	return addl_ops_len;
}

unsigned char *get_subl_ops()
{
	return subl_ops;
}

unsigned int get_subl_ops_len()
{
	return subl_ops_len;
}

unsigned char *get_imull_ops()
{
	return imull_ops;
}

unsigned int get_imull_ops_len()
{
	return imull_ops_len;
}

unsigned char *get_idiv_ops()
{
	return idiv_ops;
}

unsigned int get_idiv_ops_len()
{
	return idiv_ops_len;
}

unsigned char *get_return_ops()
{
	return return_ops;
}

unsigned int get_return_ops_len()
{
	return return_ops_len;
}

unsigned char get_pushl_op()
{
	return pushl_op;
}
