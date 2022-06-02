;
; Copyright (C) 2019 Assured Information Security, Inc.
;
; Permission is hereby granted, free of charge, to any person obtaining a copy
; of this software and associated documentation files (the "Software"), to deal
; in the Software without restriction, including without limitation the rights
; to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
; copies of the Software, and to permit persons to whom the Software is
; furnished to do so, subject to the following conditions:
;
; The above copyright notice and this permission notice shall be included in all
; copies or substantial portions of the Software.
;
; THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
; IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
; FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
; AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
; LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
; OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
; SOFTWARE.

bits 64
default rel

extern _start_c
global _start:function

section .text

; int64_t _start(uint64_t stack, crt_info_t *crt_info);
_start:

    push rbx
    push rcx
    push rdx
    push rsi
    push rdi
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15

    push rbp
    mov rbp, rsp

; NOTE:
;
; We use the OSTYPE instead of the ABI type because the ABI for the VMM is
; SYSV, but in this case the OS is Windows. The whole point of this file
; is to transtion from the OSTYPE to the VMM.
;

%ifdef WIN64
    mov rsp, rcx    ; stack
    mov r13, rdx    ; crt_info
%else
    mov rsp, rdi    ; stack
    mov r13, rsi    ; crt_info
%endif

    and rsp, 0xFFFFFFFFFFFFFFE0

    sub rsp, 0x10
    movdqa [rsp], xmm0
    sub rsp, 0x10
    movdqa [rsp], xmm1
    sub rsp, 0x10
    movdqa [rsp], xmm2
    sub rsp, 0x10
    movdqa [rsp], xmm3
    sub rsp, 0x10
    movdqa [rsp], xmm4
    sub rsp, 0x10
    movdqa [rsp], xmm5
    sub rsp, 0x10
    movdqa [rsp], xmm6
    sub rsp, 0x10
    movdqa [rsp], xmm7
    sub rsp, 0x10
    movdqa [rsp], xmm8
    sub rsp, 0x10
    movdqa [rsp], xmm9
    sub rsp, 0x10
    movdqa [rsp], xmm10
    sub rsp, 0x10
    movdqa [rsp], xmm11
    sub rsp, 0x10
    movdqa [rsp], xmm12
    sub rsp, 0x10
    movdqa [rsp], xmm13
    sub rsp, 0x10
    movdqa [rsp], xmm14
    sub rsp, 0x10
    movdqa [rsp], xmm15

    sub rsp, 0x08
    stmxcsr [rsp]

    mov rax, 0xABCDEF1234567890
    push rax

    mov rdi, r13
    call _start_c wrt ..plt
    mov r11, rax

    pop rax
    mov rbx, 0xABCDEF1234567890
    cmp rax, rbx
    jne stack_overflow

    ldmxcsr [rsp]
    add rsp, 0x08

    movdqa xmm15, [rsp]
    add rsp, 0x10
    movdqa xmm14, [rsp]
    add rsp, 0x10
    movdqa xmm13, [rsp]
    add rsp, 0x10
    movdqa xmm12, [rsp]
    add rsp, 0x10
    movdqa xmm11, [rsp]
    add rsp, 0x10
    movdqa xmm10, [rsp]
    add rsp, 0x10
    movdqa xmm9, [rsp]
    add rsp, 0x10
    movdqa xmm8, [rsp]
    add rsp, 0x10
    movdqa xmm7, [rsp]
    add rsp, 0x10
    movdqa xmm6, [rsp]
    add rsp, 0x10
    movdqa xmm5, [rsp]
    add rsp, 0x10
    movdqa xmm4, [rsp]
    add rsp, 0x10
    movdqa xmm3, [rsp]
    add rsp, 0x10
    movdqa xmm2, [rsp]
    add rsp, 0x10
    movdqa xmm1, [rsp]
    add rsp, 0x10
    movdqa xmm0, [rsp]
    add rsp, 0x10

    mov rax, r11
    leave

    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rdi
    pop rsi
    pop rdx
    pop rcx
    pop rbx

    ret

stack_overflow:

    mov rax, 0x8000000000000010
    leave

    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rdi
    pop rsi
    pop rdx
    pop rcx
    pop rbx

    ret