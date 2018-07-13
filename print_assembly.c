#include <stdio.h>
#include <assert.h>
#include "header.h"

#define MACOS

#ifdef MACOS
#define PREFIX "_"
#else
#define PREFIX ""
#endif


void print_prologue(int alloc_size)
{
	printf(
		"//print_prologue(%d)\n"
	, alloc_size);
	printf(
		".global " PREFIX "main\n"
		PREFIX "main:\n"
		"  pushq %%rbp\n"
		"  movq %%rsp, %%rbp\n"
	);
	if (alloc_size) { 
		printf("  subq $%d, %%rsp\n", alloc_size);
	}
}

/* write to local mem what's at the top of the stack. does not consume stack. */
void write_to_local(int offset)
{
	assert(offset < 0);
	printf(
		"//write_to_local(%d)\n"
	, offset);
	printf("  movl (%%rsp), %%eax\n");
	printf("  movl %%eax, %d(%%rbp)\n", offset);
}

/* push what's on local mem */
void push_from_local(int offset)
{
	assert(offset < 0);
	printf(
		"//push_from_local(%d)\n"
	, offset);
	printf(
		"  subq $4, %%rsp\n"
		"  movl %d(%%rbp), %%eax\n"
		"  movl %%eax, (%%rsp)\n"
	, offset);
}

void push_int(int num)
{
	printf(
		"//push_int(%d)\n"
	, num);
	printf(
		"  subq $4, %%rsp\n"
		"  movl $%d, (%%rsp)\n"
	, num);
}

void op_ints(const char* str)
{
	printf(
		"//op_ints(\"%s\")\n"
	, str);
	printf(
		"  movl (%%rsp), %%eax\n"
		"  %s %%eax, +4(%%rsp)\n"
		"  addq $4, %%rsp\n"
	, str);
}

void mul_ints(void)
{
	printf(
		"//mul_ints()\n"
	);
	printf(
		"  movl +4(%%rsp), %%eax\n"
		"  imull (%%rsp), %%eax\n"
		"  movl %%eax, +4(%%rsp)\n"
		"  addq $4, %%rsp\n"
	);
}

void div_ints(void)
{
	printf(
		"//div_ints()\n"
	);
	printf(
		"  movl +4(%%rsp), %%eax\n"
  		"  cltd\n"
  		"  idivl (%%rsp)\n"
  		"  movl %%eax, +4(%%rsp)\n"
  		"  addq $4, %%rsp\n"
	);
}

void rem_ints(void)
{
	printf(
		"//rem_ints()\n"
	);
	printf(
		"  movl +4(%%rsp), %%eax\n"
  		"  cltd\n"
  		"  idivl (%%rsp)\n"
  		"  movl %%edx, +4(%%rsp)\n"
  		"  addq $4, %%rsp\n"
	);
}

/*
setl: less than
setle: less than or eq
setg: greater than
setge: greater than or eq
*/
void compare_ints(const char* str)
{
	printf(
		"//compare_ints(\"%s\")\n"
	, str);
	printf(
		"  movl +4(%%rsp), %%eax\n"
		"  cmpl (%%rsp), %%eax\n"
		"  %s %%al\n"
		"  movzbl %%al, %%eax\n"
		"  movl %%eax, +4(%%rsp)\n"
		"  addq $4, %%rsp\n"
	,str);
}

void unary_not(void)
{
	printf(
		"//unary_not()\n"
	);
	printf(
		"  cmpl $0, (%%rsp)\n"
		"  sete %%al\n"
		"  movzbl %%al, %%eax\n"
		"  movl %%eax, (%%rsp)\n"
	);
}

void unary_bitnot(void)
{
	printf(
		"//unary_bitnot()\n"
	);
	printf("  notl (%%rsp)\n");
}

/*
sall: left shift
sarl: right shift
*/
void shift_ints(const char* str)
{
	printf(
		"//shift_ints(\"%s\")\n"
	, str);
	printf(
		"  movl (%%rsp), %%eax\n"
		"  movl %%eax, %%ecx\n"
		"  %s %%cl, +4(%%rsp)\n"
		"  addq $4, %%rsp\n"
	,str);
}

void print_epilogue(int alloc_size)
{
	printf(
		"//print_epilogue(%d)\n"
	, alloc_size);
	printf(
		"  movl (%%rsp), %%eax\n"
		"  addq $%d, %%rsp\n"
		"  movq (%%rbp), %%rbp\n"
		"  ret\n"
	, 12+alloc_size);
}
