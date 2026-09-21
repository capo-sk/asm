/* 6502 Assembler

   Copyright 2026 William Brioschi aka CAPo/Spinning Kids

   See LICENSE file
*/

#ifndef SRCFILE_HPP
#define SRCFILE_HPP

#include "source.hpp"
#include "nfile.hpp"
#include <unordered_set>
#include <stack>
#include <string>

class SrcFile: public SrcText {
private:
	NFile *nfile;

public:
	SrcFile(std::string const &fname);
	virtual ~SrcFile();

	virtual void rewind();

	virtual bool getline(char *buffer, unsigned size);
};

class SrcStack: public SrcText {
private:
	std::stack<SrcText *> sources;
	std::unordered_set<std::string> names;
	std::string first;

public:
	SrcStack(std::string const &fname);

	SrcText &get_current();
	bool is_present(std::string const &name);
	void new_source(SrcText *source);
	void new_source_once(SrcText *source);
	bool Pop();
	virtual void rewind();
	virtual void reset();
	virtual std::string get_location();
	virtual bool getline(char *buffer, unsigned max);
};

#endif
