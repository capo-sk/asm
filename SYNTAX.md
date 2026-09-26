General format for each source line:
```
[<label_def>:] [<instruction>] [;<comment>]
```

The maximum length of a line is 255 characters (see `_buf_line_max` in buffer.hpp).

Whitespace between tokens is ignored.

Comments start with **;** and go to the end of the line.

A label definition is an identifier followed by a colon **:**.
An identifier is a word starting with a letter **A-Z a-z**, underscore **_**, or dot **.**; other allowed characters are digits **0-9**, ampersand **&** and dollar **$**, but these can't be at the front as they identify other types of token. The maximum length of an identifier (or any token, in fact) is 63 characters (see `MAX_TOKEN_LENGTH` in tkstream.hpp).
Labels can only be defined once.

A variable assignment takes the form `<variable> = <expression>`.
Variables can be reassigned a new value.

The special assignment `* = <expression>` changes the memory location.
Incidentally, the default start location is 0 ($0000), which is probably not where you want your code. It is probably a good idea to start the source code with a `* =` assignment.

A symbol (label or variable) starting with dot (.) is treated as local. The symbol is qualified by prepending the most recent non-local label name. This applies to both labels and variables. The combined name must fit in 63 characters.
In the example below, `.count` is qualified to `proc.count` and `.loop` to `proc.loop`:
```
	proc:
	.count = 10
		LDX #.count
	.loop:	DEX
		BNE .loop
		RTS
```

A symbol starting with two dots is local to a macro invocation. It is qualified with the macro name and a unique sequence number.
Example:
```
	.MACRO count num
		LDX #num
		BEQ ..finish
	..loop:
		DEX
		BNE ..loop
	..finish:
	.ENDM
```

Literal values can be expressed as decimal `nnn`, hex `$xxx`, ASCII `'c'` and used anywhere a numerical value is expected.

Character strings can be expressed as `"text"` and used (only) in .BYTE pseudo-instructions. A final null byte is not automatically included.

Exampes:
```
		* = 4000
		LDA #$5f
		STA $1234
		.BYTE "null-terminated string",0
```

Expressions yield 16-bit unsigned integer results. They can include numeric literals and symbols (variables and labels). The following operators are supported:
* the four basic arithmetic operations `+`, `-`, `*` and `/`
* `*` can also be used as operand, in which case it takes the value of the current location
* `<` and `>` (low-byte and high-byte)
* `^` (shift left by 8 bits, i.e. takes a page number and makes it an address)
* `!` (force the value to be 16-bit)
Operands and intermediate results are computed as 64-bit unsigned integers (because it's 2026 not 1986) but the result must fit in 16 bits; values from (two-s complement of) -32768 to +65535 are allowed.
Operations are left-associative and there is no precedence (e.g. of multiplication over addition); parentheses for precedence are not supported.

The standard 6502 opcodes and addressing modes are supported.
`ASL A` and `ASL` are both valid and equivalent.
`BRK #imm` is not supported. Use `BRK` and then `.BYTE` instead.

Pseudo-instructions are:
* `.BYTE`: sequence of bytes
* `.WORD`: sequence of 16-bit words, in low-endian order
* `.INCLUDE`: process an include file; repeat includes are ignored
* `.ALIGN`: align location to the next multiple
* `.MACRO name [par ...]`: beginning of a macro definition
* `.ENDM`: end of macro defintion
* `&name [par ...]`: invocation of a macro
I do not plan on supporting nested macro definitions (macros defined inside macros). Nested macro invocations (one macro invoking another) are supported, but not recursive invocations: a macro invoking itself is blocked; indirect recursion is not blocked but the behaviour is undefined.

Potential enhancements being considered, but not implemented yet:
* conditional blocks (`.IF expression / .ELIF / .ELSE / .ENDIF`); expression is true if non-zero
* variants: `.IFZ` (true if zero); `.IFP / .IFN` (if positive / negative)
* that should cover all usual conditions (e.g. `.IF a-b` is true if *a* equals *b*, `.IFP a-b` is true if *a* is greater than *b*)
* maybe a `.DEFINED(symbol)` function; or maybe `.IFDEF / .IFNDEF` instead
* repeats for e.g. unrolling loops (`.REPEAT expression / .ENDR`)
I do not plan on supporting nested conditional blocks or nested repeat loops.

This is a two-pass assembler.
The first pass populates the symbol table. The second pass emits the code.
If there are inconsistencies between first and second pass, the code will be incorrect or an error will occur.
After the first pass, the symbol table is output to stdout (human readable) or to a text file (VICE monitor format).

Ambiguity between absolute and zero page modes is resolved (independently during each pass) based on the value of the argument: if the value is 0-255, zero page mode is preferred; if not, absolute is needed. `!` can be used to force 16-bit argument and therefore absolute mode.
```
	LDA	$80		;zero page mode
	LDA	$1000		;absolute mode
	LDA	!$80		;absolute mode
	LDX	($80),Y		;zero page mode, indexed by Y
	LDA	($80),Y		;absolute mode, indexed by Y (LDA does not have a ZP,Y mode)
	JMP	$80		;absolute mode (JMP does not have ZP mode)
```

During the first pass, a yet undefined symbol is assumed to have the value of the current location. This can lead to inconsistencies between first and second pass, resulting in incorrect output.
***THERE IS NO PROTECTION AGAINST THESE INCONSISTENCIES***.
Examples:
```
* = $1000
	LDA	variable1	;first pass assumes variable=$1000 -> absolute mode
				;second pass knows variable=$fe -> zero page mode
variable1 = $fe
```

```
* = $C0
	LDA	variable2	;first pass assumes variable=$C0 -> zero page mode
				;second pass knows variable=$1234 -> absolute mode
variable2 = $1234
```

The binary output is Commodore PRG format: one word with the load location, followed by the binary memory dump.
