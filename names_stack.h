/* names_stack.h
   Copyright (C) 2012, 2015, 2016, 2018 Eric Herman <eric@freesa.org>

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
#ifndef NAMES_STACK_H
#define NAMES_STACK_H

typedef struct names_list_s {
	const char **names;
	unsigned int names_c;
} names_list;

typedef struct names_stack_s {
	names_list **names_lists;
	unsigned int names_lists_c;
} names_stack;

names_stack *stack_new(void);
void stack_enter(names_stack *stack);
void stack_name_add(names_stack *stack, const char *name);
unsigned int stack_name_pos(names_stack *stack, const char *name);
unsigned int stack_frame_size(names_stack *stack);
void stack_leave(names_stack *stack);
void stack_destroy(names_stack *stack);

#endif /* NAMES_STACK_H */
