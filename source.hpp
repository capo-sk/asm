/* 6502 Assembler

   Copyright 2026 William Brioschi aka CAPo/Spinning Kids

   See LICENSE file
*/

#ifndef SOURCE_HPP
#define SOURCE_HPP

#include <string>

using std::string;

class SrcText {
private:
	string textname;
	unsigned linenum;

protected:
	virtual void advance_line();

public:
	SrcText(string const &tname);
	virtual ~SrcText() = default;

	virtual string get_location() const;
	virtual string const &get_name() const;
	virtual unsigned get_linenum() const;

	virtual void rewind();

	virtual bool getline(string &buffer) = 0;
};

#endif
