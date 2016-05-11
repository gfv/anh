%define TEST_NAME xor3
%include "test_prologue.asm.inc"

section .text
  _Test_Start

%assign i 0
%rep UNROLL
  mov rax, 0x1122334455667788
  mov rbx, 0x8877665544332211

  xor rax, rbx
  xor rbx, rax
  xor rax, rbx
%assign i i+8
%endrep

  _Test_Conclude
  _Setup_Empty

section .bss
result resq 1
