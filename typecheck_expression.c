#include "parser.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static enum SimpleBinOp to_simplebinop(enum TokenKind t)
{
	switch (t) {
		case OP_PLUS:
			return SIMPLE_BIN_OP_PLUS;
		case OP_MINUS:
			return SIMPLE_BIN_OP_MINUS;
		case OP_ASTERISK:
			return SIMPLE_BIN_OP_ASTERISK;
		case OP_SLASH:
			return SIMPLE_BIN_OP_SLASH;
		case OP_PERCENT:
			return SIMPLE_BIN_OP_PERCENT;
		case OP_COMMA:
			return SIMPLE_BIN_OP_COMMA;
		case OP_LT:
			return SIMPLE_BIN_OP_LT;
		case OP_LT_EQ:
			return SIMPLE_BIN_OP_LT_EQ;
		case OP_LSHIFT:
			return SIMPLE_BIN_OP_LSHIFT;
		case OP_GT:
			return SIMPLE_BIN_OP_GT;
		case OP_GT_EQ:
			return SIMPLE_BIN_OP_GT_EQ;
		case OP_RSHIFT:
			return SIMPLE_BIN_OP_RSHIFT;
		case OP_AND:
			return SIMPLE_BIN_OP_AND;
		case OP_OR:
			return SIMPLE_BIN_OP_OR;
		case OP_EQ_EQ:
			return SIMPLE_BIN_OP_EQ_EQ;
		case OP_NOT_EQ:
			return SIMPLE_BIN_OP_NOT_EQ;
		case OP_HAT:
			return SIMPLE_BIN_OP_HAT;
		default:
			assert("cannot happen" && 0);
	}
}

static struct Expression binary_op(struct Expression expr,
                                   struct Expression expr2,
                                   enum ExprCategory cat, struct Type type)
{
	struct Expression *ptr_expr1 = calloc(1, sizeof(struct Expression));
	struct Expression *ptr_expr2 = calloc(1, sizeof(struct Expression));
	*ptr_expr1 = expr;
	*ptr_expr2 = expr2;

	struct Expression new_expr;
	new_expr.details.type = type;
	new_expr.category = cat;
	new_expr.ptr1 = ptr_expr1;
	new_expr.ptr2 = ptr_expr2;
	new_expr.ptr3 = 0;

	return new_expr;
}

static struct Expression simple_binary_op(struct Expression expr,
                                          struct Expression expr2,
                                          enum TokenKind kind, struct Type type)
{
	struct Expression *ptr_expr1 = calloc(1, sizeof(struct Expression));
	struct Expression *ptr_expr2 = calloc(1, sizeof(struct Expression));
	*ptr_expr1 = expr;
	*ptr_expr2 = expr2;

	struct Expression new_expr;
	new_expr.details.type = type;
	new_expr.category = SIMPLE_BINARY_EXPR;
	new_expr.simple_binary_operator = to_simplebinop(kind);
	new_expr.ptr1 = ptr_expr1;
	new_expr.ptr2 = ptr_expr2;
	new_expr.ptr3 = 0;

	return new_expr;
}

static struct Expression pointer_plusorminus_int(struct Expression expr,
                                                 struct Expression expr2,
                                                 enum TokenKind kind)
{
	struct Expression *ptr_expr1 = calloc(1, sizeof(struct Expression));
	struct Expression *ptr_expr2 = calloc(1, sizeof(struct Expression));
	*ptr_expr1 = expr;
	*ptr_expr2 = expr2;

	struct Expression new_expr;
	new_expr.details = expr.details;
	new_expr.category = kind == OP_PLUS ? POINTER_PLUS_INT : POINTER_MINUS_INT;
	new_expr.ptr1 = ptr_expr1;
	new_expr.ptr2 = ptr_expr2;
	new_expr.ptr3 = 0;

	return new_expr;
}

static struct Expression combine_by_add(struct Expression expr,
                                        struct Expression expr2)
{
	struct Type type1 = expr.details.type;
	struct Type type2 = expr2.details.type;

	if (is_compatible(type1, INT_TYPE)) {
		if (is_compatible(type2, INT_TYPE)) {
			return simple_binary_op(expr, expr2, OP_PLUS, INT_TYPE);
		} else if (is_pointer(type2)) {
			/* swapped */
			return pointer_plusorminus_int(expr2, expr, OP_PLUS);
		}

	} else if (is_pointer(type1)) {

		expect_type(expr2.details.type, INT_TYPE,
		            "cannot add a pointer to a pointer");
		return pointer_plusorminus_int(expr, expr2, OP_PLUS);
	}
	fprintf(stderr, "fail\n");
	exit(EXIT_FAILURE);
}

static struct Expression combine_by_sub(struct Expression expr,
                                        struct Expression expr2)
{
	struct Type type1 = expr.details.type;
	struct Type type2 = expr2.details.type;

	if (is_compatible(type1, INT_TYPE)) {
		if (is_compatible(type2, INT_TYPE)) {
			return simple_binary_op(expr, expr2, OP_MINUS, INT_TYPE);
		} else if (is_pointer(type2)) {

			fprintf(stderr, "cannot subtract a pointer from an integer.\n");
			exit(EXIT_FAILURE);
		}

	} else if (is_pointer(type1)) {
		if (is_compatible(type2, INT_TYPE)) {

			/* pointer minus int */
			return pointer_plusorminus_int(expr, expr2, OP_MINUS);
		} else {
			/* pointer minus pointer */
			return binary_op(expr, expr2, POINTER_MINUS_POINTER, INT_TYPE);
		}
	}
	fprintf(stderr, "fail\n");
	exit(EXIT_FAILURE);
}

static int is_local_var(struct ScopeChain t, const char *str)
{
	if (isElem(t.var_table, str)) {
		return 1;
	} else if (t.outer == 0) {
		/* most outer, but cannot be found */
		return 0;
	} else {
		return is_local_var(*(t.outer), str);
	}
}

static struct LocalVarInfo resolve_name_locally(struct ScopeChain t,
                                                const char *str)
{
	if (isElem(t.var_table, str)) {
		struct LocalVarInfo *ptr_varinfo = lookup(t.var_table, str);
		return *ptr_varinfo;
	} else if (t.outer == 0) {
		/* most outer, but cannot be found */
		fprintf(stderr, "%s is not declared locally\n", str);
		exit(EXIT_FAILURE);
	} else {
		return resolve_name_locally(*(t.outer), str);
	}
}

int isAssign(enum TokenKind opkind)
{
	return (opkind == OP_EQ || opkind == OP_PLUS_EQ || opkind == OP_MINUS_EQ ||
	        opkind == OP_ASTERISK_EQ || opkind == OP_SLASH_EQ ||
	        opkind == OP_PERCENT_EQ || opkind == OP_LSHIFT_EQ ||
	        opkind == OP_RSHIFT_EQ || opkind == OP_AND_EQ ||
	        opkind == OP_HAT_EQ || opkind == OP_OR_EQ);
}

static struct Type resolve_name_globally(struct Map m, const char *str)
{
	if (isElem(m, str)) {
		struct Type *ptr_type = lookup(m, str);
		return *ptr_type;
	} else {
		fprintf(stderr, "%s is not declared globally\n", str);
		exit(EXIT_FAILURE);
	}
}

static enum UnaryOp to_unaryop(enum TokenKind t)
{
	switch (t) {
		case OP_NOT:
			return UNARY_OP_NOT;
		case OP_TILDA:
			return UNARY_OP_TILDA;
		case OP_PLUS:
			return UNARY_OP_PLUS;
		case OP_MINUS:
			return UNARY_OP_MINUS;

		case OP_PLUS_PLUS:
			return UNARY_OP_PLUS_PLUS;
		case OP_MINUS_MINUS:
			return UNARY_OP_MINUS_MINUS;

		case OP_AND:
			return UNARY_OP_AND;

		case OP_ASTERISK:
			return UNARY_OP_ASTERISK;

		default:
			assert("cannot happen" && 0);
	}
}

static struct Expression deref_expr(struct Expression expr);

static struct Expression unary_op_(struct Expression expr, enum TokenKind kind,
                                   struct Type type)
{
	struct Expression *ptr_expr1 = calloc(1, sizeof(struct Expression));
	*ptr_expr1 = expr;

	struct Expression new_expr;
	new_expr.details.type = type;
	new_expr.category = UNARY_OP_EXPR;
	new_expr.unary_operator = to_unaryop(kind);
	new_expr.ptr1 = ptr_expr1;
	new_expr.ptr2 = 0;
	new_expr.ptr3 = 0;

	return new_expr;
}

static struct Expression deref_expr(struct Expression expr)
{
	struct Type type = deref_type(expr.details.type);

	struct Expression new_expr =
	    unary_op_(expr, OP_ASTERISK, if_array_convert_to_ptr(type));
	new_expr.details.true_type = type;

	return new_expr;
}

static enum SimpleBinOp op_before_assign(enum TokenKind kind)
{
	switch (kind) {
		case OP_EQ:
			return SIMPLE_BIN_OP_COMMA;
		case OP_PLUS_EQ:
			return SIMPLE_BIN_OP_PLUS;
		case OP_MINUS_EQ:
			return SIMPLE_BIN_OP_MINUS;
		case OP_ASTERISK_EQ:
			return SIMPLE_BIN_OP_ASTERISK;
		case OP_SLASH_EQ:
			return SIMPLE_BIN_OP_SLASH;
		case OP_PERCENT_EQ:
			return SIMPLE_BIN_OP_PERCENT;
		case OP_LSHIFT_EQ:
			return SIMPLE_BIN_OP_LSHIFT;
		case OP_RSHIFT_EQ:
			return SIMPLE_BIN_OP_RSHIFT;
		case OP_AND_EQ:
			return SIMPLE_BIN_OP_AND;
		case OP_HAT_EQ:
			return SIMPLE_BIN_OP_HAT;
		case OP_OR_EQ:
			return SIMPLE_BIN_OP_OR;
		default:
			assert("op_before_assign called before a non-assignment operator" &&
			       0);
	}
}

static struct Expression assignment_expr(struct Expression expr,
                                         struct Expression expr2,
                                         enum TokenKind opkind)
{
	struct Expression *ptr_expr1 = calloc(1, sizeof(struct Expression));
	struct Expression *ptr_expr2 = calloc(1, sizeof(struct Expression));
	*ptr_expr1 = expr;
	*ptr_expr2 = expr2;

	struct Expression new_expr;
	new_expr.details = expr.details;
	new_expr.category = ASSIGNMENT_EXPR;
	new_expr.simple_binary_operator = op_before_assign(opkind);
	new_expr.ptr1 = ptr_expr1;
	new_expr.ptr2 = ptr_expr2;
	new_expr.ptr3 = 0;

	return new_expr;
}

struct Expression typecheck_expression(const struct ParserState *ptr_ps,
                                       struct UntypedExpression uexpr)
{
	switch (uexpr.category) {
		case UNARY_EXPR: {
			switch (uexpr.operator) {
				case OP_NOT:
				case OP_TILDA:
				case OP_PLUS:
				case OP_MINUS: {
					enum TokenKind kind = uexpr.operator;
					struct Expression expr =
					    typecheck_expression(ptr_ps, *uexpr.ptr1);
					expect_type(expr.details.type, INT_TYPE,
					            "operand of logical not, bitnot, unary plus or "
					            "unary minus");

					struct Expression new_expr =
					    unary_op_(expr, kind, expr.details.type);
					/* new_expr.details.true_type = expr.details.true_type; */
					/* should not propagate */

					return new_expr;
				}

				case OP_PLUS_PLUS:
				case OP_MINUS_MINUS: {
					enum TokenKind opkind = uexpr.operator;

					struct Expression expr =
					    typecheck_expression(ptr_ps, *uexpr.ptr1);
					expect_type(expr.details.type, INT_TYPE,
					            "operand of unary increment/decrement");

					struct Expression new_expr =
					    unary_op_(expr, opkind, INT_TYPE);
					return new_expr;
				}

				case OP_AND: {
					struct Expression expr;

					expr = typecheck_expression(ptr_ps, *uexpr.ptr1);

					struct Type type = expr.details.type;
					if (is_array(type)) {
						fprintf(stderr, "array is not an lvalue\n");
						exit(EXIT_FAILURE);
					}

					struct Type *ptr_type = calloc(1, sizeof(struct Type));
					*ptr_type = expr.details.type;

					struct Expression new_expr = unary_op_(
					    expr, OP_AND, ptr_of_type_to_ptr_to_type(ptr_type));

					return new_expr;
				}
				case OP_ASTERISK: {
					struct Expression expr =
					    typecheck_expression(ptr_ps, *uexpr.ptr1);

					return deref_expr(expr);
				}
				default: {
					fprintf(stderr,
					        "FAILURE::::::: INVALID TOKEN %d in unary\n",
					        uexpr.operator);
					exit(EXIT_FAILURE);
				}
			}
		}
		case FUNCCALL: {
			const char *ident_str = uexpr.var_name;

			struct Type ret_type;
			if (!isElem(ptr_ps->func_info_map, ident_str)) {
				fprintf(stderr, "Undeclared function `%s()` detected.\n",
				        ident_str);
				fprintf(stderr, "Assumes that `%s()` returns `int`\n",
				        ident_str);
				ret_type = INT_TYPE;
			} else {
				struct FuncInfo *ptr_func_info =
				    lookup(ptr_ps->func_info_map, ident_str);
				ret_type = ptr_func_info->ret_type;
			}

			struct Expression *args = calloc(10, sizeof(struct Expression));

			int counter = 0;
			for (; counter < uexpr.arg_exprs_vec.length; counter++) {
				const struct UntypedExpression *ptr =
				    uexpr.arg_exprs_vec.vector[counter];
				args[counter] = typecheck_expression(ptr_ps, *ptr);
				if (counter > 5) {
					unsupported("calling with 7 or more arguments");
				}
			}

			struct Expression expr;
			expr.category = FUNCCALL_EXPR;
			expr.arg_expr_vec = args;
			expr.arg_length = counter;
			expr.details.type = ret_type;
			expr.global_var_name = ident_str;
			return expr;
		}
		case POSTFIX_EXPR: {
			struct Expression expr = typecheck_expression(ptr_ps, *uexpr.ptr1);
			struct Expression *ptr_expr1 = calloc(1, sizeof(struct Expression));
			*ptr_expr1 = expr;

			enum TokenKind opkind = uexpr.operator;

			struct Expression new_expr;
			new_expr.details.type = INT_TYPE;
			new_expr.category =
			    opkind == OP_PLUS_PLUS ? POSTFIX_INCREMENT : POSTFIX_DECREMENT;
			new_expr.ptr1 = ptr_expr1;
			new_expr.ptr2 = 0;
			new_expr.ptr3 = 0;
			return new_expr;
		}
		case INT_LITERAL_: {
			struct Expression expr;
			expr.details.type = INT_TYPE;
			expr.int_value = uexpr.int_value;
			expr.category = INT_VALUE;
			return expr;
		}
		case VAR: {
			const char *name = uexpr.var_name;

			if (!is_local_var(ptr_ps->scope_chain, name)) {
				struct Type type =
				    resolve_name_globally(ptr_ps->global_vars_type_map, name);

				struct Expression expr;
				expr.details.type = if_array_convert_to_ptr(type);
				;
				expr.details.true_type = type;
				expr.category = GLOBAL_VAR_;
				expr.global_var_name = name;
				return expr;
			} else {
				struct LocalVarInfo info =
				    resolve_name_locally(ptr_ps->scope_chain, name);

				struct Expression expr;
				expr.details.type = if_array_convert_to_ptr(info.type);
				expr.details.true_type = info.type;
				expr.category = LOCAL_VAR_;
				expr.local_var_offset = info.offset;
				return expr;
			}
		}
		case STRING_LITERAL_: {
			struct Type *ptr_char = calloc(1, sizeof(struct Type));
			*ptr_char = CHAR_TYPE;

			struct Expression expr;
			expr.details.type = ptr_of_type_to_ptr_to_type(ptr_char);
			expr.details.true_type = ptr_of_type_to_arr_of_type(
			    ptr_char, strlen(uexpr.literal_string) + 1);
			expr.category = STRING_LITERAL;
			expr.literal_string = uexpr.literal_string;

			return expr;
		}
		case CONDITIONAL: {
			struct Expression expr = typecheck_expression(ptr_ps, *uexpr.ptr1);
			struct Expression true_branch =
			    typecheck_expression(ptr_ps, *uexpr.ptr2);
			struct Expression false_branch =
			    typecheck_expression(ptr_ps, *uexpr.ptr3);
			expect_type(
			    false_branch.details.type, true_branch.details.type,
			    "mismatch of type in the false branch and the true branch");
			struct Expression *ptr_expr1 = calloc(1, sizeof(struct Expression));
			struct Expression *ptr_expr2 = calloc(1, sizeof(struct Expression));
			struct Expression *ptr_expr3 = calloc(1, sizeof(struct Expression));
			*ptr_expr1 = expr;
			*ptr_expr2 = true_branch;
			*ptr_expr3 = false_branch;

			struct Expression new_expr;
			new_expr.details = true_branch.details;
			new_expr.category = CONDITIONAL_EXPR;
			new_expr.ptr1 = ptr_expr1;
			new_expr.ptr2 = ptr_expr2;
			new_expr.ptr3 = ptr_expr3;
			return new_expr;
		}
		case BINARY_EXPR: {
			if (isAssign(uexpr.operator)) {
				struct Expression expr =
				    typecheck_expression(ptr_ps, *uexpr.ptr1);
				struct Expression expr2 =
				    typecheck_expression(ptr_ps, *uexpr.ptr2);
				if (is_array(expr.details.type) ||
				    is_array(expr.details.true_type)) {
					fprintf(stderr, "array is not an lvalue\n");
					exit(EXIT_FAILURE);
				}

				expect_type(expr.details.type, expr2.details.type,
				            "mismatch in assignment operator");

				return assignment_expr(expr, expr2, uexpr.operator);
			}
			switch (uexpr.operator) {
				case OP_PLUS: {
					struct Expression expr =
					    typecheck_expression(ptr_ps, *uexpr.ptr1);
					struct Expression expr2 =
					    typecheck_expression(ptr_ps, *uexpr.ptr2);
					return combine_by_add(expr, expr2);
				}
				case OP_MINUS: {
					struct Expression expr =
					    typecheck_expression(ptr_ps, *uexpr.ptr1);
					struct Expression expr2 =
					    typecheck_expression(ptr_ps, *uexpr.ptr2);
					return combine_by_sub(expr, expr2);
				}
				case OP_AND_AND: {
					struct Expression first_expr =
					    typecheck_expression(ptr_ps, *uexpr.ptr1);
					struct Expression expr2 =
					    typecheck_expression(ptr_ps, *uexpr.ptr2);

					return binary_op(first_expr, expr2, LOGICAL_AND_EXPR,
					                 INT_TYPE);
				}
				case OP_OR_OR: {
					struct Expression expr =
					    typecheck_expression(ptr_ps, *uexpr.ptr1);
					struct Expression expr2 =
					    typecheck_expression(ptr_ps, *uexpr.ptr2);
					return binary_op(expr, expr2, LOGICAL_OR_EXPR, INT_TYPE);
				}
				case OP_OR:
				case OP_AND:
				case OP_EQ_EQ:
				case OP_NOT_EQ:
				case OP_GT:
				case OP_GT_EQ:
				case OP_LT:
				case OP_LT_EQ:
				case OP_LSHIFT:
				case OP_RSHIFT:
				case OP_HAT:
				case OP_ASTERISK:
				case OP_SLASH:
				case OP_PERCENT: {
					struct Expression expr =
					    typecheck_expression(ptr_ps, *uexpr.ptr1);
					struct Expression expr2 =
					    typecheck_expression(ptr_ps, *uexpr.ptr2);
					expect_type(expr.details.type, INT_TYPE,
					            "left operand of an operator");
					expect_type(expr2.details.type, INT_TYPE,
					            "right operand of an operator");
					return simple_binary_op(expr, expr2, uexpr.operator,
					                        expr2.details.type);
				}
				case OP_COMMA: {
					struct Expression expr =
					    typecheck_expression(ptr_ps, *uexpr.ptr1);
					struct Expression expr2 =
					    typecheck_expression(ptr_ps, *uexpr.ptr2);
					return simple_binary_op(expr, expr2, uexpr.operator,
					                        expr2.details.type);
				}
				default: {
					fprintf(stderr,
					        "FAILURE::::::: INVALID TOKEN %d in binary expr\n",
					        uexpr.operator);
					exit(EXIT_FAILURE);
				}
			}
		}
	}
	assert("should not pass here" && 0);
}