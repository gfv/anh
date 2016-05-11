%ifndef TEST_NAME
  %error Test name not defined
%else
  %define SETUP_FUNC_NAME test_setup_%[TEST_NAME]
  %define RUN_FUNC_NAME test_run_%[TEST_NAME]
%endif

%ifndef UNROLL
  %error Unroll depth undefined
%endif

extern run_count
global %[RUN_FUNC_NAME]
global %[SETUP_FUNC_NAME]

%macro _Test_Start 0
%[RUN_FUNC_NAME]:
  xor rax, rax
  mov r15, [run_count]
test_restart:
%endmacro

%macro _Test_Conclude 0
  sub r15, 1
  jnz test_restart
  ret
%endmacro

%macro _Setup_Start 0
%[SETUP_FUNC_NAME]:
%endmacro

%macro _Setup_End 0
  ret
%endmacro

%macro _Setup_Empty 0
  _Setup_Start
  xor rax, rax
  _Setup_End
%endmacro

section .meta
  unroll_%[TEST_NAME] dq UNROLL
