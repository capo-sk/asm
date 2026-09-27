/* 6502 Assembler

   Copyright 2026 William Brioschi aka CAPo/Spinning Kids

   See LICENSE file
*/

#ifndef SRCSTACK_HPP
#define SRCSTACK_HPP

#include "common.hpp"
#include "source.hpp"
#include <unordered_set>
#include <stack>

class SrcStack: public SrcText {
private:
	std::stack<unique_ptr<SrcText>> sources;
	std::unordered_set<string> names;
	string first;

public:
	SrcStack(string const &fname);

	SrcText &get_current();
	bool is_present(string const &name) const;
	void new_source(unique_ptr<SrcText> source);
	void new_source_once(unique_ptr<SrcText> source);
	bool pop();
	virtual void rewind();
	virtual void reset();
	virtual string get_location() const;
	virtual bool getline(string &buffer);
};

#endif