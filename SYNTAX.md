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

Literal values can be expressed as decimal `nnn`, hex `$xxx`, ASCII `'c'` and used anywhere a numerical value is expected.

Character strings can be expressed as `"text"` and used (only) in .BYTE pseudo-instructions. A final null byte is not automatically included.

Exampes:
```
		* = 4000
		LDA #$5f
		STA $1234
		.BYTE "null-terminated string",0
```

Expressions are evaluated as 16-bit unsigned integers. The can include numeric literals and symbols (variables and labels). The following operators are supported:
* `+` and `-`
* `<` and `>` (low-byte and high-byte)
* `!` (force the value to be 16-bit)
Parentheses are not supported (nor they would make a difference with only additions and subtractions).

The standard 6502 opcodes and addressing modes are supported.
`ASL A` and `ASL` are both valid and equivalent.
`BRK #imm` is not supported.

Pseudo-instructions are:
* `.BYTE`: sequence of bytes
* `.WORD`: sequence of 16-bit words, in low-endian order
* `.INCLUDE`: process an include file; repeat includes are ignored
* `MACRO name [par ...]`: beginning of a macro definition
* `MEND`: end of macro defintion
* `&name [par ...]`: invocation of a macro
* `.IFDEF symbol`, `.IFNDEF symbol`: conditional block, if symbol is/isn't defined
* `.IFZ/.IFNZ expression`: conditional block - if value of expression is zero/nonzero
* `.ENDIF`: end of conditional block
Please note that macros and conditional blocks are not implemented yet.
I do not plan on supporting nested macros (macros defined inside macros). I am on the fence about allowing macro invocation inside macros.
I do not plan on supporting nested conditional blocks.

This is a two-pass assembler.
The first pass populates the symbol table. The second pass emits the code.
If there are inconsistencies between first and second pass, the code will be incorrect or an erro will occur.
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
