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
