void gen_prologue(int alloc_size, const char* fname);
void gen_push_int(int num);
void gen_op_ints(const char* str);
void gen_compare_ints(const char* str);
void gen_mul_ints(void);
void gen_div_ints(void);
void gen_rem_ints(void);
void gen_shift_ints(const char* str);
void gen_unary_not(void);
void gen_unary(const char* str);
void gen_push_from_local(int offset);
void gen_write_to_local(int offset);
void gen_epilogue(int label);
void gen_push_ret_of(const char* fname);
void gen_pop_to_reg(const char *str);
void gen_write_register_to_local(const char* str, int offset);
void gen_logical_AND_set(int offset, int label1);
void gen_logical_OR_set(int offset, int label1);
void gen_logical_AND_final(int final_depth, int label1, int label2);
void gen_logical_OR_final(int final_depth, int label1, int label2);
void gen_ternary_part1(int label1, int label2);
void gen_ternary_part2(int label1, int label2);
void gen_ternary_part3(int label1, int label2);
void gen_if_else_part1(int label1, int label2);
void gen_if_else_part2(int label1, int label2);
void gen_if_else_part3(int label1, int label2);
void gen_label(int label1);
void gen_do_while_final(int label1, int label2);
void gen_while_part2(int label1, int label2);
void gen_discard(void);
void gen_for_part4(int label1, int label2);
void gen_push_address_of_local(int offset);
void gen_write_to_local_8byte(int offset);
void gen_push_from_local_8byte(int offset);
void gen_peek_and_dereference(void);
void gen_peek_and_dereference_8byte(void);
void gen_assign_to_backed_up_address(void);
void gen_jump(int label, const char *comment);
void gen_pop2nd_to_local_8byte(int offset);
void gen_discard2nd_8byte(void);
void gen_pop_to_reg_8byte(const char *str);
void gen_write_register_to_local_8byte(const char *str, int offset);
void gen_epilogue_8byte(int label);
void gen_push_ret_of_8byte(const char *fname);
void gen_cltq(void);
void gen_op_8byte(const char *str);
void gen_mul_by_const(int mul);
void gen_swap(void);
void gen_div_by_const(int num);
void gen_before_epilogue(int label1, int label2, int capacity);
void gen_after_prologue(int label1, int label2);
void gen_write_to_global_8byte(const char *ident);
void gen_push_from_global_8byte(const char *ident);
void gen_push_from_global_4byte(const char *ident);
void gen_write_to_global_4byte(const char *ident);
void gen_global_declaration(const char *ident, int size);
void gen_push_address_of_global(const char *ident);
void gen_peek_deref_push_4byte(void);
void gen_peek_deref_push_8byte(void);
void gen_assign_8byte(void);
void gen_assign_4byte(void);
void gen_discard3rd_8byte(void);

#ifdef OSX
#define PREFIX "_"
#else
#define PREFIX ""
#endif

