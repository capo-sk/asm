An assembler for the 6502 8-bit CPU.

The original plan to prototype it in C and then translate to 6502 assembly, self-assemble and self-host on C64.

After using the prototype for a little while, I decided that I prefer it to be cross-assembler after all.
The new goal is for it to be portable C, assuming a modern 32-bit or 64-bit host.

The main target platform is Commodore 64 as that's what I'm familiar with.

2026-09-08 Moved most of the code to C++.
