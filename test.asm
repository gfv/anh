global run_test

extern instruction_count
extern run_count

section .data
pattern:
%rep (1048576 / 8)
dq 0x5555555555555555
%endrep

result: dq 0
checkpoint_start: dq 0

section .bss

section .text
run_test:
  xor rax, rax
  mov r15, [run_count]

reset:
  rdtsc
  shl rdx, 32
  or rax, rdx
  mov [checkpoint_start], rax

test_body:
  mov rsi, 0
  mov [result], rsi
  mov rbx, pattern

.bodyloop:
  mov rax, [rbx + 8 * rsi]
  popcnt rcx, rax
  add [result], rcx
  inc rsi
  cmp rsi, (1048576 / 8)
  jne .bodyloop
  

; looptest
  dec r15
  jg test_body

  rdtsc
  shl rdx, 32
  or rax, rdx
  sub rax, [checkpoint_start]
  mov [instruction_count], rax
shutdown:
  mov rax, [result]
  ret


