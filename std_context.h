/* std_context.h */
#ifndef _STD_CONTEXT_H_
#define _STD_CONTEXT_H_

#include "context.h"

context_t *alloc_std_context(const char *source_file, const char *executable);

void free_std_context(context_t * ptr);

#endif /* _STD_CONTEXT_H_ */
