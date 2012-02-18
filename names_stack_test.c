#include "names_stack.h"
#include "test_util.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_TO_STRING 10000

void stack_to_string(names_stack_t * stack, char *buf, unsigned int buf_size)
{
	unsigned int i;
	unsigned int j;
	const char **names;
	unsigned int names_c;

	buf[0] = '\0';

	sprintf(buf, "stack (%p) (%u names_lists)\n", (void *)stack,
		stack->names_lists_c);

	for (i = 0; i < stack->names_lists_c; i++) {
		if (!stack->names_lists) {
			sprintf(buf + strlen(buf), "no stack->names_lists");
			return;
		}
		if (!stack->names_lists[i]) {
			sprintf(buf + strlen(buf), "no stack->names_lists[%u]",
				i);
			return;
		}
		names_c = stack->names_lists[i]->names_c;
		sprintf(buf + strlen(buf), "\tnames_list %u (%u names):\n",
			i + 1, names_c);
		if (names_c && stack->names_lists) {
			names = stack->names_lists[i]->names;
		} else {
			names = 0;
		}
		for (j = 0; j < names_c; j++) {
			sprintf(buf + strlen(buf), "\t\t%u %s\n", j + 1,
				names[j]);
		}
		strncpy(buf + strlen(buf), "\t-----\n", buf_size - strlen(buf));
	}
	strncpy(buf + strlen(buf), "-----", buf_size - strlen(buf));
}

void check_depth(unsigned int check_num, names_stack_t * stack,
		 const char *name, unsigned int depth)
{
	unsigned int actual_depth;
	char buf[MAX_TO_STRING];

	sprintf(buf, "check %u: ", check_num);

	stack_to_string(stack, buf + strlen(buf), MAX_TO_STRING - strlen(buf));
	actual_depth = stack_name_pos(stack, name);
	check_unsigned_ints(actual_depth, depth, buf);
}

void test_two_level_stack(void)
{
	names_stack_t *stack;
	unsigned int check_num;

	check_num = 0;
	stack = stack_new();

	stack_enter(stack);

	check_depth(check_num++, stack, "foo", 0);
	check_depth(check_num++, stack, "bar", 0);
	check_depth(check_num++, stack, "baz", 0);
	check_unsigned_int(stack_frame_size(stack), 0);

	stack_name_add(stack, "foo");
	stack_name_add(stack, "bar");

	check_depth(check_num++, stack, "foo", 1);
	check_depth(check_num++, stack, "bar", 2);
	check_depth(check_num++, stack, "baz", 0);
	check_unsigned_ints(stack_frame_size(stack), 2, "1");

	stack_enter(stack);
	check_unsigned_ints(stack_frame_size(stack), 0, "2");
	stack_name_add(stack, "baz");
	check_unsigned_ints(stack_frame_size(stack), 1, "3");
	stack_name_add(stack, "foo");
	check_unsigned_ints(stack_frame_size(stack), 2, "4");

	check_depth(check_num++, stack, "foo", 2);
	check_depth(check_num++, stack, "bar", 0);
	check_depth(check_num++, stack, "baz", 1);

	stack_leave(stack);
	check_unsigned_ints(stack_frame_size(stack), 2, "5");

	check_depth(check_num++, stack, "foo", 1);
	check_depth(check_num++, stack, "bar", 2);
	check_depth(check_num++, stack, "baz", 0);

	stack_leave(stack);
	check_unsigned_ints(stack_frame_size(stack), 0, "6");

	check_depth(check_num++, stack, "foo", 0);
	check_depth(check_num++, stack, "bar", 0);
	check_depth(check_num++, stack, "baz", 0);

	stack_destroy(stack);
}

void test_create_destroy_works(void)
{
	names_stack_t *stack;

	stack = stack_new();
	stack_destroy(stack);

	stack = stack_new();
	stack_enter(stack);
	stack_name_add(stack, "baz");
	stack_name_add(stack, "foo");
	stack_destroy(stack);

	stack = stack_new();
	stack_enter(stack);
	stack_enter(stack);
	stack_enter(stack);
	stack_name_add(stack, "baz");
	stack_name_add(stack, "foo");
	stack_enter(stack);
	stack_name_add(stack, "foo");
	stack_name_add(stack, "baz");
	stack_enter(stack);
	stack_destroy(stack);
}

int main(int argc, char *argv[])
{
	if (argc > 1) {
		printf("%s takes no arguments\n", argv[0]);
		return 1;
	}

	test_two_level_stack();
	test_create_destroy_works();

	return 0;
}
