/* 6502 Assembler

   Copyright 2026 William Brioschi aka CAPo/Spinning Kids

   See LICENSE file
*/

#include "errorpro.hpp"
#include "error.h"

[[noreturn]] void ErrorConsumer::error(std::string const &msg)
{
    if (err)
        err->error(msg);
    else
        abort_msg((std::string("ERROR: ") + msg).c_str());

    abort();  // only because GCC gets confused and claims function returns
}

void ErrorConsumer::set_error_provider(ErrorProvider *ep)
{
	err = ep;
}
