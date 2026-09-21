; 6502 Test Suite Assembly Source

; AI-generated with prompt:
; Generate an assembly source file with the instructions in the table, in order. 1-byte operands are 42 and 2-byte operands are 1337.
;
; Manually inserted origin
; Manually corrected branch instruction operands

* = $1234

BRK           ; 0x00
ORA (42,X)    ; 0x01
ORA 42        ; 0x05
ASL 42        ; 0x06
PHP           ; 0x08
ORA #42       ; 0x09
ASL A         ; 0x0A
ORA 1337      ; 0x0D
ASL 1337      ; 0x0E
BPL *+44        ; 0x10
ORA (42),Y    ; 0x11
ORA 42,X      ; 0x15
ASL 42,X      ; 0x16
CLC           ; 0x18
ORA 1337,Y    ; 0x19
ORA 1337,X    ; 0x1D
ASL 1337,X    ; 0x1E
JSR 1337      ; 0x20
AND (42,X)    ; 0x21
BIT 42        ; 0x24
AND 42        ; 0x25
ROL 42        ; 0x26
PLP           ; 0x28
AND #42       ; 0x29
ROL A         ; 0x2A
BIT 1337      ; 0x2C
AND 1337      ; 0x2D
ROL 1337      ; 0x2E
BMI *+44        ; 0x30
AND (42),Y    ; 0x31
AND 42,X      ; 0x35
ROL 42,X      ; 0x36
SEC           ; 0x38
AND 1337,Y    ; 0x39
AND 1337,X    ; 0x3D
ROL 1337,X    ; 0x3E
RTI           ; 0x40
EOR (42,X)    ; 0x41
EOR 42        ; 0x45
LSR 42        ; 0x46
PHA           ; 0x48
EOR #42       ; 0x49
LSR A         ; 0x4A
JMP 1337      ; 0x4C
EOR 1337      ; 0x4D
LSR 1337      ; 0x4E
BVC *+44        ; 0x50
EOR (42),Y    ; 0x51
EOR 42,X      ; 0x55
LSR 42,X      ; 0x56
CLI           ; 0x58
EOR 1337,Y    ; 0x59
EOR 1337,X    ; 0x5D
LSR 1337,X    ; 0x5E
RTS           ; 0x60
ADC (42,X)    ; 0x61
ADC 42        ; 0x65
ROR 42        ; 0x66
PLA           ; 0x68
ADC #42       ; 0x69
ROR A         ; 0x6A
JMP (1337)    ; 0x6C
ADC 1337      ; 0x6D
ROR 1337      ; 0x6E
BVS *+44        ; 0x70
ADC (42),Y    ; 0x71
ADC 42,X      ; 0x75
ROR 42,X      ; 0x76
SEI           ; 0x78
ADC 1337,Y    ; 0x79
ADC 1337,X    ; 0x7D
ROR 1337,X    ; 0x7E
STA (42,X)    ; 0x81
STY 42        ; 0x84
STA 42        ; 0x85
STX 42        ; 0x86
DEY           ; 0x88
TXA           ; 0x8A
STY 1337      ; 0x8C
STA 1337      ; 0x8D
STX 1337      ; 0x8E
BCC *+44        ; 0x90
STA (42),Y    ; 0x91
STY 42,X      ; 0x94
STA 42,X      ; 0x95
STX 42,Y      ; 0x96
TYA           ; 0x98
STA 1337,Y    ; 0x99
TXS           ; 0x9A
STA 1337,X    ; 0x9D
LDY #42       ; 0xA0
LDA (42,X)    ; 0xA1
LDX #42       ; 0xA2
LDY 42        ; 0xA4
LDA 42        ; 0xA5
LDX 42        ; 0xA6
TAY           ; 0xA8
LDA #42       ; 0xA9
TAX           ; 0xAA
LDY 1337      ; 0xAC
LDA 1337      ; 0xAD
LDX 1337      ; 0xAE
BCS *+44        ; 0xB0
LDA (42),Y    ; 0xB1
LDY 42,X      ; 0xB4
LDA 42,X      ; 0xB5
LDX 42,Y      ; 0xB6
CLV           ; 0xB8
LDA 1337,Y    ; 0xB9
TSX           ; 0xBA
LDY 1337,X    ; 0xBC
LDA 1337,X    ; 0xBD
LDX 1337,Y    ; 0xBE
CPY #42       ; 0xC0
CMP (42,X)    ; 0xC1
CPY 42        ; 0xC4
CMP 42        ; 0xC5
DEC 42        ; 0xC6
INY           ; 0xC8
CMP #42       ; 0xC9
DEX           ; 0xCA
CPY 1337      ; 0xCC
CMP 1337      ; 0xCD
DEC 1337      ; 0xCE
BNE *+44        ; 0xD0
CMP (42),Y    ; 0xD1
CMP 42,X      ; 0xD5
DEC 42,X      ; 0xD6
CLD           ; 0xD8
CMP 1337,Y    ; 0xD9
CMP 1337,X    ; 0xDD
DEC 1337,X    ; 0xDE
CPX #42       ; 0xE0
SBC (42,X)    ; 0xE1
CPX 42        ; 0xE4
SBC 42        ; 0xE5
INC 42        ; 0xE6
INX           ; 0xE8
SBC #42       ; 0xE9
NOP           ; 0xEA
CPX 1337      ; 0xEC
SBC 1337      ; 0xED
INC 1337      ; 0xEE
BEQ *+44        ; 0xF0
SBC (42),Y    ; 0xF1
SBC 42,X      ; 0xF5
INC 42,X      ; 0xF6
SED           ; 0xF8
SBC 1337,Y    ; 0xF9
SBC 1337,X    ; 0xFD
INC 1337,X    ; 0xFE
