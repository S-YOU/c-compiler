#include "header.h"
#include <stdio.h>

/*

int main() {
    int x;
    int *y;
    x = 3;
    y = &x;
    *y = 171;
    *y += 3;
    return x;
}
*/
int main()
{
	gen_prologue(32, "main");

	/* x = 3; */
	gen_push_int(3);
	gen_write_to_local(-16);
	gen_discard();

	/* y = &x; */
	gen_push_address_of_local(-16);
	gen_write_to_local_8byte(-24);
	gen_discard();

	/* *y = 171; */
	gen_push_from_local_8byte(-24);
	gen_peek_and_dereference();
	gen_push_from_local_8byte(-8);
	gen_push_int(171);
	gen_pop2nd_to_local_8byte(-8);
	gen_discard2nd_8byte();
	gen_assign_to_backed_up_address();
	gen_discard();

	gen_push_from_local_8byte(-24);
	gen_peek_and_dereference();
	gen_push_from_local_8byte(-8);
	gen_push_int(3);
	gen_pop2nd_to_local_8byte(-8);
	gen_op_ints("addl");
	gen_assign_to_backed_up_address();
	gen_discard();

	gen_push_from_local(-16);
	gen_epilogue(12345);
	return 0;
}
