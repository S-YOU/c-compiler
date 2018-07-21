#include "header.h"
#include "map.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct ExprInfo FIXME = {FIXME__TYPE_UNKNOWN};
struct Type INT_TYPE = {INT_, 0};
struct ExprInfo UNASSIGNABLE_INT = {NOT_ASSIGNABLE, {INT_, 0}, GARBAGE_INT};

struct ExprInfo parse_additive_expression(struct ParserState *ptr_ps,
                                          const struct Token **ptr_tokvec);
struct ExprInfo
parse_multiplicative_expression(struct ParserState *ptr_ps,
                                const struct Token **ptr_tokvec);

struct ExprInfo parse_exclusive_OR_expression(struct ParserState *ptr_ps,
                                              const struct Token **ptr_tokvec);
struct ExprInfo parse_OR_expression(struct ParserState *ptr_ps,
                                    const struct Token **ptr_tokvec);
struct ExprInfo parse_AND_expression(struct ParserState *ptr_ps,
                                     const struct Token **ptr_tokvec);
struct ExprInfo parse_equality_expression(struct ParserState *ptr_ps,
                                          const struct Token **ptr_tokvec);
struct ExprInfo parse_relational_expression(struct ParserState *ptr_ps,
                                            const struct Token **ptr_tokvec);
struct ExprInfo parse_shift_expression(struct ParserState *ptr_ps,
                                       const struct Token **ptr_tokvec);

void binary_op(enum TokenKind kind)
{
	switch (kind) {
		case OP_PLUS:
			gen_op_ints("addl");
			return;
		case OP_MINUS:
			gen_op_ints("subl");
			return;
		case OP_ASTERISK:
			gen_mul_ints();
			return;
		case OP_SLASH:
			gen_div_ints();
			return;
		case OP_PERCENT:
			gen_rem_ints();
			return;
		case OP_COMMA:
			gen_op_ints("movl");
			return;
		case OP_LT:
			gen_compare_ints("setl");
			return;
		case OP_LT_EQ:
			gen_compare_ints("setle");
			return;
		case OP_LSHIFT:
			gen_shift_ints("sall");
			return;
		case OP_GT:
			gen_compare_ints("setg");
			return;
		case OP_GT_EQ:
			gen_compare_ints("setge");
			return;
		case OP_RSHIFT:
			gen_shift_ints("sarl");
			return;
		case OP_AND:
			gen_op_ints("andl");
			return;
		case OP_OR:
			gen_op_ints("orl");
			return;
		case OP_EQ_EQ:
			gen_compare_ints("sete");
			return;
		case OP_NOT_EQ:
			gen_compare_ints("setne");
			return;
		case OP_HAT:
			gen_op_ints("xorl");
			return;

		case OP_EQ:
		case OP_PLUS_EQ:
		case OP_MINUS_EQ:
		case OP_ASTERISK_EQ:
		case OP_SLASH_EQ:
		case OP_PERCENT_EQ:
		case OP_LSHIFT_EQ:
		case OP_RSHIFT_EQ:
		case OP_AND_EQ:
		case OP_HAT_EQ:
		case OP_OR_EQ:
		case OP_AND_AND:
		case OP_OR_OR:
			assert("failure!!! must be handled separately!!!!" && 0);

		default:
			assert("failure!!! not a binary op!!!!" && 0);
	}

	assert("unimplemented!!!!" && 0);
}

void print_unary_prefix_op(enum TokenKind kind)
{
	switch (kind) {
		case OP_NOT:
			gen_unary_not();
			return;
		case OP_TILDA:
			gen_unary("notl");
			return;
		case OP_PLUS:
			/* do nothing */
			return;
		case OP_MINUS:
			gen_unary("negl");
			return;
		case IDENT_OR_RESERVED:
		case LEFT_PAREN:
		case RIGHT_PAREN:
		case END:
		case LIT_DEC_INTEGER:
			assert("failure!!! not an op!!!!" && 0);
		default:
			assert("failure!!! not a unary prefix op!!!!" && 0);
	}
	assert("unimplemented!!!!" && 0);
}

struct ExprInfo parse_inclusive_OR_expression(struct ParserState *ptr_ps,
                                              const struct Token **ptr_tokvec)
{
	const struct Token *tokvec = *ptr_tokvec;
	struct ExprInfo expr_info = parse_exclusive_OR_expression(ptr_ps, &tokvec);
	expect_type(expr_info, INT_TYPE);

	while (1) {
		enum TokenKind kind = tokvec[0].kind;
		if (kind != OP_OR) {
			break;
		}
		++tokvec;
		expr_info =
		    remove_leftiness(parse_exclusive_OR_expression(ptr_ps, &tokvec));
		expect_type(expr_info, INT_TYPE);
		binary_op(kind);
	}
	*ptr_tokvec = tokvec;
	return expr_info;
}

struct ExprInfo parse_exclusive_OR_expression(struct ParserState *ptr_ps,
                                              const struct Token **ptr_tokvec)
{
	const struct Token *tokvec = *ptr_tokvec;
	struct ExprInfo expr_info = parse_AND_expression(ptr_ps, &tokvec);
	expect_type(expr_info, INT_TYPE);
	while (1) {
		enum TokenKind kind = tokvec[0].kind;
		if (kind != OP_HAT) {
			break;
		}
		++tokvec;
		expr_info = remove_leftiness(parse_AND_expression(ptr_ps, &tokvec));
		expect_type(expr_info, INT_TYPE);
		binary_op(kind);
	}
	*ptr_tokvec = tokvec;
	return expr_info;
}

struct ExprInfo parse_AND_expression(struct ParserState *ptr_ps,
                                     const struct Token **ptr_tokvec)
{
	const struct Token *tokvec = *ptr_tokvec;
	struct ExprInfo expr_info = parse_equality_expression(ptr_ps, &tokvec);
	expect_type(expr_info, INT_TYPE);
	while (1) {
		enum TokenKind kind = tokvec[0].kind;
		if (kind != OP_AND) {
			break;
		}
		++tokvec;
		expr_info =
		    remove_leftiness(parse_equality_expression(ptr_ps, &tokvec));
		expect_type(expr_info, INT_TYPE);
		binary_op(kind);
	}
	*ptr_tokvec = tokvec;
	return expr_info;
}

struct ExprInfo parse_equality_expression(struct ParserState *ptr_ps,
                                          const struct Token **ptr_tokvec)
{
	const struct Token *tokvec = *ptr_tokvec;
	struct ExprInfo expr_info = parse_relational_expression(ptr_ps, &tokvec);
	expect_type(expr_info, INT_TYPE);
	while (1) {
		enum TokenKind kind = tokvec[0].kind;
		if (kind != OP_EQ_EQ && kind != OP_NOT_EQ) {
			break;
		}
		++tokvec;
		expr_info =
		    remove_leftiness(parse_relational_expression(ptr_ps, &tokvec));
		expect_type(expr_info, INT_TYPE);
		binary_op(kind);
	}
	*ptr_tokvec = tokvec;
	return expr_info;
}

struct ExprInfo parse_relational_expression(struct ParserState *ptr_ps,
                                            const struct Token **ptr_tokvec)
{
	const struct Token *tokvec = *ptr_tokvec;
	struct ExprInfo expr_info = parse_shift_expression(ptr_ps, &tokvec);
	expect_type(expr_info, INT_TYPE);
	while (1) {
		enum TokenKind kind = tokvec[0].kind;
		if (kind != OP_GT && kind != OP_GT_EQ && kind != OP_LT &&
		    kind != OP_LT_EQ) {
			break;
		}
		++tokvec;
		expr_info = remove_leftiness(parse_shift_expression(ptr_ps, &tokvec));
		expect_type(expr_info, INT_TYPE);
		binary_op(kind);
	}
	*ptr_tokvec = tokvec;
	return expr_info;
}

struct ExprInfo parse_shift_expression(struct ParserState *ptr_ps,
                                       const struct Token **ptr_tokvec)
{
	const struct Token *tokvec = *ptr_tokvec;
	struct ExprInfo expr_info = parse_additive_expression(ptr_ps, &tokvec);
	expect_type(expr_info, INT_TYPE);
	while (1) {
		enum TokenKind kind = tokvec[0].kind;
		if (kind != OP_LSHIFT && kind != OP_RSHIFT) {
			break;
		}
		++tokvec;
		expr_info =
		    remove_leftiness(parse_additive_expression(ptr_ps, &tokvec));
		expect_type(expr_info, INT_TYPE);
		binary_op(kind);
	}
	*ptr_tokvec = tokvec;
	return expr_info;
}

struct ExprInfo parse_additive_expression(struct ParserState *ptr_ps,
                                          const struct Token **ptr_tokvec)
{
	const struct Token *tokvec = *ptr_tokvec;
	struct ExprInfo expr_info =
	    parse_multiplicative_expression(ptr_ps, &tokvec);
	expect_type(expr_info, INT_TYPE);
	while (1) {
		enum TokenKind kind = tokvec[0].kind;
		if (kind != OP_PLUS && kind != OP_MINUS) {
			break;
		}
		++tokvec;
		expr_info =
		    remove_leftiness(parse_multiplicative_expression(ptr_ps, &tokvec));
		expect_type(expr_info, INT_TYPE);
		binary_op(kind);
	}
	*ptr_tokvec = tokvec;
	return expr_info;
}

struct ExprInfo parse_multiplicative_expression(struct ParserState *ptr_ps,
                                                const struct Token **ptr_tokvec)
{
	const struct Token *tokvec = *ptr_tokvec;
	struct ExprInfo expr_info = parse_cast_expression(ptr_ps, &tokvec);
	expect_type(expr_info, INT_TYPE);
	while (1) {
		enum TokenKind kind = tokvec[0].kind;
		if (kind != OP_ASTERISK && kind != OP_SLASH && kind != OP_PERCENT) {
			break;
		}
		++tokvec;
		expr_info = remove_leftiness(parse_cast_expression(ptr_ps, &tokvec));
		expect_type(expr_info, INT_TYPE);
		binary_op(kind);
	}
	*ptr_tokvec = tokvec;
	return FIXME;
}
