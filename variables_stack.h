#ifndef _VARIABLES_STACK_H_
#define _VARIABLES_STACK_H_

typedef struct {
	const char **names;
	unsigned int names_c;
} names_list_t;

typedef struct {
	names_list_t **names_lists;
	unsigned int names_lists_c;
} names_stack_t;

void stack_to_string(names_stack_t * stack, char *buf, unsigned int buf_size);
names_stack_t *stack_new(void);
void stack_enter(names_stack_t * stack);
void stack_name_add(names_stack_t * stack, const char *name);
unsigned int stack_name_pos(names_stack_t * stack, const char *name);
void stack_leave(names_stack_t * stack);
void stack_destroy(names_stack_t * stack);

#endif /* _VARIABLES_STACK_H_ */
