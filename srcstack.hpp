/* 6502 Assembler

   Copyright 2026 William Brioschi aka CAPo/Spinning Kids

   See LICENSE file
*/

#ifndef SRCSTACK_HPP
#define SRCSTACK_HPP

#include "source.hpp"
#include <unordered_set>
#include <stack>
#include <string>
using std::string;

class SrcStack: public SrcText {
private:
	std::stack<SrcText *> sources;
	std::unordered_set<string> names;
	string first;

public:
	SrcStack(string const &fname);

	SrcText &get_current();
	bool is_present(string const &name) const;
	void new_source(SrcText *source);
	void new_source_once(SrcText *source);
	bool pop();
	virtual void rewind();
	virtual void reset();
	virtual std::string get_location() const;
	virtual bool getline(string &buffer);
};

#endif