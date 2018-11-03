/* names_stack.c
   Copyright (C) 2012, 2016, 2018 Eric Herman <eric@freesa.org>

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
#include "names_stack.h"
#include "c_lib.h"
#include "misc.h"

names_stack *stack_new(void)
{
	unsigned int size;
	names_stack *stack;

	size = sizeof(names_stack);
	stack = (names_stack *)heap_malloc(size);
	if (!stack) {
		err_msg("malloc names_stack\n");
		die();
	}
	stack->names_lists = 0;
	stack->names_lists_c = 0;

	return stack;
}

void stack_enter_or_leave(names_stack *stack, unsigned int enter)
{
	names_list **names_lists;
	names_list **old_names_lists;
	names_list *new_names_list;
	names_list *old_names_list;
	unsigned int old_names_lists_c;
	unsigned int new_names_lists_c;
	unsigned int size;
	unsigned int i;

	old_names_lists = stack->names_lists;
	old_names_lists_c = stack->names_lists_c;
	new_names_lists_c = old_names_lists_c;
	if (enter) {
		new_names_lists_c += 1;
	} else {
		if (old_names_lists_c) {
			new_names_lists_c -= 1;
		} else {
			new_names_lists_c = 0;
		}
	}

	size = sizeof(names_list *) * new_names_lists_c;
	if (size > 0) {
		names_lists = (names_list **) heap_malloc(size);
		if (!names_lists) {
			err_msg("malloc names_list array\n");
			die();
		}
	}

	for (i = 0; i < old_names_lists_c && i < new_names_lists_c; i++) {
		names_lists[i] = old_names_lists[i];
	}

	if (enter == 0 && old_names_lists_c != 0) {
		old_names_list = old_names_lists[old_names_lists_c - 1];
		for (i = 0; i < old_names_list->names_c; i++) {
			heap_free((void *)old_names_list->names[i]);
		}
		heap_free((void *)old_names_list->names);
		heap_free((void *)old_names_list);
	}
	if (old_names_lists) {
		heap_free(old_names_lists);
	}

	stack->names_lists = names_lists;
	stack->names_lists_c = new_names_lists_c;

	if (enter) {
		size = sizeof(names_list);
		new_names_list = (names_list *) heap_malloc(size);
		if (!new_names_list) {
			err_msg("malloc names_list\n");
			die();
		}
		new_names_list->names = 0;
		new_names_list->names_c = 0;

		names_lists[new_names_lists_c - 1] = new_names_list;
	}
}

void stack_name_add(names_stack *stack, const char *name)
{
	names_list *names_lists;
	const char **old_names;
	const char **names;
	char *new_buffer;
	unsigned int size;
	unsigned int i;

	names_lists = stack->names_lists[stack->names_lists_c - 1];
	old_names = names_lists->names;

	size = sizeof(const char *) * (names_lists->names_c + 1);
	names = (const char **)heap_malloc(size);
	if (!names) {
		err_msg("malloc char**\n");
		die();
	}

	for (i = 0; i < names_lists->names_c; i++) {
		names[i] = old_names[i];
	}
	if (old_names) {
		heap_free(old_names);
	}

	size = sizeof(char) * str_nlen(name, 100) + 1;
	new_buffer = (char *)heap_malloc(size);
	if (!new_buffer) {
		err_msg("malloc char*\n");
		die();
	}

	names[names_lists->names_c] = new_buffer;
	str_ncpy(new_buffer, name, size);

	names_lists->names = names;
	names_lists->names_c++;
}

unsigned int stack_name_pos(names_stack *stack, const char *name)
{
	unsigned int i;
	names_list *list;

	if (stack->names_lists_c == 0) {
		return 0;
	}
	list = stack->names_lists[stack->names_lists_c - 1];
	if (!list) {
		err_msg("no list?\n");
		return 0;
	}

	for (i = 0; i < list->names_c; i++) {
		if (str_ncmp(list->names[i], 100, name, 100) == 0) {
			return i + 1;
		}
	}
	return 0;
}

unsigned int stack_frame_size(names_stack *stack)
{
	names_list *list;

	if (stack->names_lists_c == 0) {
		return 0;
	}
	list = stack->names_lists[stack->names_lists_c - 1];
	if (!list) {
		err_msg("no list?\n");
		return 0;
	}

	return list->names_c;
}

void stack_enter(names_stack *stack)
{
	stack_enter_or_leave(stack, 1);
}

void stack_leave(names_stack *stack)
{
	stack_enter_or_leave(stack, 0);
}

void stack_destroy(names_stack *stack)
{
	while (stack->names_lists_c) {
		stack_leave(stack);
	}

	if (stack->names_lists_c > 0) {
		if (stack->names_lists[0]) {
			if (stack->names_lists[0]->names) {
				heap_free(stack->names_lists[0]->names);
			}
		}
		heap_free(stack->names_lists);
	}
	heap_free(stack);
}
