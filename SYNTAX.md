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

A symbol (label or variable) starting with dot (.) is treated as local. The symbol is qualified by prepending the most recent non-local label name. The combined name must fit in 63 characters.
In the example below, `.loop` is qualified to `proc.loop`:
```
	proc:	LDX #10
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
* (macros are not implemented yet)
