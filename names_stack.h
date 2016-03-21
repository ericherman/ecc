#ifndef NAMES_STACK_H
#define NAMES_STACK_H

typedef struct {
	const char **names;
	unsigned int names_c;
} names_list_t;

typedef struct {
	names_list_t **names_lists;
	unsigned int names_lists_c;
} names_stack_t;

names_stack_t *stack_new(void);
void stack_enter(names_stack_t *stack);
void stack_name_add(names_stack_t *stack, const char *name);
unsigned int stack_name_pos(names_stack_t *stack, const char *name);
unsigned int stack_frame_size(names_stack_t *stack);
void stack_leave(names_stack_t *stack);
void stack_destroy(names_stack_t *stack);

#endif /* NAMES_STACK_H */
