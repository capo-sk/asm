/* 6502 Assembler

   Copyright 2026 William Brioschi aka CAPo/Spinning Kids

   See LICENSE file
*/

#ifndef SOURCE_HPP
#define SOURCE_HPP

#include <string>

class SrcText {
private:
	std::string textname;
	unsigned linenum;

protected:
	virtual void advance_line();

public:
	SrcText(std::string const &tname);
	virtual ~SrcText() = default;

	virtual std::string get_location() const;
	virtual std::string const &get_name() const;
	virtual unsigned get_linenum() const;

	virtual void rewind();

	virtual bool getline(char *buffer, unsigned size) = 0;
};

#endif
