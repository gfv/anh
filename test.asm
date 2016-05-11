global asm_setup
global run_test

extern instruction_count
extern run_count

datasize equ 32 * 1024 * 1024

section .data

pattern:
%rep 256
db 0x55
%endrep

result: dq 0
checkpoint_start: dq 0

section .bss
resd 1
source: resb datasize
resd 1
destination: resb datasize

section .text
run_test:
  xor rax, rax
  mov r15, [run_count]

reset:
  rdtsc
  shl rdx, 32
  or rax, rdx
  mov [checkpoint_start], rax
; setup code complete

; test operation
; cache line size is 64 bytes
; and a Meg contains 16384 cache lines
stride equ 128
test_body:
  mov rsi, source + 56
  mov rdi, destination
  mov rcx, datasize / stride

.mcpy:
  mov rax, [rsi]
  mov [rdi], rax
  sub rcx, 1
  add rsi, stride
  add rdi, 8
  test rcx, rcx
  jnz .mcpy
  
; inloop 
  sub r15, 1
  jg test_body

; destroy code
  rdtsc
  shl rdx, 32
  or rax, rdx
  sub rax, [checkpoint_start]
  mov [instruction_count], rax
shutdown:
  mov rax, [result]
  ret

asm_setup:
  vmovdqu ymm1, [pattern]
  mov rsi, source
  mov rcx, datasize / 32

.load:
  vmovdqu [rsi], ymm1
  sub rcx, 1
  jnz .load
  ret
