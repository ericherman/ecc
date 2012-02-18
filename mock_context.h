/* mock_context.h */
#ifndef _MOCK_CONTEXT_H_
#define _MOCK_CONTEXT_H_

#include "context.h"

typedef struct mock_data_t {
	const char *call[100];
	unsigned int calls;
	int track_lookahead;

	char no_token[1];

	int term[100];
	unsigned int terms;

	const char **token;
	unsigned int tokens;
	unsigned int token_pos;

	unsigned int mock_return_uint_vals[100];
	unsigned int mock_return_uint_val_c;

} mock_data;

context_t *init_fake_context(const char **token, unsigned int tokens);
void free_fake_context(context_t * ctx);

#endif /* _MOCK_CONTEXT_H_ */
