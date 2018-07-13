void print_prologue(int alloc_size);
void push_int(int num);
void print_epilogue(int alloc_size);
void op_ints(const char* str);
void compare_ints(const char* str);
void mul_ints(void);
void div_ints(void);
void rem_ints(void);
void shift_ints(const char* str);
void unary_not(void);
void unary_bitnot(void);
void push_from_local(int offset);
void write_to_local(int offset);

#pragma once

#define GARBAGE_INT 0xCCCCCCCC
#define BOOL int

enum TokenKind {
	OP_PLUS,
	OP_MINUS,
	END,
	LIT_DEC_INTEGER,
	OP_ASTERISK,
	LEFT_PAREN,
	RIGHT_PAREN,
	OP_SLASH,
	OP_PERCENT,
	OP_COMMA,
	OP_LSHIFT,
	OP_LT_EQ,
	OP_LT,
	OP_RSHIFT,
	OP_GT_EQ,
	OP_GT,
	OP_AND,
	OP_OR,
	OP_EQ_EQ,
	OP_NOT_EQ,
	OP_NOT,
	OP_TILDA,
	OP_EQ,
	IDENT_OR_RESERVED
};

struct Token{
	enum TokenKind kind;
	int int_value;
	const char* ident_str;
};

void print_token(struct Token tok);
struct Token get_token(const char** ptr_to_str);

struct charptANDint {
	const char* ptr;
	int value;
};

struct Int {
	int i;
};