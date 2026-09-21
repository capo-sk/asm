/* 6502 Assembler test suite

Mostly AI-generated with prompts:
create a table with all the valid 6502 opcodes, tracking each valid addressing mode as a separate row; sort by opcode number; each row must include the opcode number, the mnemonic, and the size of the operand in bytes (0, 1, or 2). Use C language. Use a struct for the row and an array for the table 
create a function that generates a binary file with all the instructions in the table in order. All 1-byte arguments are 42, all 2-byte arguments are 1337
*/


#include <stdint.h>
#include <stdio.h>


typedef struct {
    uint8_t opcode;
    const char* mnemonic;
    uint8_t operand_size;
} OpcodeInfo;

const OpcodeInfo opcodes_6502[] = {
    {0x00, "BRK", 0}, // Implied
    {0x01, "ORA", 1}, // (Indirect, X)
    {0x05, "ORA", 1}, // Zero Page
    {0x06, "ASL", 1}, // Zero Page
    {0x08, "PHP", 0}, // Implied
    {0x09, "ORA", 1}, // Immediate
    {0x0A, "ASL", 0}, // Accumulator
    {0x0D, "ORA", 2}, // Absolute
    {0x0E, "ASL", 2}, // Absolute
    {0x10, "BPL", 1}, // Relative
    {0x11, "ORA", 1}, // (Indirect), Y
    {0x15, "ORA", 1}, // Zero Page, X
    {0x16, "ASL", 1}, // Zero Page, X
    {0x18, "CLC", 0}, // Implied
    {0x19, "ORA", 2}, // Absolute, Y
    {0x1D, "ORA", 2}, // Absolute, X
    {0x1E, "ASL", 2}, // Absolute, X
    {0x20, "JSR", 2}, // Absolute
    {0x21, "AND", 1}, // (Indirect, X)
    {0x24, "BIT", 1}, // Zero Page
    {0x25, "AND", 1}, // Zero Page
    {0x26, "ROL", 1}, // Zero Page
    {0x28, "PLP", 0}, // Implied
    {0x29, "AND", 1}, // Immediate
    {0x2A, "ROL", 0}, // Accumulator
    {0x2C, "BIT", 2}, // Absolute
    {0x2D, "AND", 2}, // Absolute
    {0x2E, "ROL", 2}, // Absolute
    {0x30, "BMI", 1}, // Relative
    {0x31, "AND", 1}, // (Indirect), Y
    {0x35, "AND", 1}, // Zero Page, X
    {0x36, "ROL", 1}, // Zero Page, X
    {0x38, "SEC", 0}, // Implied
    {0x39, "AND", 2}, // Absolute, Y
    {0x3D, "AND", 2}, // Absolute, X
    {0x3E, "ROL", 2}, // Absolute, X
    {0x40, "RTI", 0}, // Implied
    {0x41, "EOR", 1}, // (Indirect, X)
    {0x45, "EOR", 1}, // Zero Page
    {0x46, "LSR", 1}, // Zero Page
    {0x48, "PHA", 0}, // Implied
    {0x49, "EOR", 1}, // Immediate
    {0x4A, "LSR", 0}, // Accumulator
    {0x4C, "JMP", 2}, // Absolute
    {0x4D, "EOR", 2}, // Absolute
    {0x4E, "LSR", 2}, // Absolute
    {0x50, "BVC", 1}, // Relative
    {0x51, "EOR", 1}, // (Indirect), Y
    {0x55, "EOR", 1}, // Zero Page, X
    {0x56, "LSR", 1}, // Zero Page, X
    {0x58, "CLI", 0}, // Implied
    {0x59, "EOR", 2}, // Absolute, Y
    {0x5D, "EOR", 2}, // Absolute, X
    {0x5E, "LSR", 2}, // Absolute, X
    {0x60, "RTS", 0}, // Implied
    {0x61, "ADC", 1}, // (Indirect, X)
    {0x65, "ADC", 1}, // Zero Page
    {0x66, "ROR", 1}, // Zero Page
    {0x68, "PLA", 0}, // Implied
    {0x69, "ADC", 1}, // Immediate
    {0x6A, "ROR", 0}, // Accumulator
    {0x6C, "JMP", 2}, // Indirect
    {0x6D, "ADC", 2}, // Absolute
    {0x6E, "ROR", 2}, // Absolute
    {0x70, "BVS", 1}, // Relative
    {0x71, "ADC", 1}, // (Indirect), Y
    {0x75, "ADC", 1}, // Zero Page, X
    {0x76, "ROR", 1}, // Zero Page, X
    {0x78, "SEI", 0}, // Implied
    {0x79, "ADC", 2}, // Absolute, Y
    {0x7D, "ADC", 2}, // Absolute, X
    {0x7E, "ROR", 2}, // Absolute, X
    {0x81, "STA", 1}, // (Indirect, X)
    {0x84, "STY", 1}, // Zero Page
    {0x85, "STA", 1}, // Zero Page
    {0x86, "STX", 1}, // Zero Page
    {0x88, "DEY", 0}, // Implied
    {0x8A, "TXA", 0}, // Implied
    {0x8C, "STY", 2}, // Absolute
    {0x8D, "STA", 2}, // Absolute
    {0x8E, "STX", 2}, // Absolute
    {0x90, "BCC", 1}, // Relative
    {0x91, "STA", 1}, // (Indirect), Y
    {0x94, "STY", 1}, // Zero Page, X
    {0x95, "STA", 1}, // Zero Page, X
    {0x96, "STX", 1}, // Zero Page, Y
    {0x98, "TYA", 0}, // Implied
    {0x99, "STA", 2}, // Absolute, Y
    {0x9A, "TXS", 0}, // Implied
    {0x9D, "STA", 2}, // Absolute, X
    {0xA0, "LDY", 1}, // Immediate
    {0xA1, "LDA", 1}, // (Indirect, X)
    {0xA2, "LDX", 1}, // Immediate
    {0xA4, "LDY", 1}, // Zero Page
    {0xA5, "LDA", 1}, // Zero Page
    {0xA6, "LDX", 1}, // Zero Page
    {0xA8, "TAY", 0}, // Implied
    {0xA9, "LDA", 1}, // Immediate
    {0xAA, "TAX", 0}, // Implied
    {0xAC, "LDY", 2}, // Absolute
    {0xAD, "LDA", 2}, // Absolute
    {0xAE, "LDX", 2}, // Absolute
    {0xB0, "BCS", 1}, // Relative
    {0xB1, "LDA", 1}, // (Indirect), Y
    {0xB4, "LDY", 1}, // Zero Page, X
    {0xB5, "LDA", 1}, // Zero Page, X
    {0xB6, "LDX", 1}, // Zero Page, Y
    {0xB8, "CLV", 0}, // Implied
    {0xB9, "LDA", 2}, // Absolute, Y
    {0xBA, "TSX", 0}, // Implied
    {0xBC, "LDY", 2}, // Absolute, X
    {0xBD, "LDA", 2}, // Absolute, X
    {0xBE, "LDX", 2}, // Absolute, Y
    {0xC0, "CPY", 1}, // Immediate
    {0xC1, "CMP", 1}, // (Indirect, X)
    {0xC4, "CPY", 1}, // Zero Page
    {0xC5, "CMP", 1}, // Zero Page
    {0xC6, "DEC", 1}, // Zero Page
    {0xC8, "INY", 0}, // Implied
    {0xC9, "CMP", 1}, // Immediate
    {0xCA, "DEX", 0}, // Implied
    {0xCC, "CPY", 2}, // Absolute
    {0xCD, "CMP", 2}, // Absolute
    {0xCE, "DEC", 2}, // Absolute
    {0xD0, "BNE", 1}, // Relative
    {0xD1, "CMP", 1}, // (Indirect), Y
    {0xD5, "CMP", 1}, // Zero Page, X
    {0xD6, "DEC", 1}, // Zero Page, X
    {0xD8, "CLD", 0}, // Implied
    {0xD9, "CMP", 2}, // Absolute, Y
    {0xDD, "CMP", 2}, // Absolute, X
    {0xDE, "DEC", 2}, // Absolute, X
    {0xE0, "CPX", 1}, // Immediate
    {0xE1, "SBC", 1}, // (Indirect, X)
    {0xE4, "CPX", 1}, // Zero Page
    {0xE5, "SBC", 1}, // Zero Page
    {0xE6, "INC", 1}, // Zero Page
    {0xE8, "INX", 0}, // Implied
    {0xE9, "SBC", 1}, // Immediate
    {0xEA, "NOP", 0}, // Implied
    {0xEC, "CPX", 2}, // Absolute
    {0xED, "SBC", 2}, // Absolute
    {0xEE, "INC", 2}, // Absolute
    {0xF0, "BEQ", 1}, // Relative
    {0xF1, "SBC", 1}, // (Indirect), Y
    {0xF5, "SBC", 1}, // Zero Page, X
    {0xF6, "INC", 1}, // Zero Page, X
    {0xF8, "SED", 0}, // Implied
    {0xF9, "SBC", 2}, // Absolute, Y
    {0xFD, "SBC", 2}, // Absolute, X
    {0xFE, "INC", 2}  // Absolute, X
};

// Calculate the number of valid opcodes (151 total)
const int OPCODES_COUNT = sizeof(opcodes_6502) / sizeof(OpcodeInfo);


int generate_test_binary(const char* filename) {
    FILE* file = fopen(filename, "wb");
    if (!file) {
        perror("Failed to open file");
        return 1;
    }

    // (added manually) before the instructions, write the load location as 2-byte word
    const uint16_t org_2byte = 0x1234;
    const uint8_t org_2byte_low = org_2byte & 0xFF;
    const uint8_t org_2byte_high = (org_2byte >> 8) & 0xFF;
    fwrite(&org_2byte_low, sizeof(uint8_t), 1, file);
    fwrite(&org_2byte_high, sizeof(uint8_t), 1, file);

    const uint8_t arg_1byte = 42;
    const uint16_t arg_2byte = 1337;
    
    // The 6502 is a little-endian architecture, so 16-bit addresses 
    // are stored with the least significant byte (LSB) first.
    const uint8_t arg_2byte_low = arg_2byte & 0xFF;         // 0x39 (57)
    const uint8_t arg_2byte_high = (arg_2byte >> 8) & 0xFF; // 0x05 (5)

    for (int i = 0; i < OPCODES_COUNT; ++i) {
        // 1. Write the opcode
        fwrite(&opcodes_6502[i].opcode, sizeof(uint8_t), 1, file);

        // 2. Write the operand(s) if required
        if (opcodes_6502[i].operand_size == 1) {
            fwrite(&arg_1byte, sizeof(uint8_t), 1, file);
        } 
        else if (opcodes_6502[i].operand_size == 2) {
            fwrite(&arg_2byte_low, sizeof(uint8_t), 1, file);
            fwrite(&arg_2byte_high, sizeof(uint8_t), 1, file);
        }
    }

    fclose(file);

    return 0;
}

int main(void)
{
	generate_test_binary("valid_instructions.ref");
}
