# Local x86-64 practice lab

This is a small, runnable companion to the FFmpeg lessons. The lesson repository's advertised assignments are not included, so this lab supplies a working kernel, correctness tests, and a path for creating your own progressively harder assignments.

It targets **Linux x86-64, the System V AMD64 ABI, and SSE2**. Its example is written in GNU assembler (GAS) using Intel syntax; that is deliberately different from the FFmpeg lessons' NASM syntax and `x86inc.asm` macros. The lab is self-contained and needs no NASM.

## Run it

From this directory:

```sh
make test       # build and run the correctness tests
make sanitize   # rebuild/run the C harness with ASan and UBSan
make disasm     # inspect the executable with Intel-syntax disassembly
make debug      # start GDB
make clean      # remove generated files
```

To step through the assembly in GDB:

```sh
gdb --args ./build/test_add_u8
(gdb) set disassembly-flavor intel
(gdb) break add_u8_sse2
(gdb) run
(gdb) info registers
(gdb) x/8i $pc
(gdb) si
```

The tests exercise lengths 0 through 128, all source/destination alignments modulo 16, vector tails, and exact in-place operation. They also compare the entire destination allocation to detect writes outside the requested range. Partial overlap (for example, `dst == src + 1`) is not part of this function's contract.

## First study session

1. Read the prototype and its ABI comment at the top of `src/add_u8.S`.
2. Run `make test`, then read the test/reference implementation in `tests/test_add_u8.c`.
3. For each assembly instruction, write down which register or memory bytes it reads/writes and whether it changes flags.
4. Draw the register contents immediately before and after `paddb`. It performs 16 independent byte additions, wrapping each result modulo 256.
5. Run the GDB commands above and confirm the argument registers (`rdi`, `rsi`, `rdx`) and vector registers (`xmm0`, `xmm1`).
6. Run `make disasm` and find the kernel in the linked executable. Compare source, disassembly, and observed register state.

## Practice progression

Keep each change small. Run the tests after every change; add tests before optimizing.

1. **Scalar baseline:** make a copy of the kernel and implement byte-at-a-time addition only. Confirm it handles zero length and does not read or write past the end.
2. **Control flow:** rewrite the scalar loop using a different counter/branch arrangement. Explain the signed versus unsigned branch conditions and the FLAGS consumed by each branch.
3. **SIMD:** restore a 16-byte SSE2 loop. Keep a scalar tail; test every length around the vector boundary (15, 16, 17, 31, 32, 33).
4. **Arithmetic semantics:** add a second kernel for unsigned saturating addition (`paddusb`). Update the C reference and tests so wrapping (`paddb`) and saturation are distinguished at values such as 250 + 10.
5. **Memory behavior:** add a separate operation such as byte subtraction or clamping. Test unaligned addresses and guard bytes. State exactly which kinds of aliasing are supported.
6. **Compiler literacy:** write an equivalent C function, inspect its output with `gcc -O2 -S -masm=intel`, and compare it to your hand-written implementation. Try `-O0`, `-O3`, and `-fno-tree-vectorize`; don't assume instruction count alone predicts speed.
7. **Performance:** only after correctness, benchmark scalar and SIMD implementations over multiple buffer sizes. Include warmup and repeated measurements, and prevent the compiler from deleting the work. Record CPU/compiler, flags, input sizes, and results. Treat tiny timing differences skeptically.
8. **Dispatch:** add a scalar fallback plus an SSE2/AVX2 implementation and select a safe implementation at runtime. Never execute an ISA your CPU/OS does not support. Keep the baseline binary runnable on machines without AVX2.
9. **FFmpeg-style work:** study how FFmpeg selects implementations, handles pixel strides/edge widths, writes tests, and organizes NASM assembly. Start by reproducing a small existing primitive and comparing it against its C reference.

Suggested mini-projects after the byte-add lab: sum and maximum reductions; pixel alpha blending; clipping signed values to an unsigned byte; grayscale conversion; and a separable image filter. For each, define behavior for edge values and lengths before writing assembly.

## A practical learning sequence

This is a guide, not a promise that assembly mastery fits into a fixed number of weeks.

- **Before assembly:** get comfortable with C pointers/arrays, integer widths, bitwise operations, hex/binary, compilation, linking, and using a shell and Git. Review two's-complement arithmetic and signed/unsigned conversions.
- **Scalar x86-64:** registers, operand sizes, loads/stores, arithmetic, flags, branches, loops, effective addresses, and the System V calling convention. Learn which registers are caller/callee-saved, stack alignment, and return-value rules.
- **Machine-code tools:** compile C to assembly; use `objdump`, GDB, and (optionally) Compiler Explorer. Learn ELF sections, symbols, relocations, and how the linker combines object files.
- **SIMD:** lanes and element widths, wrapping versus saturating arithmetic, widening/narrowing, shuffles, alignment, and safe tails. Begin with SSE2, then add SSSE3/AVX2 only when the problem needs them.
- **Optimization:** establish a correct reference and representative tests first; inspect generated code; benchmark repeatably; then consider cache behavior, throughput/latency, and runtime dispatch.
- **Broader low-level work:** virtual memory, caches, atomics and the memory model, concurrency, system calls, and operating-system interfaces are useful separate topics. Assembly alone does not replace learning C, compiler behavior, or computer architecture.

## Tooling on this machine

The lab uses a C compiler, GNU Make, GDB, and GNU `objdump`. Those are already available in the prepared environment. The exact source lessons use NASM and FFmpeg-specific macros; NASM and FFmpeg's `x86inc.asm` are **not** prerequisites for this lab.

When you are ready to work with those exact lesson examples on Ubuntu, install NASM and the normal build tools:

```sh
sudo apt update
sudo apt install build-essential gdb binutils nasm
```

`x86inc.asm` is maintained in the FFmpeg source tree, not this lesson repo. It depends on FFmpeg's build/configuration conventions, so installing NASM alone does not make the lesson snippets stand-alone programs. Follow the build/test workflow for the FFmpeg revision you study rather than copying the macro file into this lab.

## References

- [x86-64 System V ABI](https://gitlab.com/x86-psABIs/x86-64-ABI): function calls, registers, and stack rules.
- [Intel Software Developer's Manuals](https://www.intel.com/content/www/us/en/developer/articles/technical/intel-sdm.html): authoritative instruction-set reference.
- [AMD64 Architecture Programmer's Manual](https://www.amd.com/en/search/documentation/hub.html): AMD architecture reference.
- [Agner Fog's optimization manuals](https://www.agner.org/optimize/): instruction behavior and optimization methodology.
- [FFmpeg development documentation](https://ffmpeg.org/developer.html): project conventions and contribution/testing guidance.
