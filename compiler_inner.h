/* compiler_inner.h */
#ifndef COMPILER_INNER_H
#define COMPILER_INNER_H

#include "ecc_context.h"

void compile_inner(ecc_context *ctx);

void factor(ecc_context *ctx, const char *token);

void term(ecc_context *ctx, const char *token);

void expression(ecc_context *ctx, const char *token);

void declaration(ecc_context *ctx, const char *token);

void assignment(ecc_context *ctx, const char *token);

#endif /* COMPILER_INNER_H */
