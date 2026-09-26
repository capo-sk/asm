/* 6502 Assembler

   Copyright 2026 William Brioschi aka CAPo/Spinning Kids

   See LICENSE file
*/

#ifndef MACRO_HPP
#define MACRO_HPP

#include "source.hpp"
#include <list>
#include <map>

using std::string;

class MacroText: public SrcText {
private:
	std::list<string> text;
	std::list<string>::iterator current;
	//std::string textname;
	//unsigned linenum;
	std::string filename;
	unsigned fileline;
	std::list<string> params;
	std::list<string> values;

public:
	MacroText(string const &mname);
	virtual ~MacroText();

	void add_line(string const &tline);

	void add_param(string const &param);
	unsigned get_param_count() const;
	
	void new_invocation();
	void add_value(string const &value);
	unsigned get_value_count() const;

	virtual string get_location() const;

    virtual bool getline(string &buffer);

	virtual void advance_line();
	virtual void rewind();
};

class MacroTable: public std::map<string, MacroText &> {
public:
	void add(MacroText &macro);
};

#endif
