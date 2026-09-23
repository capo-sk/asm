/* 6502 Assembler

   Copyright 2026 William Brioschi aka CAPo/Spinning Kids

   See LICENSE file
*/

#ifndef EMIT_HPP
#define EMIT_HPP

#include <cstdint>
#include <ostream>
#include "errorpro.hpp"

class Emitter {
private:
	std::ostream &nfile;
	ErrorProvider *err;
	unsigned pass;
	int flag;
	uint16_t loc;
	uint16_t last_loc;

	void emit_next(void);

public:
	Emitter(std::ostream &a_nfile);
	void error_provider(ErrorProvider *ep);
	uint16_t get_loc() { return loc; }
	void set_loc(uint16_t a_loc) { loc = a_loc; }
	void set_pass(unsigned a_pass) { pass = a_pass; }
	void emit_byte(uint8_t value);
	void emit_word(uint16_t value);
	void emit_bytes(uint8_t const *value, uint16_t count);
	void emit_instruction(uint8_t index, uint8_t mode, uint16_t number, uint8_t size);
};

#endif
