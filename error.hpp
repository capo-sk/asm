/* 6502 Assembler

   Copyright 2026 William Brioschi aka CAPo/Spinning Kids

   See LICENSE file
*/

#ifndef ERROR_HPP
#define ERROR_HPP

#include <string>
using std::string;

#include "buffer.hpp"

class ErrorProvider {
   Buffer const *source;

public:
   ErrorProvider() = default;
   ErrorProvider(Buffer const &src): source(&src) {}
   ~ErrorProvider() = default;

   void set_source(Buffer const &src) { source = &src; }
	[[noreturn]] void error(string const &msg) const;
};

class ErrorConsumer {
private:
   ErrorProvider const *err;

   protected:
	[[noreturn]] void error(string const &msg) const;
   
public:
	void set_error_provider(ErrorProvider const &ep);
   void clear_error_provider();
};

[[noreturn]] extern void abort_now();
[[noreturn]] extern void abort_msg(string const &msg);
[[noreturn]] extern void abort_sys(string const &text);

#endif
