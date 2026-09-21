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

	virtual std::string getLocation() const;
	virtual std::string const &getName() const;
	virtual unsigned getLine() const;

	virtual void rewind();

	virtual bool getline(char *buffer, unsigned size) = 0;
};

#endif
