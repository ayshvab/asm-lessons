.intel_syntax noprefix
.text

# void add_u8_sse2(uint8_t *dst, const uint8_t *src, size_t n)
# System V AMD64 arguments: dst=rdi, src=rsi, n=rdx.
# Adds modulo 256. Exact in-place operation (dst == src) is supported;
# partially overlapping ranges are outside this function's contract.
.globl add_u8_sse2
.type add_u8_sse2, @function
add_u8_sse2:
    cmp     rdx, 16
    jb      .Ltail

.Lvector:
    movdqu  xmm0, XMMWORD PTR [rdi]
    movdqu  xmm1, XMMWORD PTR [rsi]
    paddb   xmm0, xmm1
    movdqu  XMMWORD PTR [rdi], xmm0
    add     rdi, 16
    add     rsi, 16
    sub     rdx, 16
    cmp     rdx, 16
    jae     .Lvector

.Ltail:
    test    rdx, rdx
    je      .Ldone

.Lscalar:
    movzx   eax, BYTE PTR [rdi]
    add     al, BYTE PTR [rsi]
    mov     BYTE PTR [rdi], al
    inc     rdi
    inc     rsi
    dec     rdx
    jne     .Lscalar

.Ldone:
    ret
.size add_u8_sse2, .-add_u8_sse2

.section .note.GNU-stack,"",@progbits
