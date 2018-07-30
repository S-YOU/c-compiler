#include "header.h"
#include <stdio.h>
#include <stdlib.h>

void error_unexpected_token(const struct Token *tokvec, const char *str)
{
	fprintf(stderr, "Unexpected token: `");
	print_token(tokvec[0]);
	fprintf(stderr, "` while expecting %s. \n", str);
	fprintf(stderr, "Next token: `");
	print_token(tokvec[1]);
	fprintf(stderr, "`\n");
	fprintf(stderr, "Previous token: `");
	print_token(
	    tokvec[-1]); /* it does not fail if tokvec[0] was the first token, since
	                    there always is at least one token (BEGINNING) */
	fprintf(stderr, "`\n");
	exit(EXIT_FAILURE);
}

void expect_and_consume(const struct Token **ptr_tokvec, enum TokenKind kind,
                        const char *str)
{
	const struct Token *tokvec = *ptr_tokvec;

	if (tokvec[0].kind != kind) {
		error_unexpected_token(tokvec, str);
	}
	++tokvec;
	*ptr_tokvec = tokvec;
}