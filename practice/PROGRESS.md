# Assembly learning progress checkpoint

Use this note to resume the interactive walk-through. The learner prefers short sections, plain-language explanations, and a check question before moving on.

## Workspace

- Checkout: `/home/ays/Work/asm-lessons`
- Branch: `learning-setup`
- Fork: `https://github.com/ayshvab/asm-lessons`
- Upstream: `https://github.com/FFmpeg/asm-lessons`
- Practice lab: `practice/README.md`; run it with `cd practice && make test`.
- The lab uses GAS Intel syntax. FFmpeg's lesson examples use NASM plus `x86inc.asm`; do not imply that the FFmpeg macros can be used directly in GAS.

## Covered

### Lesson 1 — complete

- Assembly mnemonics are assembled into machine code; scalar instructions work on individual values, while SIMD instructions operate on lanes packed in vector registers.
- XMM is 128 bits: 16 byte lanes or 8 word lanes. `paddb` performs independent modulo-256 byte additions; it does not saturate or carry between lanes.
- Intel syntax uses destination first. Registers, widths, immediate operands, and the purpose of FFmpeg aliases such as `r0q` were discussed. The x86inc aliases expand at assembly time based on configured architecture/ABI; they do not perform runtime CPU detection.
- The first FFmpeg function was traced: `movu` loads/stores 16 bytes without requiring alignment; `paddb` adds; `RET` returns. The caller must provide sufficient valid memory and SSE2 support. An out-of-bounds vector access may silently read/write adjacent mapped memory or fault at an inaccessible page; a fault is not guaranteed.
- Alignment: e.g. aligned `movdqa` requires a 16-byte-aligned address; `movdqu` allows unaligned addresses. Unaligned does not mean safe past the buffer boundary.
- Instruction latency/throughput are microarchitecture-dependent; do not assume fewer instructions are always faster. AVX2 is not a universal x86-64 baseline; keep a baseline and runtime-dispatch optimized versions.

### Lesson 2 — complete

- Labels and unconditional `jmp`; `dec`/conditional branches; loops and FLAGS.
- `cmp` sets flags without storing the subtraction result. Conditional jumps consume flags from the preceding flag-setting instruction. `jg`/`jl` are signed conditions.
- NASM constants `db`, `dw`, `times`; GAS counterparts `.byte`, `.short`, `.rept` / `.endr`.
- Array address form: `base + index*scale + displacement`; struct-field addressing is `base + i*sizeof(struct) + offsetof(field)`, accounting for padding.
- `lea` computes that arithmetic into a register; it does not dereference memory or change flags.

## Current point — Lesson 3

Instruction-set extensions and baseline/runtime dispatch have been introduced. We are now covering the negative-pointer-offset loop trick. The displayed FFmpeg snippet assumes a positive width divisible by `mmsize`; in this XMM example `mmsize` is 16. It has no pre-loop check, so zero or a partial final vector would not be safe as a general-purpose length.

The current check question is still unanswered. Ask the learner first; do not reveal the answer unless requested:

> Starting with `width = 32` and `mmsize = 16`, relative to the original buffer start, what address does the first vector load use, and what address does the second use?

Then continue Lesson 3 with alignment, range/sign extension, packing, and shuffles. Keep presenting one concept at a time and check understanding interactively.
