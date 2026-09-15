/* 6502 Assembler

   Copyright 2026 William Brioschi aka CAPo/Spinning Kids

   See LICENSE file
*/

#ifndef MACRO_HPP
#define MACRO_HPP

#include "source.hpp"
#include <list>

class MacroText: public SrcText {
private:
	std::list<std::string> text;
	std::list<std::string>::iterator current;
	//std::string textname;
	//unsigned linenum;
	std::string filename;
	unsigned fileline;

public:
	MacroText(std::string const &mname);
	virtual ~MacroText();

	void InvokeAt(std::string const &fname, unsigned fline);
	void AddLine(std::string const &tline);

	virtual std::string getFileLocation();

	virtual void AdvanceLine();
	virtual void Rewind();
};

#endif
