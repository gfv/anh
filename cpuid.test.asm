%define TEST_NAME cpuid
%include "test_prologue.asm.inc"

section .text
  _Test_Start
  xor rax, rax

%assign i 0
%rep UNROLL
  cpuid	
  mov [result], rax
%assign i i+8
%endrep

  _Test_Conclude
  _Setup_Empty

section .bss
result resq 1
