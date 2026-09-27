/* 6502 Assembler

   Copyright 2026 William Brioschi aka CAPo/Spinning Kids

   See LICENSE file
*/

#ifndef MACRO_HPP
#define MACRO_HPP

#include "source.hpp"
#include "common.hpp"
#include <list>
#include <map>

// MacroDefinition's are instantiated and populated by Parser during pass 1
// and accumulated into a MacroTable
// Lifecycle is from definition to end of program

// MacroInvocation's are instantiated during pass 2 and stored in SrcStack
// Lifecycle is from invocation until popped from the stack

// MacroInvocation realises SrcText
// MacroDefinition does not

class MacroInvocation;

class MacroDefinition {
private:
	std::list<string> text;
	//std::string textname;
	//unsigned linenum;
	std::list<string> params;

	friend MacroInvocation;

public:
	void add_line(string const &tline);

	void add_param(string const &param);
	unsigned get_param_count() const;
};

class MacroTable: public std::map<string, std::unique_ptr<MacroDefinition>> {
public:
	void add(string const &name, std::unique_ptr<MacroDefinition> macrop);
};

class MacroInvocation: public SrcText {
private:
	MacroDefinition const *definition;
	std::string filename;
	unsigned fileline;
	std::list<string> values;
	std::list<string>::const_iterator current;

public:
	MacroInvocation(string const &name, MacroDefinition const *def, SrcText const &callfrom);

	void add_value(string const &value);
	unsigned get_value_count() const;

	virtual string get_location() const;

    virtual bool getline(string &buffer);

	virtual void advance_line();
	virtual void rewind();
};

#endif
