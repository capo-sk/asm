/* 6502 Assembler

   Copyright 2026 William Brioschi aka CAPo/Spinning Kids

   See LICENSE file
*/

#ifndef MACRO_HPP
#define MACRO_HPP

#include "source.hpp"
#include <list>
#include <map>

class MacroText: public SrcText {
private:
	std::list<std::string> text;
	std::list<std::string>::iterator current;
	//std::string textname;
	//unsigned linenum;
	std::string filename;
	unsigned fileline;
	std::list<std::string> params;
	std::list<std::string> const *values;

public:
	MacroText(std::string const &mname);
	virtual ~MacroText();

	void add_line(std::string const &tline);
	void add_param(std::string const &param);

	void invoke(SrcText const &callfrom, std::list<std::string> const &values);

	virtual std::string get_location() const;

    virtual bool getline(std::string &buffer);

	virtual void advance_line();
	virtual void rewind();
};

class MacroTable: public std::map<std::string, MacroText &> {
public:
	void add(MacroText &macro);
};

#endif
