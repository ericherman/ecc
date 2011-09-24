/* compiler_inner.h */
#ifndef _COMPILER_INNER_H_
#define _COMPILER_INNER_H_

#include "context.h"

void compile_inner(context_t * ctx);

void factor(context_t * ctx);

void term(context_t * ctx);

void expression(context_t * ctx, const char *token);

#endif /* _COMPILER_INNER_H_ */
