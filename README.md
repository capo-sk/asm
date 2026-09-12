An assembler for the 6502 8-bit CPU.

It doesn't quite have a name yet. It is just a pet project born during a very long flight across the Pacific. Something to fill several hours without connectivity. By the end of the return flight, I had a prototype which could assemble a couple of opcodes.

The original plan was to prototype it in C and then translate to 6502 assembly, self-assemble and self-host on C64. Kind of a bootstrap the hard way.

After using the prototype for a little while, I decided that I prefer it to be cross-assembler after all.
The assembler runs on modern machine and OS (I am using Debian on x64 but it should be straightforward to use as-is or port/adapt to a different OS and architecture).

I have since converted the code to C++. It could use a bit of clean-up but it seems to work so there's that.

Please refer to SYNTAX.md for the assembly syntax supported.
