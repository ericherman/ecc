/* ecc_std_context.h */
#ifndef ECC_STD_CONTEXT_H
#define ECC_STD_CONTEXT_H

#include "ecc_context.h"

ecc_context *alloc_std_context(const char *source_file, const char *out_file);

void free_std_context(ecc_context *ptr);

#endif /* ECC_STD_CONTEXT_H */
