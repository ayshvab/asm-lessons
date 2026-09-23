Welcome to the FFmpeg School of Assembly Language. You have taken the first step on the most interesting, challenging, and rewarding journey in programming. These lessons will give you a grounding in the way assembly language is written in FFmpeg and open your eyes to what's actually going on in your computer.

## Local study setup

This checkout includes an independent, runnable x86-64 practice lab in [`practice/`](./practice/README.md), with a C test harness, a working SSE2 byte-add kernel, and commands for testing, debugging, sanitizing, and disassembling it. It uses GNU assembler's Intel syntax so it runs with the compiler and tools already installed on a typical Linux development machine.

The original lessons mention assignments that are not present in this repository. The practice lab provides a starting point; its guide also lays out a progression from scalar assembly and the calling convention through SIMD, correctness, and benchmarking. The lesson examples use FFmpeg's NASM-based `x86inc.asm` macros, which are separate from this starter lab and are best explored in a configured FFmpeg source/build tree.

**Required Knowledge**

* Knowledge of C, in particular pointers. If you don't know C, work through [The C Programming Language](https://en.wikipedia.org/wiki/The_C_Programming_Language) book  
* High School Mathematics (scalar vs vector, addition, multiplication etc)

**Lessons**

In this Git repository there are lessons and assignments (not uploaded yet) that correspond with each lessons. By the end of the lessons you'll be able to contribute to FFmpeg.

A discord server is available to answer questions:
https://discord.com/invite/Ks5MhUhqfB

**Translations**

* [English](./README.md)
* [Français](./README.fr.md)
* [Spanish](./README.es.md)
* [Turkish](./README.tr.md)
* [中文](./README.zh.md)
