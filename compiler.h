/* compiler.h */
#ifndef _COMPILER_H_
#define _COMPILER_H_

#include "context.h"

void compile(context_t *ctx);

void compile_inner(context_t *ctx);

void factor(context_t *ctx);

void term(context_t *ctx);

void expression(context_t *ctx);

#endif /* _COMPILER_H_ */

