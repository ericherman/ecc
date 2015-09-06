/* std_context.h */
#ifndef STD_CONTEXT_H
#define STD_CONTEXT_H

#include "context.h"

context_t *alloc_std_context(const char *source_file, const char *out_file);

void free_std_context(context_t * ptr);

#endif /* STD_CONTEXT_H */
