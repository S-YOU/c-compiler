.global _main
_main:
  pushq %rbp
  movq %rsp, %rbp
  subq $4, %rsp
  movl $7, -0(%rsp)
  subq $4, %rsp
  movl $5, -0(%rsp)
  movl +4(%rsp), %eax
  imull -0(%rsp), %eax
  movl %eax, +4(%rsp)
  addq $4, %rsp
  subq $4, %rsp
  movl $12, -0(%rsp)
  subq $4, %rsp
  movl $41, -0(%rsp)
  subq $4, %rsp
  movl $3, -0(%rsp)
  movl +4(%rsp), %eax
  imull -0(%rsp), %eax
  movl %eax, +4(%rsp)
  addq $4, %rsp
  movl -0(%rsp), %eax
  movl %eax, +4(%rsp)
  addq $4, %rsp
  subq $4, %rsp
  movl $7, -0(%rsp)
  subq $4, %rsp
  movl $16, -0(%rsp)
  movl +4(%rsp), %eax
  imull -0(%rsp), %eax
  movl %eax, +4(%rsp)
  addq $4, %rsp
  subq $4, %rsp
  movl $9, -0(%rsp)
  subq $4, %rsp
  movl $2, -0(%rsp)
  movl -0(%rsp), %eax
  movl %eax, +4(%rsp)
  addq $4, %rsp
  movl +4(%rsp), %eax
  cltd
  idivl -0(%rsp)
  movl %eax, +4(%rsp)
  addq $4, %rsp
  movl -0(%rsp), %eax
  addl %eax, +4(%rsp)
  addq $4, %rsp
  subq $4, %rsp
  movl $10, -0(%rsp)
  subq $4, %rsp
  movl $8, -0(%rsp)
  subq $4, %rsp
  movl $3, -0(%rsp)
  movl +4(%rsp), %eax
  cltd
  idivl -0(%rsp)
  movl %edx, +4(%rsp)
  addq $4, %rsp
  movl +4(%rsp), %eax
  cltd
  idivl -0(%rsp)
  movl %eax, +4(%rsp)
  addq $4, %rsp
  movl -0(%rsp), %eax
  subl %eax, +4(%rsp)
  addq $4, %rsp
  movl -0(%rsp), %eax
  movl %eax, +4(%rsp)
  addq $4, %rsp
  movl -0(%rsp), %eax
  addq $4, %rsp
  popq %rbp
  ret
